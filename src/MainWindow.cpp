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

#include <KDE/KCategorizedView>
#include <KDE/KCategorizedSortFilterProxyModel>
#include <KDE/KCategoryDrawer>
#include <KDE/KRun>
#include <KDE/KHelpMenu>
#include <KDE/KStandardAction>
#include <KDE/KApplication>
#include <KDE/KActionCollection>
#include <KDE/KMenu>
#include <KDE/KAction>
#include <KDE/KCmdLineArgs>

#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>

#include <QtCore/QDebug>

#include <QZeitgeist/Interpretation>
#include <QZeitgeist/LogModel>

#include "ui_MainWindow.h"

#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGUI(Default, "krono_ui");
    setupActions();

    setAutoSaveSettings();

    m_model = new QZeitgeist::LogModel(this);

    KCategorizedSortFilterProxyModel *sort = new KCategorizedSortFilterProxyModel(this);
    sort->setCategorizedModel(true);
    sort->setSourceModel(m_model);
    ui->searchBox->setProxy(sort);

    m_drawer = new KCategoryDrawerV3(ui->historyView);
    ui->historyView->setCategoryDrawer(m_drawer);
    ui->historyView->setSpacing(KDialog::spacingHint());
    ui->historyView->setItemDelegate(new EventDelegate(this));
    ui->historyView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->historyView->setModel(sort);

    m_velocityTimer = new QTimer(this);
    m_velocityTimer->setInterval(50);
    connect(m_velocityTimer, SIGNAL(timeout()), this, SLOT(applyVelocity()));

    m_slideTimer = new QTimer(this);
    m_slideTimer->setInterval(1000);
    connect(m_slideTimer, SIGNAL(timeout()), this, SLOT(slideRange()));

    connect(ui->timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(addTimeVelocity(int)));
    connect(ui->timeSlider, SIGNAL(sliderPressed()), this, SLOT(startVelocity()));
    connect(ui->timeSlider, SIGNAL(sliderReleased()), this, SLOT(stopVelocity()));

    connect(ui->historyView, SIGNAL(activated(const QModelIndex &)), this, SLOT(eventActivated(const QModelIndex &)));

    connect(ui->todayButton, SIGNAL(clicked()), this, SLOT(goToToday()));

    connect(ui->startTime, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(setStart(const QDateTime &)));
    connect(ui->endTime, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(setEnd(const QDateTime &)));

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

    goToToday();
}

void MainWindow::filterChanged(const QModelIndex &idx)
{
    QString uri = idx.data(CategoryModel::URIRole).toString();
    QZeitgeist::DataModel::EventList templates;
    QZeitgeist::DataModel::Event evt;
    evt.setInterpretation(uri);
    templates << evt;
    m_model->setEventTemplates(templates);
}

void MainWindow::slideRange()
{
    slideRange(1000);
}

void MainWindow::addTimeVelocity(int scale)
{
    int velocity = pow(2, abs(scale)) * ((scale> 0) ? 1 : -1);
    slideRange(velocity);
}

void MainWindow::slideRange(int msec)
{
    QZeitgeist::DataModel::TimeRange range = m_model->range();
    range.setBegin(range.begin()+msec);
    range.setEnd(range.end()+msec);
    m_model->setRange(range);
    QDateTime start = QDateTime::fromMSecsSinceEpoch(range.begin());
    QDateTime end = QDateTime::fromMSecsSinceEpoch(range.end());
    ui->startTime->setDateTime(start);
    ui->endTime->setDateTime(end);
}

void MainWindow::eventActivated(const QModelIndex &idx)
{
    new KRun(KUrl(idx.data(QZeitgeist::LogModel::URLRole).toUrl()), this);
}

void MainWindow::setStart(const QDateTime &time)
{
    m_slideTimer->stop();
    QZeitgeist::DataModel::TimeRange range = m_model->range();
    range.setBegin(time.toMSecsSinceEpoch());
    m_model->setRange(range);
}

void MainWindow::setEnd(const QDateTime &time)
{
    m_slideTimer->stop();
    QZeitgeist::DataModel::TimeRange range = m_model->range();
    range.setEnd(time.toMSecsSinceEpoch());
    m_model->setRange(range);
}

void MainWindow::goToToday()
{
    QDateTime end(QDateTime::currentDateTime());
    QDateTime start = end.addDays(-7);
    QZeitgeist::DataModel::TimeRange range(start.toMSecsSinceEpoch(), end.toMSecsSinceEpoch());
    m_model->setRange(range);
    m_slideTimer->start();
}

void MainWindow::applyVelocity()
{
    addTimeVelocity(ui->timeSlider->value());
}

void MainWindow::startVelocity()
{
    m_slideTimer->stop();
    m_velocityTimer->start();
}

void MainWindow::stopVelocity()
{
    m_velocityTimer->stop();
    QPropertyAnimation *anim = new QPropertyAnimation(ui->timeSlider, "value", this);
    anim->setEndValue(0);
    anim->setDuration(500);
    anim->setEasingCurve(QEasingCurve::OutElastic);
    anim->start();
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
