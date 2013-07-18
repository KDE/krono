/**
 * Copyright (C) 2012 Torrie Fischer <tdfischer@kde.org>
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

#include "KronoApp.h"

#include "MainWindow.h"
#include <KDE/KStatusNotifierItem>

int KronoApp::newInstance()
{
    static bool first = true;
    if (!first) {
        m_window->show();
        m_window->raise();
    } else {
        m_statusIcon = new KStatusNotifierItem(this);
        m_window = new MainWindow;
        m_window->setAttribute(Qt::WA_DeleteOnClose, false);
        setQuitOnLastWindowClosed(false);

        m_statusIcon->setAssociatedWidget(m_window);
        m_statusIcon->setCategory(KStatusNotifierItem::ApplicationStatus);
        m_statusIcon->setTitle(tr("Krono"));
        m_statusIcon->setStatus(KStatusNotifierItem::Passive);
        m_statusIcon->setIconByName("chronometer");

        connect(m_statusIcon, SIGNAL(activateRequested(bool,const QPoint&)), this, SLOT(activateWindow(bool,const QPoint&)));
        m_window->show();
    }
    first = false;
    return 0;
}

void KronoApp::activateWindow(bool active, const QPoint &pos)
{
    if (active)
        m_window->show();
    else
        m_window->hide();
}

#include "KronoApp.moc"
