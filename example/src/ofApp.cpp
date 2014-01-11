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


#include "ofApp.h"


//------------------------------------------------------------------------------
void ofApp::setup()
{
    ofEnableAlphaBlending();
    ofSetFrameRate(30);

    ofColor currentColor = ofColor::black;
    float currentSpeed = 0;


    // currently basic.ics is being downloaded from here
    // "https://www.google.com/calendar/ical/christopherbaker.net_91ul9n5dq2b6pkmin511q3bq14%40group.calendar.google.com/public/basic.ics";

    // update it every minute
    calendar = ICalendar::makeShared("basic.ics", 60000);

    watcher = ICalendarWatcher::makeShared(calendar);
    
    watcher->registerAllEvents(this); // register this class for all events

    // set up the watcher
    ofRectangle window;
    window.setFromCenter(ofGetWidth() / 2, ofGetHeight() / 2, 480, ofGetHeight()-40);
    calendarWidget = CalendarWidget::makeShared(calendar, window);

}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    if (calendarWidget)
    {
        calendarWidget->draw();
    }

    int y = 30;

    std::deque<std::string>::const_iterator iter = messages.begin();

    ofDrawBitmapStringHighlight("Events:", 20, y);

    y += 10;

    while (iter != messages.end())
    {
        y += 14;

        ofDrawBitmapString(*iter, 20, y);

        if (y > ofGetHeight()) break;

        ++iter;
    }

    ofDrawBitmapStringHighlight("From the Current Event Description:", 20, ofGetHeight() - 45);
    ofDrawBitmapString("color: " + ofToString(currentColor), 20, ofGetHeight() - 30);
    ofDrawBitmapString("speed: " + ofToString(currentSpeed), 20, ofGetHeight() - 15);
}

//------------------------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//------------------------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//------------------------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//------------------------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//------------------------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    messages.push_front(msg.message);

    while (messages.size() > MAX_MESSAGES)
    {
        messages.pop_back(); // throw away the oldest.
    }
}

void ofApp::processInstance(const ICalendarEventInstance& instance)
{
    if (instance.isValidEventInstance())
    {
        std::string description = instance.getEvent().getDescription();

        std::vector<std::string> lines = ofSplitString(description, "\n", true, true);

        std::vector<std::string>::iterator iter = lines.begin();

        while (iter != lines.end())
        {
            std::vector<std::string> tokens = ofSplitString(*iter, "=", true, true);

            if (tokens.size() == 2)
            {
                if (tokens[0] == "color")
                {
                    std::vector<std::string> rgbTokens = ofSplitString(tokens[1], ",", true, true);

                    if (rgbTokens.size())
                    {
                        currentColor.set(ofToFloat(rgbTokens[0]), ofToFloat(rgbTokens[1]), ofToFloat(rgbTokens[2]));
                    }
                }
                else if(tokens[0] == "speed")
                {
                    currentSpeed = ofToFloat(tokens[1]);
                }
                else
                {
                    ofLogError("ofApp::processInstance") << "Unknown key.";
                }
            }

            ++iter;
        }
    }
}


//------------------------------------------------------------------------------
void ofApp::onCalendarWatcherEventAdded(const ICalendarEventInstance& instance)
{
    ofSendMessage("ADDED: " + instance.getEvent().getSummary() );

    processInstance(instance);
}

//------------------------------------------------------------------------------
void ofApp::onCalendarWatcherEventRemoved(const ICalendarEventInstance& instance)
{
    // note, there is a good chance this event will be invalid
    ofSendMessage("REMOVED: " + instance.getEvent().getUID());
}

//------------------------------------------------------------------------------
void ofApp::onCalendarWatcherEventModified(const ICalendarEventInstance& instance)
{
    ofSendMessage("MODIFIED: " + instance.getEvent().getSummary() );

    processInstance(instance);
}

//------------------------------------------------------------------------------
void ofApp::onCalendarWatcherEventStarted(const ICalendarEventInstance& instance)
{
    ofSendMessage("STARTED: " + instance.getEvent().getSummary() );

    processInstance(instance);
}

//------------------------------------------------------------------------------
void ofApp::onCalendarWatcherEventEnded(const ICalendarEventInstance& instance)
{
    ofSendMessage("ENDED: " + instance.getEvent().getSummary() );
}

//------------------------------------------------------------------------------
void ofApp::onCalendarWatcherError(const Poco::Exception& exception)
{
    ofSendMessage("ERROR: " + exception.displayText());
}
