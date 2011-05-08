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

#include "RefreshJob.h"

#include <QtCore/QDebug>

RefreshJob::RefreshJob(const QtZeitgeist::DataModel::TimeRange &range,
           const QtZeitgeist::DataModel::EventList &templates,
           QtZeitgeist::Log::StorageState state,
           uint maxEvents,
           QtZeitgeist::Log::ResultType type,
           QtZeitgeist::Log *log,
           QObject *parent)
    : ThreadWeaver::Job(parent)
    , m_range(range)
    , m_templates(templates)
    , m_state(state)
    , m_count(maxEvents)
    , m_type(type)
    , m_log(log)
{
    m_reply = m_log->findEvents(m_range, m_templates, m_state, m_count, m_type);
}

RefreshJob::~RefreshJob()
{
}

void RefreshJob::run()
{
    m_reply.waitForFinished();
    if (m_reply.isValid()) {
        setFinished(true);
    } else {
        qDebug() << m_reply.error();
        setFinished(false);
    }
}

QtZeitgeist::DataModel::EventList RefreshJob::results() const
{
    return m_reply.value();
}

#include "RefreshJob.moc"
