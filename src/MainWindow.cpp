/**
 * Copyright (C) 2011 Trever Fischer <tdfischer@fedoraproject.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "MainWindow.h"
#include "EventDelegate.h"
#include "CategoryModel.h"
#include "CategorizedLogModel.h"

#include <KDE/KCategorizedSortFilterProxyModel>
#include <KDE/KRun>
#include <KDE/KHelpMenu>
#include <KDE/KStandardAction>
#include <KDE/KApplication>
#include <KDE/KActionCollection>
#include <KDE/KMenu>
#include <KDE/KAction>
#include <KDE/KCmdLineArgs>

#include <QtCore/QDebug>

#include <QZeitgeist/Interpretation>
#include <QZeitgeist/LogModel>

#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGUI();
    setupActions();

    setAutoSaveSettings();

    m_model = new CategorizedLogModel(this);

    KCategorizedSortFilterProxyModel *sort = new KCategorizedSortFilterProxyModel(this);
    sort->setCategorizedModel(true);
    sort->setSourceModel(m_model);
    ui->searchBox->setProxy(sort);

    ui->historyView->setModel(sort);


    connect(ui->historyView, SIGNAL(activated(const QModelIndex &)), this, SLOT(eventActivated(const QModelIndex &)));

    m_categories = new CategoryModel(this);

    Category c;

    c.uri = "";
    c.icon = KIcon("view-history");
    c.description = tr("All");
    m_categories->addCategory(c);

    c.uri = QZeitgeist::Interpretation::Subject::NFOAudio;
    c.icon = KIcon("audio-x-generic");
    c.description = tr("Audio");
    m_categories->addCategory(c);

    c.uri = QZeitgeist::Interpretation::Subject::NFODocument;
    c.icon = KIcon("applications-office");
    c.description = tr("Documents");
    m_categories->addCategory(c);

    c.uri = QZeitgeist::Interpretation::Subject::NFOApplication;
    c.icon = KIcon("applications-other");
    c.description = tr("Applications");
    m_categories->addCategory(c);

    c.uri = QZeitgeist::Interpretation::Subject::NFOWebsite;
    c.icon = KIcon("applications-internet");
    c.description = tr("Websites");
    m_categories->addCategory(c);

    c.uri = QZeitgeist::Interpretation::Subject::NMOMessage;
    c.icon = KIcon("view-pim-contacts");
    c.description = tr("Conversations");
    m_categories->addCategory(c);

    c.uri = QZeitgeist::Interpretation::Subject::NFOVideo;
    c.icon = KIcon("video-x-generic");
    c.description = tr("Video");
    m_categories->addCategory(c);

    ui->categoryPicker->setModel(m_categories);

    connect(ui->categoryPicker->selectionModel(), SIGNAL(currentChanged(const QModelIndex, const QModelIndex)), this, SLOT(filterChanged(const QModelIndex)));

    connect(ui->timeSlider, SIGNAL(rangeChanged(const QZeitgeist::DataModel::TimeRange&)), m_model, SLOT(setRange(const QZeitgeist::DataModel::TimeRange&)));
    ui->timeSlider->goToToday();
}

void MainWindow::filterChanged(const QModelIndex &idx)
{
    QString uri = idx.data(CategoryModel::URIRole).toString();
    QZeitgeist::DataModel::Event evt;
    QZeitgeist::DataModel::Subject s;
    s.setInterpretation(uri);
    evt.setSubjects(QZeitgeist::DataModel::SubjectList() << s);
    m_model->setEventTemplates(QZeitgeist::DataModel::EventList() << evt);
}

void MainWindow::eventActivated(const QModelIndex &idx)
{
    new KRun(KUrl(idx.data(QZeitgeist::LogModel::URLRole).toUrl()), this);
}

void MainWindow::openPrefs()
{

}

void MainWindow::setupActions()
{
    m_helpMenu = new KHelpMenu(this, KCmdLineArgs::aboutData());
    m_mainMenu = m_helpMenu->menu();
    ui->optionsButton->setMenu(m_mainMenu);
    ui->optionsButton->setIcon(KIcon("krono"));

    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
    m_mainMenu->addAction(KStandardAction::preferences(this, SLOT(openPrefs()), actionCollection()));
}

#include "MainWindow.moc"
