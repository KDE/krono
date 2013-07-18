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

#include "EventDelegate.h"

#include <QZeitgeist/LogModel>

#include <QtGui/QPainter>
#include <QtGui/QIcon>
#include <QtCore/QDateTime>
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QCryptographicHash>
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
    return QSize(350, 100);
}

void EventDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &idx) const
{
    QPixmap icon;
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

    QString url(idx.data(QZeitgeist::LogModel::URLRole).toString());
    QPixmap overlay;
    if (!m_iconCache.contains(url)) {
      icon = idx.data(QZeitgeist::LogModel::ThumbnailRole).value<QPixmap>();
      if (icon.isNull()) {
          KIcon kicon = KIcon(QUrl(idx.data(QZeitgeist::LogModel::ActorRole).toString()).authority().section(".desktop", 0, 0));
          icon = kicon.pixmap(iconRect.size());
      } else {
          icon = icon.scaled(iconRect.size(), Qt::KeepAspectRatio);
          KIcon kicon = KIcon(QUrl(idx.data(QZeitgeist::LogModel::ActorRole).toString()).authority().section(".desktop", 0, 0));
          QPainter overlayPainter(&icon);
          QRect overlayRect(QPoint(0, 0), iconRect.size());
          int iconSize = qMin(overlayRect.width(), overlayRect.height());
          overlayRect.setX(iconSize/2);
          overlayRect.setY(iconSize/2);
          kicon.paint(&overlayPainter, overlayRect, Qt::AlignCenter, QIcon::Active);
      }
      m_iconCache[url] = icon;
    } else {
      icon = m_iconCache[url];
    }

    painter->drawPixmap(iconRect, icon);
    
    //icon.paint(painter, iconRect, Qt::AlignCenter, mode);

    painter->setOpacity(1);
    QFont font;
    font.setBold(true);
    font.setPixelSize(15);
    painter->setFont(font);

    painter->drawText(baseRect, Qt::AlignLeft | Qt::TextWordWrap, text);
    painter->restore();

    painter->drawText(baseRect, Qt::AlignRight | Qt::AlignBottom, time.toString(Qt::SystemLocaleShortDate));
}

#include "EventDelegate.moc"
