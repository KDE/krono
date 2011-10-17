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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QModelIndex>
#include <KDE/KXmlGuiWindow>

namespace QZeitgeist {
class LogModel;
}

class CategoryModel;

class QTimer;

class KCategoryDrawer;
class KHelpMenu;

namespace Ui {
    class MainWindow;
}

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);

    private slots:
        void addTimeVelocity(int scale);
        void startVelocity();
        void stopVelocity();
        void applyVelocity();
        void slideRange();
        void slideRange(int msec);
        void goToToday();
        void eventActivated(const QModelIndex &);
        void setStart(const QDateTime &);
        void setEnd(const QDateTime &);
        void openPrefs();
        void filterChanged(const QModelIndex &);

    private:
        void setupActions();
        QZeitgeist::LogModel *m_model;
        QTimer *m_velocityTimer;
        QTimer *m_slideTimer;
        Ui::MainWindow *ui;
        KCategoryDrawer *m_drawer;
        CategoryModel *m_categories;
        KHelpMenu *m_helpMenu;
        KMenu *m_mainMenu;
};

#endif //MAINWINDOW_H
