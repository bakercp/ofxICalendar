//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofEnableAlphaBlending();
    ofSetFrameRate(30);

    // currently basic.ics is being downloaded from here
    // "https://www.google.com/calendar/ical/christopherbaker.net_91ul9n5dq2b6pkmin511q3bq14%40group.calendar.google.com/public/basic.ics";

    // update it every minute
    calendar = std::make_shared<ICalendar>("basic.ics", 60000);

    calendar->reload();
    calendar->startThread();

    watcher = std::make_shared<ICalendarWatcher>(calendar);
    
    watcher->registerAllEvents(this); // register this class for all events

    // set up the watcher
    ofRectangle window;
    window.setFromCenter(ofGetWidth() / 2, ofGetHeight() / 2, 480, ofGetHeight()-40);
    calendarWidget = std::make_shared<CalendarWidget>(calendar, window);

}


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


void ofApp::onCalendarWatcherEventAdded(const ICalendarEventInstance& instance)
{
    ofSendMessage("ADDED: " + instance.getEvent().getSummary());

    processInstance(instance);
}


void ofApp::onCalendarWatcherEventRemoved(const ICalendarEventInstance& instance)
{
    // note, there is a good chance this event will be invalid
    ofSendMessage("REMOVED: " + instance.getEvent().getUID());
}


void ofApp::onCalendarWatcherEventModified(const ICalendarEventInstance& instance)
{
    ofSendMessage("MODIFIED: " + instance.getEvent().getSummary() );

    processInstance(instance);
}


void ofApp::onCalendarWatcherEventStarted(const ICalendarEventInstance& instance)
{
    ofSendMessage("STARTED: " + instance.getEvent().getSummary() );

    processInstance(instance);
}


void ofApp::onCalendarWatcherEventEnded(const ICalendarEventInstance& instance)
{
    ofSendMessage("ENDED: " + instance.getEvent().getSummary() );
}


void ofApp::onCalendarWatcherError(const Poco::Exception& exception)
{
    ofSendMessage("ERROR: " + exception.displayText());
}
