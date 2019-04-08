//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofMain.h"
#include "ofxICalendar.h"
#include "CalendarWidget.h"


using namespace ofx::Time;


class ofApp: public ofBaseApp
{
public:
    void setup();
    void draw();

    void gotMessage(ofMessage msg);

    void onCalendarWatcherEventAdded(const ICalendarEventInstance& instance);
    void onCalendarWatcherEventRemoved(const ICalendarEventInstance& instance);
    void onCalendarWatcherEventModified(const ICalendarEventInstance& instance);
    void onCalendarWatcherEventStarted(const ICalendarEventInstance& instance);
    void onCalendarWatcherEventEnded(const ICalendarEventInstance& instance);
    void onCalendarWatcherError(const Poco::Exception& exception);

    void processInstance(const ICalendarEventInstance& instance);

    ICalendar::SharedPtr calendar;
    ICalendarWatcher::SharedPtr watcher;

    CalendarWidget::SharedPtr calendarWidget;

    enum
    {
        MAX_MESSAGES = 500
    };

    std::deque<std::string> messages;

    ofColor currentColor;
    float currentSpeed;

};
