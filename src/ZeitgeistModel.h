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

#ifndef ZEITGEISTMODEL_H
#define ZEITGEISTMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtDBus/QDBusPendingReply>
#include <QtGui/QIcon>

#include <QtZeitgeist/DataModel/Event>
#include <QtZeitgeist/DataModel/TimeRange>
#include <QtZeitgeist/Log>

namespace ThreadWeaver {
    class Job;
}

namespace QtZeitgeist {
    class Log;
}

class ZeitgeistModel : public QAbstractItemModel {
    Q_OBJECT

    public:
        enum {
            EventRole = Qt::UserRole+1,
            TimeRole,
            IDRole,
            URLRole,
            MimeRole,
        } Roles;
        ZeitgeistModel(QObject *parent);
        virtual ~ZeitgeistModel();
        int rowCount(const QModelIndex &idx = QModelIndex()) const;
        int columnCount(const QModelIndex &idx = QModelIndex()) const;
        QVariant data(const QModelIndex &idx, int role) const;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &idx) const;
        QIcon iconForEvent(const QtZeitgeist::DataModel::Event &event) const;
        QStringList eventIconOverlays(const QtZeitgeist::DataModel::Event &event) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;

        void setRange(const QtZeitgeist::DataModel::TimeRange &range);
        QtZeitgeist::DataModel::TimeRange range() const;

        void setResultType(QtZeitgeist::Log::ResultType type);
        QtZeitgeist::Log::ResultType resultType() const;

    public slots:
        void refresh();

    private slots:
        void refreshDone(ThreadWeaver::Job *job);

    private:
        void cacheIcon(const QtZeitgeist::DataModel::Event &event);
        void diffEvents(const QtZeitgeist::DataModel::EventList &events);
        QHash<QString, QString> m_iconCache;
        QtZeitgeist::Log *m_log;
        QtZeitgeist::DataModel::TimeRange m_range;
        QtZeitgeist::DataModel::EventList m_eventTemplates;
        QtZeitgeist::Log::StorageState m_storageState;
        QDBusPendingReply<QtZeitgeist::DataModel::EventList> m_reply;
        QtZeitgeist::DataModel::EventList m_events;
        QtZeitgeist::Log::ResultType m_type;
};

#endif // ZEITGEISTMODEL_H
