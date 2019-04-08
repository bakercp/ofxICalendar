//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <set>
#include "ofx/Time/ICalendar.h"
#include "ofx/Time/Interval.h"
#include "ofx/Time/Utils.h"
#include "ofRectangle.h"
#include "ofTrueTypeFont.h"
#include "ofGraphics.h"


using namespace ofx::Time;


class CalendarWidget
{
public:
    typedef std::shared_ptr<CalendarWidget> SharedPtr;

    CalendarWidget(ICalendar::SharedPtr calendar, const ofRectangle& window);

    virtual ~CalendarWidget();

    void update(ofEventArgs& args);
        ///< A callback for the ofApp to keep everything in the
        ///< main thread.  Is automatically registered and unregistered
        ///< upon CalendarWidget construction and destruction.

    void draw();

    static SharedPtr makeShared(ICalendar::SharedPtr calendar, const ofRectangle& window)
    {
        return SharedPtr(new CalendarWidget(calendar, window));
    }

private:
    ICalendar::SharedPtr _calendar;
    ofRectangle _window;

    ICalendar::EventInstances _currentEvents;

    Poco::Timestamp _now;
    Poco::DateTime _nowDateTime;

    Poco::Timestamp _nextRefresh;

    Poco::Timespan _windowTimespan;

    Interval _windowInterval;

    ofTrueTypeFont _font;

};
