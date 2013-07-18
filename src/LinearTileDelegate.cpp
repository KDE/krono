/**
 * Copyright (C) 2011 Torrie Fischer <tdfischer@kde.org>
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

#include "LinearTileDelegate.h"
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

LinearTileDelegate::LinearTileDelegate(LinearBrowser *view, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_view(view)
{
}

void LinearTileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &idx) const
{
    QIcon icon = qVariantValue<QIcon>(idx.data(Qt::DecorationRole));
    QString text = idx.data(Qt::DisplayRole).toString();

    QStyle *style = QApplication::style();

    QStyleOptionButton buttonStyle;
    buttonStyle.initFrom(m_view);
    buttonStyle.rect = option.rect;
    buttonStyle.state = option.state;
    buttonStyle.text = text;
    buttonStyle.icon = icon;
    buttonStyle.iconSize = QSize(16, 16);

    if (option.state & QStyle::State_Selected)
      buttonStyle.state |= QStyle::State_Sunken;

    style->drawControl(QStyle::CE_PushButton, &buttonStyle, painter);
}

QSize LinearTileDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFontMetrics fm(option.font);
    QIcon icon = qVariantValue<QIcon>(index.data(Qt::DecorationRole));
    QString text = index.data(Qt::DisplayRole).toString();
    int width = fm.width(text)+m_view->spacing()*4;
    int height = m_view->height()-m_view->spacing()*2;

    width += height/2;
    return QSize(width, height);
}

#include "LinearTileDelegate.moc"
