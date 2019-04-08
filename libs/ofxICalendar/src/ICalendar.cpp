//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofx/Time/ICalendar.h"


namespace ofx {
namespace Time {


const Poco::Timespan ICalendar::DEFAULT_UPDATE_INTERVAL = 0;


ICalendar::ICalendar(const std::string& uri, unsigned long long autoRefreshInterval):
    _pICalendar(0),
    _uri(""),
//    _autoUpdateTimer(0, autoRefreshInterval),
    _nextUpdate(0),
    _autoUpdateInterval(autoRefreshInterval),
    _calendarBuffer("")
{
    ofAddListener(ofEvents().update, this, &ICalendar::update);

    setURI(uri);

//    _autoUpdateTimer.start(Poco::TimerCallback<ICalendar>(*this, &ICalendar::onAutoUpdate));
}


ICalendar::ICalendar(const ICalendar& other):
    _pICalendar(0),
    _uri(other._uri),
    _autoUpdateInterval(other._autoUpdateInterval),
//
//    _autoUpdateTimer(other._autoUpdateTimer.getStartInterval(),
//                     other._autoUpdateTimer.getPeriodicInterval()),
    _calendarBuffer(other._calendarBuffer)
{
    if(other._pICalendar)
    {
        _pICalendar = icalcomponent_new_clone(other._pICalendar);
    }

    ofAddListener(ofEvents().update, this, &ICalendar::update);
}


ICalendar& ICalendar::operator = (ICalendar other)
{
    std::swap(_pICalendar, other._pICalendar);
    return *this;
}


ICalendar::~ICalendar()
{
    ofRemoveListener(ofEvents().update, this, &ICalendar::update);

    if (_pICalendar)
    {
        icalcomponent_free(_pICalendar);
        _pICalendar = 0;
    }
}


bool ICalendar::isLoaded() const
{
    return _pICalendar;
}


void ICalendar::setURI(const std::string& uri)
{
    try
    {
        _uri = Poco::URI(uri); // set the uri
    }
    catch (const Poco::SyntaxException& exc)
    {
        ofLogError("ICalendar::setURI") << exc.displayText();
    }
}


Poco::URI ICalendar::getURI() const
{
    return _uri;
}


void ICalendar::setAutoRefreshInterval(unsigned long long autoRefreshInterval)
{
    _autoUpdateInterval = autoRefreshInterval;

//    if (0 == _autoUpdateTimer.getPeriodicInterval())
//    {
//        _autoUpdateTimer.stop();
//        _autoUpdateTimer.start(Poco::TimerCallback<ICalendar>(*this, &ICalendar::onAutoUpdate));
//    }
//
//    _autoUpdateTimer.restart(autoRefreshInterval);
}


unsigned long long ICalendar::getAutoRefreshInterval() const
{
//    return _autoUpdateTimer.getPeriodicInterval();
    return _autoUpdateInterval;
}


bool ICalendar::parse(const ofBuffer& buffer)
{
    if (buffer.size() > 0)
    {
        icalcomponent* _pNewICalendar = icalcomponent_new_from_string(buffer.getData());

        if (_pNewICalendar)
        {
            std::swap(_pICalendar, _pNewICalendar);

            if (_pNewICalendar)
            {
                icalcomponent_free(_pNewICalendar); // free the old
            }

            return true;
        }
        else
        {
            ofLogError("ICalendar::parse()") << "Buffer could not be loaded.";
            return false;
        }
    }
    else
    {
        ofLogError("ICalendar::parse()") << "Buffer was empty.";
        return false;
    }
}


std::string ICalendar::getProductID() const
{
    if (_pICalendar)
    {
        icalproperty* pProperty = 0;
        const char* pProductId = 0;

        pProperty = icalcomponent_get_first_property(_pICalendar,
                                                     ICAL_PRODID_PROPERTY);
        if (pProperty)
        {
            pProductId = icalproperty_get_prodid(pProperty);

            if (pProductId)
            {
                return std::string(pProductId);
            }
            else
            {
                ofLogVerbose("ICalendar::getProductID()") << "No value for ICAL_PRODID_PROPERTY was found.";
                return "";
            }
        }
        else
        {
            return "";
        }
    }
    else
    {
        ofLogError("ICalendar::getSequence()") << "A calendar is not loaded.";
        return "";
    }
}


std::string ICalendar::getVersion() const
{
    if (_pICalendar)
    {
        icalproperty* pProperty = 0;
        const char* pVersion = 0;

        pProperty = icalcomponent_get_first_property(_pICalendar,
                                                     ICAL_VERSION_PROPERTY);
        if (pProperty)
        {
            pVersion = icalproperty_get_version(pProperty);

            if (pVersion)
            {
                return pVersion;
            }
            else
            {
                ofLogVerbose("ICalendar::getVersion()") << "No value for ICAL_VERSION_PROPERTY was found.";
                return "";
            }
        }
        else
        {
            return "";
        }
    }
    else
    {
        ofLogError("ICalendar::getVersion()") << "A calendar is not loaded.";
        return "";
    }

}


std::string ICalendar::getName() const
{
    if (_pICalendar)
    {
        std::string name = "";

        if (ICalendarUtils::getExtensionValue(_pICalendar,
                                              "X-WR-CALNAME",
                                              name))
        {
            return name;
        }
        else
        {
            ofLogVerbose("ICalendar::getName()") << "No value for X-WR-CALNAME was found.";
            return "";
        }
    }
    else
    {
        ofLogError("ICalendar::getName()") << "A calendar is not loaded.";
        return "";
    }
}


std::string ICalendar::getDescription() const
{
    if (_pICalendar)
    {
        std::string name = "";

        if (ICalendarUtils::getExtensionValue(_pICalendar,
                                              "X-WR-CALDESC",
                                              name))
        {
            return name;
        }
        else
        {
            ofLogVerbose("ICalendar::getDescription()") << "No value for X-WR-CALDESC was found.";
            return "";
        }
    }
    else
    {
        ofLogError("ICalendar::getDescription()") << "A calendar is not loaded.";
        return "";
    }
}


std::string ICalendar::getScale() const
{
    if (_pICalendar)
    {
        icalproperty* pProperty = 0;
        const char* pScale = 0;

        pProperty = icalcomponent_get_first_property(_pICalendar,
                                                     ICAL_CALSCALE_PROPERTY);
        if (pProperty)
        {
            pScale = icalproperty_get_calscale(pProperty);

            if (pScale)
            {
                return pScale;
            }
            else
            {
                return "";
            }
        }
        else
        {
            ofLogVerbose("ICalendar::getScale()") << "Scale not found";
            return "";
        }
    }
    else
    {
        ofLogError("ICalendar::getScale()") << "Calendar is not loaded.";
        return "";
    }
}


std::size_t ICalendar::getNumEvents() const
{
    if (_pICalendar)
    {
        return icalcomponent_count_components(_pICalendar,
                                              ICAL_VEVENT_COMPONENT);
    }
    else
    {
        ofLogError("ICalendar::getNumEvents()") << "Calendar is not loaded.";
        return 0;
    }
}


Poco::Timestamp ICalendar::getLastModified() const
{
    if (_pICalendar)
    {
        Poco::Timestamp lastModification(0);

        icalcomponent* pEventComponent = icalcomponent_get_first_component(_pICalendar,
                                                                           ICAL_ANY_COMPONENT);

        while (pEventComponent)
        {

            icalproperty* pProperty = icalcomponent_get_first_property(pEventComponent,
                                                                       ICAL_LASTMODIFIED_PROPERTY);

            if (pProperty)
            {
                Poco::Timestamp timestamp;

                if (ICalendarUtils::timeToTimestamp(icalproperty_get_lastmodified(pProperty),
                                                    timestamp))
                {
                    if (timestamp > lastModification)
                    {
                        lastModification = timestamp;
                    }
                }
            }

            pEventComponent = icalcomponent_get_next_component(_pICalendar,
                                                               ICAL_ANY_COMPONENT);
        }

        return lastModification;
    }
    else
    {
        ofLogError("ICalendar::getLastModified()") << "Calendar is not loaded.";
        return Poco::Timestamp(0);
    }
}



ICalendar::Events ICalendar::getEvents() const
{
    ICalendar::Events events;

    if (_pICalendar)
    {
        icalcomponent* pEventComponent = icalcomponent_get_first_component(_pICalendar,
                                                                           ICAL_VEVENT_COMPONENT);

        while (pEventComponent)
        {
            icalproperty* pProperty = icalcomponent_get_first_property(pEventComponent, ICAL_UID_PROPERTY);

            if (pProperty)
            {
                const char* pUID = icalproperty_get_uid(pProperty);

                if (pUID)
                {
                    events.push_back(ICalendarEvent((ICalendarInterface*)this, std::string(pUID)));
                }
                else
                {
                    ofLogWarning("ICalendar::getEvents()") << "UID string was malformed, skipping.";
                }
            }
            else
            {
                ofLogError("ICalendar::getEvents()") << "UID string was missing, skipping.";
            }

            pEventComponent = icalcomponent_get_next_component(_pICalendar,
                                                               ICAL_VEVENT_COMPONENT);
        }

        return events;
    }
    else
    {
        ofLogError("ICalendar::getEvents()") << "Calendar is not loaded.";
        return events;
    }
}


ICalendar::Events ICalendar::getEvents(const Interval& interval) const
{
    ICalendar::Events events = getEvents();
    ICalendar::Events remaining;
    ICalendar::Events::const_iterator iter = events.begin();

    while (iter != events.end())
    {
        ICalendarEvent event = *iter;

        if (event.hasInstances(interval))
        {
            remaining.push_back(event);
        }

        ++iter;
    }

    return remaining;
}


ICalendar::Events ICalendar::getEvents(const Poco::Timestamp& timestamp) const
{
    return getEvents(Interval(timestamp, timestamp));
}


ICalendar::EventInstances ICalendar::getEventInstances(const Interval& interval) const
{
    ICalendar::EventInstances instances;

    if (_pICalendar)
    {
        std::vector<Interval> intervals;
        std::vector<Interval>::iterator iter;

        icalcomponent* pEventComponent = 0;
        icalproperty* pProperty = 0;

        struct icaltimetype start = icaltime_from_timet(interval.getStart().epochTime(), false);
        struct icaltimetype end = icaltime_from_timet(interval.getEnd().epochTime(), false);

        pEventComponent = icalcomponent_get_first_component(_pICalendar,
                                                            ICAL_VEVENT_COMPONENT);

        while (pEventComponent)
        {
            pProperty = icalcomponent_get_first_property(pEventComponent,
                                                         ICAL_UID_PROPERTY);

            if (pProperty)
            {
                const char* pUID = icalproperty_get_uid(pProperty);

                if (pUID)
                {
                    std::string uid(pUID);

                    intervals.clear();

                    icalcomponent_foreach_recurrence(pEventComponent,
                                                     start,
                                                     end,
                                                     &ICalendarEvent::recurrencesCallback,
                                                     &intervals);
                    iter = intervals.begin();

                    while (iter != intervals.end())
                    {
                        instances.push_back(ICalendarEventInstance(ICalendarEvent(
                                                                                  (ICalendarInterface*)this,
                                                                                  uid),
                                                                   *iter));
                        ++iter;
                    }

                }
                else
                {
                    ofLogWarning("ICalendar::getEvents()") << "UID string was malformed, skipping.";
                }
            }
            else
            {
                ofLogError("ICalendar::getEventInstances()") << "UID string was missing, skipping.";
            }

            pEventComponent = icalcomponent_get_next_component(_pICalendar,
                                                               ICAL_VEVENT_COMPONENT);
        }

        return instances;
    }
    else
    {
        ofLogError("ICalendar::getEvents()") << "Calendar is not loaded.";
        return instances;
    }
}


ICalendar::EventInstances ICalendar::getEventInstances(const Poco::Timestamp& timestamp) const
{
    return getEventInstances(Interval(timestamp, timestamp));
}


icalcomponent* ICalendar::getComponent()
{
    return _pICalendar;
}


icalcomponent* ICalendar::getComponent() const
{
    return _pICalendar;
}


void ICalendar::update(ofEventArgs& args)
{
    ofScopedLock lock(_mutex);

    if (_calendarBuffer.size() > 0)
    {
        parse(_calendarBuffer);
        _calendarBuffer.clear();
    }
}


//void ICalendar::onAutoUpdate(Poco::Timer& timer)
//{
//    if (!_uri.empty())
//    {
//        ofBuffer buffer;
//
//        if (loadURI(_uri, buffer))
//        {
//            ofScopedLock lock(_mutex);
//            _calendarBuffer = buffer; // lock while we set the buffer
//        }
//    }
//}


bool ICalendar::loadURI(const Poco::URI& uri, ofBuffer& buffer)
{
    if (_uri.getScheme() == "http" || _uri.getScheme() == "https")
    {
        ofHttpResponse response = ofLoadURL(_uri.toString());

        if (200 == response.status)
        {
            buffer = response.data;
            return true;
        }
        else
        {
            ofLogError("ICalendar::loadURI()") << "URI: " << uri.toString() << ": " << response.error;
            return false;
        }
    }
    else if(_uri.getScheme() == "file" || _uri.getScheme().empty())
    {
        Poco::File file(ofToDataPath(uri.getPath(), true));

        if(file.exists())
        {
            buffer = ofBufferFromFile(file.path());
            return true;
        }
        else
        {
            ofLogError("ICalendar::loadURI()") << "File: " << file.path() << " not found.";
            return false;
        }
    }
    else
    {
        ofLogError("ICalendar::loadURI()") << "Unknown URI Scheme: " << _uri.getScheme();
        return false;
    }
}



} } // namespace ofx::Time
