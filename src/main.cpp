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

#include "ZeitgeistModel.h"
#include "EventDelegate.h"
#include "MainWindow.h"

#include <KDE/KApplication>
#include <QtGui/QListView>
#include <KDE/KAboutData>
#include <KDE/KLocale>
#include <KDE/KCmdLineArgs>

int main(int argc, char** argv)
{
    KAboutData aboutData(
        "Krono",
        0,
        ki18n("Krono"),
        "0.0.1",
        ki18n("Displays your history"),
        KAboutData::License_GPL,
        ki18n("(c) 2011"));
    aboutData.addAuthor(ki18n("Trever Fischer"), ki18n("Developer") ,"tdfischer@fedoraproject.org", "http://wm161.net/");
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;
    MainWindow *mw = new MainWindow;
    mw->show();
    return app.exec();
}
