// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


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
