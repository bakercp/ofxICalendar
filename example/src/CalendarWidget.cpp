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


#include "CalendarWidget.h"


CalendarWidget::CalendarWidget(ICalendar::SharedPtr calendar, const ofRectangle& window):
    _calendar(calendar),
    _window(window)
{
    _font.load("automat.ttf", 6, false);

    // make the mapping pixel-perfect to avoid pixel rounding artifacts
    _windowTimespan = Poco::Timespan(15 * _window.getHeight() * Poco::Timespan::SECONDS);

    ofAddListener(ofEvents().update, this, &CalendarWidget::update);
}

CalendarWidget::~CalendarWidget()
{
    ofRemoveListener(ofEvents().update, this, &CalendarWidget::update);
}

void CalendarWidget::update(ofEventArgs& args)
{
    _now.update();
    _nowDateTime = _now ;

    _windowInterval.setFromCenter(_now, _windowTimespan);

    if (_now > _nextRefresh)
    {
        _currentEvents = _calendar->getEventInstances(_windowInterval);

        _nextRefresh += Poco::Timespan::MINUTES * 1;
    }
}

void CalendarWidget::draw()
{
    ofFill();
    ofSetColor(0, 200);

    ofDrawRectangle(_window);

    std::string formatStringHourMin = "%h:%M %A";
    std::string formatStringHour = "%h:%M";
    std::string formatStringHourMinSec = "%h:%M:%S %A";

    Poco::LocalDateTime minTime(_windowInterval.getStart());
    Poco::LocalDateTime maxTime(_windowInterval.getEnd());

    Poco::LocalDateTime startQuarter = Utils::ceiling(minTime, Poco::Timespan::MINUTES * 5);

    ofPushMatrix();
    ofTranslate(_window.getPosition());

    std::vector<Poco::Timestamp> hours = Utils::getInstances(startQuarter.utc().timestamp(),
                                                             maxTime.utc().timestamp(),
                                                             Period(Period::MINUTE, 5));

    std::vector<Poco::Timestamp>::const_iterator hourIter = hours.begin();

    while (hourIter != hours.end())
    {
        Poco::DateTime time = Poco::DateTime(*hourIter);

        int y = _window.getHeight() * _windowInterval.map(time.timestamp());

        int minute = time.minute();

        float alpha = ofMap(std::abs(_windowInterval.map(time.timestamp()) - 0.5), 0, .2, .25, 1, true);

        if (0 == minute)
        {
            ofSetColor(255, 80 * alpha);
            ofDrawLine(0, y, _window.getWidth(), y);
        }
        else if (0 == minute % 15)
        {
            ofSetColor(255, 255, 0, 80 * alpha);
            ofDrawLine(0, y, _window.getWidth(), y);
        }
        else
        {
            ofSetColor(127, 80 * alpha);
            ofDrawLine(0, y, _window.getWidth(), y);
        }

        std::string label = Utils::format(Poco::LocalDateTime(time), formatStringHourMinSec);

        int width = _font.stringWidth(label);
        int height = _font.stringHeight(label);

        if (y - height - 4 > 0)
        {
            _font.drawString(label, _window.getWidth() - width - 4, y - 4);
        }

        ++hourIter;
    }

    int y = _window.getHeight() * _windowInterval.map(_now);

    ofSetColor(255);
    ofDrawLine(0, y, _window.getWidth(), y);

    std::string label = Utils::format(Poco::LocalDateTime(_now), formatStringHourMinSec);

    int width = _font.stringWidth(label);

    _font.drawString(label, _window.getWidth() - width - 4, y - 4);

    std::sort(_currentEvents.begin(), _currentEvents.end());


    ICalendar::EventInstances::const_iterator iter = _currentEvents.begin();

    int x = 0;

    while (iter != _currentEvents.end())
    {
        const ICalendarEvent& event = (*iter).getEvent();
        const Interval& interval = (*iter).getInterval();

        if (_windowInterval.intersects(interval))
        {
            int y0 = _window.getHeight() * _windowInterval.map(interval.getStart());
            int y1 = _window.getHeight() * _windowInterval.map(interval.getEnd());

            ofFill();
            ofSetColor(255, 50);

            if (interval.contains(_now))
            {
                ofSetColor(255, 255, 0, 50);
            }
            else
            {
                ofSetColor(255, 50);
            }


            ofDrawRectRounded(x, y0, 80, y1 - y0, 5);

            ofNoFill();
            ofSetColor(127);
            ofDrawRectRounded(x, y0, 80, y1 - y0, 5);

            ofSetColor(255);
            ofDrawRectRounded(x-1, y0-1, 80+2, y1 - y0+2, 5);


            std::string startLabel = Utils::format(Poco::LocalDateTime(interval.getStart()), formatStringHour);
            std::string endLabel = Utils::format(Poco::LocalDateTime(interval.getEnd()), formatStringHour);

            ofFill();
            ofSetColor(255);
            _font.drawString(event.getSummary(), x + 5, y0 + 10);
            _font.drawString(startLabel,         x + 5, y0 + 20);
            _font.drawString(endLabel,           x + 5, y0 + 30);
            
            x+= 84;
            
            if (x > _window.getWidth() - 160) x = 0;
            
        }
        
        ++iter;
    }
    
    ofPopMatrix();
}
