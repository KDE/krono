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

#include "EventDelegate.h"

#include <QZeitgeist/LogModel>

#include <QtGui/QPainter>
#include <QtGui/QIcon>
#include <QtCore/QDateTime>
#include <QtGui/QApplication>
#include <KDE/KIcon>
#include <QtCore/QDebug>
#include <QtCore/QUrl>

EventDelegate::EventDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

EventDelegate::~EventDelegate()
{
}

QSize EventDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &idx) const
{
    Q_UNUSED(option);
    Q_UNUSED(idx);
    return QSize(175, 50);
}

void EventDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &idx) const
{
    // Basically, pull 'krono' out of app://krono.desktop
    KIcon icon = KIcon(QUrl(idx.data(QZeitgeist::LogModel::ActorRole).toString()).authority().section(".desktop", 0, 0));
    QString text = idx.data(Qt::DisplayRole).toString();
    QDateTime time = idx.data(QZeitgeist::LogModel::TimeRole).toDateTime();

    QStyle *style = QApplication::style();

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    QRect baseRect = option.rect.adjusted(5, 5, -5, -5);

    QRect iconRect(baseRect);
    QRect descRect(baseRect);

    iconRect.setWidth(iconRect.width()/3);
    descRect.setX(iconRect.right());

    QIcon::Mode mode = QIcon::Normal;
    if (option.state == QStyle::State_Active)
        mode = QIcon::Active;
    if (option.state == QStyle::State_Selected)
        mode = QIcon::Selected;

    painter->save();
    painter->setOpacity(0.4);
    
    icon.paint(painter, iconRect, Qt::AlignCenter, mode);

    painter->setOpacity(1);
    QFont font;
    font.setBold(true);
    font.setPixelSize(10);
    painter->setFont(font);

    painter->drawText(baseRect, Qt::AlignLeft | Qt::TextWordWrap, text);
    painter->restore();

    painter->drawText(baseRect, Qt::AlignRight | Qt::AlignBottom, time.toString(Qt::SystemLocaleShortDate));
}

#include "EventDelegate.moc"
