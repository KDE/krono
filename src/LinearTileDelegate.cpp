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

    QFontMetrics fm(option.font);
    QRect iconRect = option.rect.adjusted(0, 0, 0, -fm.height());

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    icon.paint(painter, iconRect, Qt::AlignCenter, QIcon::Active);

    painter->drawText(option.rect, Qt::AlignCenter | Qt::AlignBottom, text);
}

QSize LinearTileDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    int size = qMin(m_view->height(), m_view->width())-m_view->spacing()*2;
    return QSize(size, size);
}

#include "LinearTileDelegate.moc"
