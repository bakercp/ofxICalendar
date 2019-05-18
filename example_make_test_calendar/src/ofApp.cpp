//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofApp.h"
#include "Poco/UUIDGenerator.h"



std::string makeEvent(const std::string& uid,
                      int startHour,
                      int startMinute,
                      int durationMinutes)
{


    std::string _hour = ofToString(startHour, 2, '0');
    std::string _minute = ofToString(startMinute, 2, '0');

    int endHour = startHour + (durationMinutes / 60);
    int endMinute = startMinute + (durationMinutes % 60);

    std::string _endHour = ofToString(endHour, 2, '0');
    std::string _endMinute = ofToString(endMinute, 2, '0');

    ofJson description = {
        { "start", { { "start_hour", startHour }, { "start_minute", startMinute } } },
        {   "end", { {   "end_hour",   endHour }, {   "end_minute",   endMinute } } }
    };

    std::stringstream summary;
    summary << _hour << ":" << _minute << " - " << durationMinutes << " minutes";

    std::stringstream ss;
    ss << "BEGIN:VEVENT" << std::endl;
    ss << "CREATED:19700101T000000Z" << std::endl;
    ss << "UID:" << uid << std::endl;
    ss << "RRULE:FREQ=DAILY;INTERVAL=1" << std::endl;
    ss << "DTEND;TZID=America/Denver:19700101T" << _hour << _minute << "00" << std::endl;
    ss << "TRANSP:OPAQUE" << std::endl;
    ss << "SUMMARY:" << summary.str() << std::endl;
    ss << "DTSTART;TZID=America/Denver:19700101T" << _endHour << _endMinute << "00" << std::endl;
    ss << "DTSTAMP:19700101T000000Z" << std::endl;
    ss << "SEQUENCE:0" << std::endl;
    ss << "DESCRIPTION:" << description.dump() << std::endl;
    ss << "END:VEVENT" << std::endl;
    return ss.str();

}



void ofApp::setup()
{
    std::stringstream ss;

    Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();

    for (int hour = 0; hour < 24; ++hour)
    {
        ss << makeEvent(Poco::UUID(generator.createRandom()).toString(),
                        hour,
                        0,
                        60);

        ss << std::endl;
        ss << std::endl;

        for (int minute = 0; minute < 60; ++minute)
        {
            ss << makeEvent(Poco::UUID(generator.createRandom()).toString(),
                            hour,
                            minute,
                            1);
            ss << std::endl;
            ss << std::endl;
        }
    }


    std::cout << ss.str() << std::endl;

}

