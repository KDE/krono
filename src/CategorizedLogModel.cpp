/**
 * Copyright (C) 2012 Trever Fischer <tdfischer@fedoraproject.org>
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

#include "CategorizedLogModel.h"

#include <KDE/KCategorizedSortFilterProxyModel>
#include <KDE/KLocalizedString>

CategorizedLogModel::CategorizedLogModel(QObject *parent)
    : QZeitgeist::LogModel(parent)
{
}


// Shortcut around insanely long class name
bool inline isDisplayRole(int role)
{
    return role == KCategorizedSortFilterProxyModel::CategoryDisplayRole;
}

QVariant CategorizedLogModel::data(const QModelIndex &index, int role) const
{
    if (role == KCategorizedSortFilterProxyModel::CategoryDisplayRole || role == KCategorizedSortFilterProxyModel::CategorySortRole) {
        QDateTime eventTime = QZeitgeist::LogModel::data(index, QZeitgeist::LogModel::TimeRole).toDateTime();
        QDateTime now = QDateTime::currentDateTime();
        qint64 days = eventTime.daysTo(now);
        if (days == 0) {
            qint64 hours = eventTime.secsTo(now)/60/60;
            if (hours <= 4 && !isDisplayRole(role))
                return QDateTime(eventTime.date(), QTime(eventTime.time().hour(), 0));
            else if (hours == 0)
                return i18n("Just Now");
            else if (hours == 1)
                return i18n("Last Hour");
            else if (hours <=4)
              return i18n("%1 hours ago", hours);
            int hour = eventTime.time().hour();
            if (hour < 12) {
                if (isDisplayRole(role))
                    return i18n("This morning");
                else
                    return QDateTime(now.date(), QTime(0, 0));
            }
            if (hour < 16) {
                if (isDisplayRole(role))
                    return i18n("This afternoon");
                else
                    return QDateTime(now.date(), QTime(12, 0));
            }
            if (isDisplayRole(role))
                return i18n("This evening");
            else
                return QDateTime(now.date(), QTime(16, 0));
        }
        if (days == 1) {
            if (isDisplayRole(role))
                return i18n("Yesterday");
            else
                return eventTime.date();
        }
        int weeks = days/7;
        if (weeks == 0) {
          if (isDisplayRole(role))
              return eventTime.toString("dddd");
          else
              return eventTime.date();
        }
        if (weeks < 3) {
            if (isDisplayRole(role)) {
                if (weeks == 1)
                    return i18n("Last week");
                else
                    return i18n("%1 weeks ago", weeks);
            } else {
                qint64 stamp = now.toMSecsSinceEpoch();
                stamp -= 1000*60*60*24*7*weeks;
                return QDateTime::fromMSecsSinceEpoch(stamp);
            }
        }
        if (weeks > 3) {
            if (isDisplayRole(role))
                return eventTime.toString(i18nc("Year+month, such as December, 2012.", "MMMM, yyyy"));
            else
                return QDate(eventTime.date().year(), eventTime.date().month(), 1);
        }
        Q_ASSERT(false);
    }
    return QZeitgeist::LogModel::data(index, role);
}

#include "CategorizedLogModel.moc"
