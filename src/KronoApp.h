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

#ifndef KRONOAPP_H
#define KRONOAPP_H

#include <KDE/KUniqueApplication>

class KStatusNotifierItem;
class MainWindow;

class KronoApp : public KUniqueApplication {
    Q_OBJECT
public:
    int newInstance();

private slots:
    void activateWindow(bool active, const QPoint &pos);
private:
    KStatusNotifierItem *m_statusIcon;
    MainWindow *m_window;
};

#endif // KRONOAPP_H
