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

#ifndef TIMELINESLIDER_H
#define TIMELINESLIDER_H

#include <QtGui/QWidget>
#include <QZeitgeist/DataModel/TimeRange>

namespace Ui {
    class TimelineSlider;
}

class TimelineSlider : public QWidget
{
    Q_OBJECT
    public:
        TimelineSlider(QWidget *parent = 0);
    public slots:
        void goToToday();
        void setStart(const QDateTime &);
        void setEnd(const QDateTime &);

    signals:
        void rangeChanged(const QZeitgeist::DataModel::TimeRange &range);

    private slots:
        void addTimeVelocity(int scale);
        void startVelocity();
        void stopVelocity();
        void applyVelocity();
        void slideRange();
        void slideRange(int msec);

    private:
        Ui::TimelineSlider *ui;
        QTimer *m_velocityTimer;
        QTimer *m_slideTimer;
        QZeitgeist::DataModel::TimeRange m_range;
};

#endif //TIMELINESLIDER_H
