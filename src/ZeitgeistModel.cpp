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
#include "RefreshJob.h"

#include <QtZeitgeist>
#include <QtZeitgeist/DataModel/Subject>

#include <QtCore/QDebug>
#include <QtCore/QUrl>

#include <KDE/KIcon>
#include <KDE/KService>
#include <KDE/KServiceTypeTrader>
#include <KDE/KCategorizedSortFilterProxyModel>
#include <KDE/ThreadWeaver/Weaver>


ZeitgeistModel::ZeitgeistModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QtZeitgeist::init();
    m_log = new QtZeitgeist::Log(this);
    m_storageState = QtZeitgeist::Log::Any;
    m_range = QtZeitgeist::DataModel::TimeRange::always();
    m_eventTemplates << QtZeitgeist::DataModel::Event(this);
    ThreadWeaver::Weaver::instance()->setMaximumNumberOfThreads(1);
}

ZeitgeistModel::~ZeitgeistModel()
{
}

void ZeitgeistModel::diffEvents(const QtZeitgeist::DataModel::EventList &events)
{
    //TODO: Implement dyanmic programming for a proper diff algorithm.
    //This probably depends on the datamodel objects not being QObjects.
    QtZeitgeist::DataModel::EventList newEvents = events;
    QtZeitgeist::DataModel::EventList::iterator currentIt = m_events.begin();
    QtZeitgeist::DataModel::EventList::iterator newIt = newEvents.begin();
    int currentRow = 0;
    while(currentIt != m_events.end() && newIt != newEvents.end()) {
        if (newIt->id() == currentIt->id()) {
            newIt++;
            currentIt++;
            currentRow++;
        } else if (newIt->timestamp() >= currentIt->timestamp()) {
            beginInsertRows(QModelIndex(), currentRow, currentRow);
            currentIt = m_events.insert(currentIt, *newIt);
            cacheIcon(*currentIt);
            endInsertRows();
            newIt = newEvents.erase(newIt);
            currentIt++;
            currentRow++;
        } else if (newIt->timestamp() < currentIt->timestamp()) {
            beginRemoveRows(QModelIndex(), currentRow, currentRow);
            currentIt = m_events.erase(currentIt);
            endRemoveRows();
        }
    }
    if (newIt != newEvents.end()) {
        beginInsertRows(QModelIndex(), currentRow, currentRow+newEvents.size()-1);
        while(newIt != newEvents.end()) {
            currentIt = m_events.insert(currentIt, *newIt);
            cacheIcon(*currentIt);
            currentRow++;
            currentIt++;
            newIt++;
        }
        endInsertRows();
    }
    if (currentIt != m_events.end()) {
        beginRemoveRows(QModelIndex(), currentRow, m_events.size()-1);
        while(currentIt != m_events.end()) {
            currentIt = m_events.erase(currentIt);
        }
        endRemoveRows();
    }
}

void ZeitgeistModel::refresh()
{
    ThreadWeaver::Job *refreshJob = new RefreshJob(m_range,
                                                   m_eventTemplates,
                                                   m_storageState,
                                                   10000,
                                                   QtZeitgeist::Log::MostRecentSubjects,
                                                   m_log,
                                                   this);

    connect(refreshJob, SIGNAL(done(ThreadWeaver::Job*)), this, SLOT(refreshDone(ThreadWeaver::Job*)));
    ThreadWeaver::Weaver::instance()->enqueue(refreshJob);
}

void ZeitgeistModel::refreshDone(ThreadWeaver::Job *job)
{
    RefreshJob *refreshJob = static_cast<RefreshJob*>(job);
    diffEvents(refreshJob->results());
}

int ZeitgeistModel::rowCount(const QModelIndex &idx) const
{
    if (idx.isValid())
        return 0;
    return m_events.size();
}

int ZeitgeistModel::columnCount(const QModelIndex &idx) const
{
    if (idx.isValid())
        return 0;
    return 1;
}

QModelIndex ZeitgeistModel::parent(const QModelIndex &idx) const
{
    Q_UNUSED(idx);
    return QModelIndex();
}

QVariant ZeitgeistModel::data(const QModelIndex &idx, int role) const
{
    if (idx.isValid() && idx.row() >= 0 && idx.row() < rowCount() && idx.column() == 0) {
        QtZeitgeist::DataModel::Event event = m_events[idx.row()];
        switch(role) {
            case Qt::DisplayRole:
                return event.subjects()[0].text();
            case Qt::DecorationRole:
                return iconForEvent(event);
            case EventRole:
                return QVariant::fromValue<QtZeitgeist::DataModel::Event>(event);
            case TimeRole:
                return event.timestamp();
            case IDRole:
                return event.id();
            case URLRole:
                return event.subjects()[0].uri();
            case MimeRole:
                return event.subjects()[0].mimeType();
            case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
                return event.timestamp().date();
            case KCategorizedSortFilterProxyModel::CategorySortRole:
                return event.timestamp().date();
            default:
                return QVariant();
        }
    }
    return QVariant();
}

void ZeitgeistModel::cacheIcon(const QtZeitgeist::DataModel::Event &event)
{
    QUrl actor(event.actor());
    QString desktopFile = actor.authority().section('.', 0, 0);
    if (!m_iconCache.contains(desktopFile)) {
        KService::List results = KServiceTypeTrader::self()->query("Application", QString("DesktopEntryName == '%0'").arg(desktopFile));
        if (results.size() > 0)
            m_iconCache[desktopFile] = results.at(0)->icon();
        else
            m_iconCache[desktopFile] = "utilities-desktop-extra";
    }
}

QIcon ZeitgeistModel::iconForEvent(const QtZeitgeist::DataModel::Event &event) const
{
    QUrl actor(event.actor());
    QString desktopFile = actor.authority().section('.', 0, 0);
    if (!m_iconCache.contains(desktopFile))
        return QIcon();
    return KIcon(m_iconCache[desktopFile], NULL, eventIconOverlays(event));
}

QModelIndex ZeitgeistModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid() && row >= 0 && row < rowCount() && column == 0)
        return createIndex(row, column);
    return QModelIndex();
}

void ZeitgeistModel::setRange(const QtZeitgeist::DataModel::TimeRange &range)
{
    m_range = range;
    refresh();
}

Qt::ItemFlags ZeitgeistModel::flags(const QModelIndex &index) const
{
    if (index.isValid() && index.row() >= 0 && index.row() < rowCount() && index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return 0;
}

QStringList ZeitgeistModel::eventIconOverlays(const QtZeitgeist::DataModel::Event &event) const
{
    QStringList overlays;
    QtZeitgeist::DataModel::Subject subject = event.subjects()[0];
    /*switch(subject.interpretation()) {
        case QtZeitgeist::Interpretation::Subject::NFOAudio:
            overlays << "applications-m
    }*/
    QString mime = subject.mimeType();
    overlays << mime.replace('/', '-');
    return overlays;
}

QtZeitgeist::DataModel::TimeRange ZeitgeistModel::range() const
{
    return m_range;
}

#include "ZeitgeistModel.moc"
