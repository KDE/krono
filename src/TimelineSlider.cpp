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

#include "TimelineSlider.h"

#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>

#include <math.h>

#include "ui_TimelineSlider.h"

TimelineSlider::TimelineSlider(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimelineSlider)
{
    ui->setupUi(this);
    m_velocityTimer = new QTimer(this);
    m_velocityTimer->setInterval(50);
    connect(m_velocityTimer, SIGNAL(timeout()), this, SLOT(applyVelocity()));

    m_slideTimer = new QTimer(this);
    m_slideTimer->setInterval(1000);
    connect(m_slideTimer, SIGNAL(timeout()), this, SLOT(slideRange()));

    connect(ui->timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(addTimeVelocity(int)));
    connect(ui->timeSlider, SIGNAL(sliderPressed()), this, SLOT(startVelocity()));
    connect(ui->timeSlider, SIGNAL(sliderReleased()), this, SLOT(stopVelocity()));

    connect(ui->todayButton, SIGNAL(clicked()), this, SLOT(goToToday()));

    connect(ui->startTime, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(setStart(const QDateTime &)));
    connect(ui->endTime, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(setEnd(const QDateTime &)));

}

void TimelineSlider::slideRange()
{
    slideRange(1000);
}

void TimelineSlider::addTimeVelocity(int scale)
{
    int velocity = pow(2, abs(scale)) * ((scale> 0) ? 1 : -1);
    slideRange(velocity);
}

void TimelineSlider::slideRange(int msec)
{
    m_range.setBegin(m_range.begin()+msec);
    m_range.setEnd(m_range.end()+msec);
    QDateTime start = QDateTime::fromMSecsSinceEpoch(m_range.begin());
    QDateTime end = QDateTime::fromMSecsSinceEpoch(m_range.end());
    ui->startTime->setDateTime(start);
    ui->endTime->setDateTime(end);
    emit rangeChanged(m_range);
}

void TimelineSlider::setStart(const QDateTime &time)
{
    m_slideTimer->stop();
    m_range.setBegin(time.toMSecsSinceEpoch());
}

void TimelineSlider::setEnd(const QDateTime &time)
{
    m_slideTimer->stop();
    m_range.setEnd(time.toMSecsSinceEpoch());
}

void TimelineSlider::goToToday()
{
    QDateTime end(QDateTime::currentDateTime());
    QDateTime start = end.addDays(-7);
    m_range = QZeitgeist::DataModel::TimeRange(start.toMSecsSinceEpoch(), end.toMSecsSinceEpoch());
    m_slideTimer->start();
}

void TimelineSlider::applyVelocity()
{
    addTimeVelocity(ui->timeSlider->value());
}

void TimelineSlider::startVelocity()
{
    m_slideTimer->stop();
    m_velocityTimer->start();
}

void TimelineSlider::stopVelocity()
{
    m_velocityTimer->stop();
    QPropertyAnimation *anim = new QPropertyAnimation(ui->timeSlider, "value", this);
    anim->setEndValue(0);
    anim->setDuration(500);
    anim->setEasingCurve(QEasingCurve::OutElastic);
    anim->start();
}

#include "TimelineSlider.moc"
