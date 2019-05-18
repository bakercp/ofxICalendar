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

    std::string url = "basic.ics";

//    std::string url = "https://www.google.com/calendar/ical/christopherbaker.net_91ul9n5dq2b6pkmin511q3bq14%40group.calendar.google.com/public/basic.ics";

    // update it every minute
    calendar = ICalendar::makeShared(url);

    calendar->reload();
    calendar->startThread();

    watcher = ICalendarWatcher::makeShared(calendar);
    
    watcher->registerAllEvents(this); // register this class for all events

    // set up the watcher
    ofRectangle window(0,0,80,ofGetHeight());
    calendarWidget = CalendarWidget::makeShared(calendar, window);


    ICalendarWatcherEvents e;

//    e.one
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
    ofDrawBitmapString("printInterval: " + ofToString(printInterval), 20, ofGetHeight() - 15);
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

        ofJson parameters;

        try
        {
            parameters = ofJson::parse(description);
        }
        catch (const std::exception& exc)
        {
            ofLogError("ofApp::processInstance") << "Unable to parse description: " << exc.what();
            return;
        }

        printInterval = parameters.value("print_interval", 0);

        cout << "XX" << printInterval << endl;

    }
    else
    {
        printInterval = 0;

        cout << ">>" << printInterval << endl;
    }
}


void ofApp::onCalendarWatcherEventAdded(const ICalendarEventInstance& instance)
{
    ofSendMessage("ADDED: " + instance.getEvent().getSummary() );
    processInstance(instance);
}


void ofApp::onCalendarWatcherEventRemoved(const ICalendarEventInstance& instance)
{
    // note, there is a good chance this event will be invalid
    ofSendMessage("REMOVED: " + instance.getEvent().getUID());

    printInterval = 0;

    cout << ">>" << printInterval << endl;
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

    printInterval = 0;

    cout << ">>" << printInterval << endl;
}


void ofApp::onCalendarWatcherError(const Poco::Exception& exception)
{
    ofSendMessage("ERROR: " + exception.displayText());
}
