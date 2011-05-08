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

#ifndef REFRESHJOB_H
#define REFRESHJOB_H

#include <KDE/ThreadWeaver/Job>

#include <QtZeitgeist/Log>
#include <QtZeitgeist/DataModel/TimeRange>
#include <QtZeitgeist/DataModel/Event>

class RefreshJob : public ThreadWeaver::Job {
    Q_OBJECT

    public:
        RefreshJob(const QtZeitgeist::DataModel::TimeRange &range,
                   const QtZeitgeist::DataModel::EventList &templates,
                   QtZeitgeist::Log::StorageState state,
                   uint maxEvents,
                   QtZeitgeist::Log::ResultType type,
                   QtZeitgeist::Log *log,
                   QObject *parent = 0);
        ~RefreshJob();
        void run();
        QtZeitgeist::DataModel::EventList results() const;

    private:
        QtZeitgeist::DataModel::TimeRange m_range;
        QtZeitgeist::DataModel::EventList m_templates;
        QtZeitgeist::Log::StorageState m_state;
        uint m_count;
        QtZeitgeist::Log::ResultType m_type;
        QtZeitgeist::Log *m_log;
        QDBusPendingReply<QtZeitgeist::DataModel::EventList> m_reply;
};

#endif //REFRESHJOB_H
