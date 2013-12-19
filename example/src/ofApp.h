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


#include "ofMain.h"
#include "ofxICalendar.h"
#include "CalendarWidget.h"


using namespace ofx::Time;


class ofApp: public ofBaseApp
{
public:
    void setup();
    void draw();

    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void gotMessage(ofMessage msg);

    void onCalendarWatcherEventAdded(const ICalendarEventInstance& instance);
    void onCalendarWatcherEventRemoved(const ICalendarEventInstance& instance);
    void onCalendarWatcherEventModified(const ICalendarEventInstance& instance);
    void onCalendarWatcherEventStarted(const ICalendarEventInstance& instance);
    void onCalendarWatcherEventEnded(const ICalendarEventInstance& instance);
    void onCalendarWatcherError(const Poco::Exception& exception);

    ICalendar::SharedPtr calendar;
    ICalendarWatcher::SharedPtr watcher;

    CalendarWidget::SharedPtr calendarWidget;

    enum
    {
        MAX_MESSAGES = 500
    };

    std::deque<std::string> messages;

};
