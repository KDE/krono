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

#include "HistoryView.h"
#include "EventDelegate.h"

#include <QtGui/QResizeEvent>
#include <QtCore/QDebug>

#include <KDE/KDialog>
#include <KDE/KCategoryDrawer>

HistoryView::HistoryView(QWidget *parent)
    : KCategorizedView(parent)
{
    m_drawer = new KCategoryDrawerV3(this);
    setCategoryDrawer(m_drawer);
    setSpacing(KDialog::spacingHint());
    setItemDelegate(new EventDelegate(this));
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

#include "HistoryView.moc"
