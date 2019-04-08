//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofx/Time/ICalendarEvent.h"


namespace ofx {
namespace Time {


ICalendarEvent::ICalendarEvent(ICalendarInterface* pParent,
                               std::string uid):
    _pParent(pParent),
    _uid(uid)
{
}


ICalendarEvent::~ICalendarEvent()
{
}


std::string ICalendarEvent::getDescription() const
{
    return getProperty(ICAL_DESCRIPTION_PROPERTY);
}


std::string ICalendarEvent::getSummary() const
{
    return getProperty(ICAL_SUMMARY_PROPERTY);
}


std::string ICalendarEvent::getUID() const
{
    return _uid;
}


std::string ICalendarEvent::getLocation() const
{
    return getProperty(ICAL_LOCATION_PROPERTY);
}


int ICalendarEvent::getSequence() const
{
    icalcomponent* pEventComponent = getEventComponent();

    if (pEventComponent)
    {
        icalproperty* pProperty = icalcomponent_get_first_property(pEventComponent,
                                                                   ICAL_SEQUENCE_PROPERTY);
        if (pProperty)
        {
            return icalproperty_get_sequence(pProperty);
        }
        else
        {
            ofLogVerbose("Event::getSequence()") << "No sequence property found, returning -1.";
            return -1;
        }
    }
    else
    {
        ofLogError("Event::getSequence()") << "The icalcomponent is not loaded.";
        return -1;
    }
}


std::string ICalendarEvent::getStatus() const
{
    return getProperty(ICAL_STATUS_PROPERTY);
}


std::string ICalendarEvent::getName() const
{
    return getProperty(ICAL_NAME_PROPERTY);
}


Poco::Timestamp ICalendarEvent::getLastModified() const
{
    icalcomponent* pEventComponent = getEventComponent();

    if (pEventComponent)
    {
        icalproperty* pProperty = icalcomponent_get_first_property(pEventComponent,
                                                                   ICAL_LASTMODIFIED_PROPERTY);

        if (pProperty)
        {
            Poco::Timestamp timestamp;

            if (ICalendarUtils::timeToTimestamp(icalproperty_get_lastmodified(pProperty),
                                                timestamp))
            {
                return timestamp;
            }
            else
            {
                ofLogVerbose("Event::getLastModified()") << "Invalid timestamp.";
                return Poco::Timestamp(0);
            }
        }
        else
        {
            ofLogVerbose("Event::getLastModified()") << "The icalproperty is 0";
            return Poco::Timestamp(0);
        }
    }
    else
    {
        ofLogError("Event::getLastModified()") << "The icalcomponent is not loaded.";
        return Poco::Timestamp(0);
    }
}

    
Poco::Timestamp ICalendarEvent::getTimestamp() const
{
    icalcomponent* pEventComponent = getEventComponent();

    if (pEventComponent)
    {
        Poco::Timestamp timestamp;

        if (ICalendarUtils::timeToTimestamp(icalcomponent_get_dtstamp(pEventComponent),
                                            timestamp))
        {
            return timestamp;
        }
        else
        {
            ofLogVerbose("Event::getTimestamp()") << "Invalid timestamp.";
            return Poco::Timestamp(0);
        }
    }
    else
    {
        ofLogError("Event::getTimestamp()") << "The icalcomponent is not loaded.";
        return Poco::Timestamp(0);
    }
}


Poco::Timestamp ICalendarEvent::getCreated() const
{
    icalcomponent* pEventComponent = getEventComponent();

    if (pEventComponent)
    {
        icalproperty* pProperty = icalcomponent_get_first_property(pEventComponent,
                                                                   ICAL_CREATED_PROPERTY);

        if (pProperty)
        {
            Poco::Timestamp timestamp;

            if (ICalendarUtils::timeToTimestamp(icalproperty_get_created(pProperty),
                                                timestamp))
            {
                return timestamp;
            }
            else
            {
                ofLogVerbose("Event::getCreated()") << "Invalid timestamp.";
                return Poco::Timestamp(0);
            }
        }
        else
        {
            ofLogVerbose("Event::getCreated()") << "The icalproperty is 0";
            return Poco::Timestamp(0);
        }
    }
    else
    {
        ofLogError("Event::getCreated()") << "The icalcomponent is not loaded.";
        return Poco::Timestamp(0);
    }
}


std::string ICalendarEvent::getOrganizer() const
{
    return getProperty(ICAL_ORGANIZER_PROPERTY);
}


std::string ICalendarEvent::getOwner() const
{
    return getProperty(ICAL_OWNER_PROPERTY);
}


Poco::Timestamp ICalendarEvent::getStart() const
{
    icalcomponent* pEventComponent = getEventComponent();

    if (pEventComponent)
    {
        Poco::Timestamp timestamp;

        if (ICalendarUtils::timeToTimestamp(icalcomponent_get_dtstart(pEventComponent),
                                            timestamp))
        {
            return timestamp;
        }
        else
        {
            ofLogVerbose("Event::getStart()") << "Invalid timestamp.";
            return Poco::Timestamp(0);
        }
    }
    else
    {
        ofLogError("Event::getStart()") << "The icalcomponent is not loaded.";
        return Poco::Timestamp(0);
    }
}


Poco::Timestamp ICalendarEvent::getEnd() const
{
    icalcomponent* pEventComponent = getEventComponent();

    if (pEventComponent)
    {
        Poco::Timestamp timestamp;

        if (ICalendarUtils::timeToTimestamp(icalcomponent_get_dtend(pEventComponent),
                                            timestamp))
        {
            return timestamp;
        }
        else
        {
            ofLogVerbose("Event::getEnd()") << "Invalid timestamp.";
            return Poco::Timestamp(0);
        }
    }
    else
    {
        ofLogError("Event::getEnd()") << "The icalcomponent is not loaded.";
        return Poco::Timestamp(0);
    }
}

//icaltimezone* ICalendarEvent::getTimeFor(icalproperty_kind property, const struct icaltimetype t)
//{
//    if (!icaltime_is_utc(time))
//    {
//
//        cout << "----XXXXXXX-----NOT UTC" << endl;
//
//        icalproperty* pProperty = icalcomponent_get_first_property(_pComponent,
//                                                                   ICAL_DTSTART_PROPERTY);
//
//        if (0 != pProperty)
//        {
//            icalparameter* param = icalproperty_get_first_parameter(pProperty,
//                                                                    ICAL_TZID_PARAMETER);
//
//            if (0 != param)
//            {
//                const char* tzid = ;
//
//                if (0 != tzid)
//                {
//                    if (0 != _pParentComponent)
//                    {
//                        cout << "C" << endl;
//
//                        // timezone can be 0
//                        icaltimezone* timezone = icalcomponent_get_timezone(_pParentComponent,
//                                                                            icalparameter_get_tzid(param));
//
//                        cout << "AFTER: " << Utils::format(Poco::Timestamp::fromEpochTime(icaltime_as_timet_with_zone(time, timezone))) << endl;
//                        //                            else
//                        //                            {
//                        //                                cout << "TIMEZONE WAS NOT FOUND" << endl;
//                        //                            }
//
//                    }
//                    else
//                    {
//                        cout << "PARENT WAS UNDEFINED" << endl;
//                    }
//
//
//                }
//                else
//                {
//                    cout << "TZID was null" << endl;
//
//                }
//            }
//            else
//            {
//                cout << "param was NULL" << endl;
//            }
//        }
//        else
//        {
//            cout << "0 == pProperty" << endl;
//        }
//    }
//    
//    cout << "///////" << endl;
//}


//bool ICalendarEvent::isBounded() const
//{
//    if (0 != _pComponent)
//    {
//        //	rrule = icalcomponent_get_first_property(comp,ICAL_RRULE_PROPERTY);
//        //	recur = icalproperty_get_rrule(rrule);
//        //	start = icalproperty_get_dtstart(dtstart);
//
//
//        icalproperty* pProperty = icalcomponent_get_first_property(_pComponent,
//                                                                   ICAL_RRULE_PROPERTY);
//
//        if (0 != pProperty)
//        {
//
//            struct icalrecurrencetype recurance = icalproperty_get_rrule(pProperty);
//
//            cout << "FREQ=" << recurance.freq << endl;
//
//            cout << "COUNT=" << recurance.count << endl;
//
//            if (!icaltime_is_null_time(recurance.until))
//            {
//                if (icaltime_is_valid_time(recurance.until))
//                {
//                    cout << "UNTIL was NOT NULL AND VALID" << endl;
//                }
//                else
//                {
//                    cout << "UNTIL was NOT NULL AND NOT VALID" << endl;
//                }
//            }
//            else
//            {
//                cout << "UNTIL was NULL" << endl;
//            }
//
//
////            cout << "COUNT=" << Utils::timeToTimestamp(recurance.until) << endl;
//
//            char* pString = icalrecurrencetype_as_string(&recurance);
//
//            if(0 != pString)
//            {
//                cout << pString << endl;
//            }
//            
//
//        }
//        else
//        {
//            ofLogVerbose("Event::isBounded()") << "The icalproperty is 0";
//            return true;
//        }
//
//    }
//    else
//    {
//        ofLogError("Event::isBounded()") << "The icalcomponent is not loaded.";
//        return true;
//    }
//
//
//
////    icalproperty rrule;
////    struct icalrecurrencetype recur;
////    struct icaltimetype dtstart;
////
////	rrule = icalcomponent_get_first_property(comp,ICAL_RRULE_PROPERTY);
////	recur = icalproperty_get_rrule(rrule);
////	start = icalproperty_get_dtstart(dtstart);
////    @endcode
////
////    Or, just make them up:
////
////    @code
////    recur = icalrecurrencetype_from_string("FREQ=YEARLY;BYDAY=SU,WE");
////    dtstart = icaltime_from_string("19970101T123000")
////    @endcode
////
////    2) Create an iterator
////
////    @code
////    icalrecur_iterator* ritr;
////    ritr = icalrecur_iterator_new(recur,start);
////    @endcode
////
////    3) Iterator over the occurrences
////
////    @code
////    struct icaltimetype next;
////    while (next = icalrecur_iterator_next(ritr)
////           && !icaltime_is_null_time(next){
////               Do something with next
////           }
////           @endcode
////
//
//
//
//    // has rrule?
//    // do rrules have count or until?
//    return true;
//}


bool ICalendarEvent::hasInstances(const Interval& interval) const
{
    // TODO: improve efficiency
    return !getInstances(interval).empty();
}


bool ICalendarEvent::hasInstances(const Poco::Timestamp& timestamp) const
{
    // TODO: improve efficiency
    return !getInstances(timestamp).empty();
}


ICalendarEvent::Instances ICalendarEvent::getInstances(const Interval& interval) const
{
    Instances instances;

    icalcomponent* pEventComponent = getEventComponent();

    if (pEventComponent)
    {
        std::vector<Interval> intervals;

        struct icaltimetype start = icaltime_from_timet(interval.getStart().epochTime(), false);
        struct icaltimetype end = icaltime_from_timet(interval.getEnd().epochTime(), false);

        icalcomponent_foreach_recurrence(pEventComponent,
                                         start,
                                         end,
                                         &ICalendarEvent::recurrencesCallback,
                                         &instances);
    }
    else
    {
        ofLogError("Event::getInstances()") << "The icalcomponent is not loaded.";
    }

    return instances;
}


ICalendarEvent::Instances ICalendarEvent::getInstances(const Poco::Timestamp& timestamp) const
{
    return getInstances(Interval(timestamp, timestamp));
}


bool ICalendarEvent::operator == (const ICalendarEvent& event) const
{
    std::string thisUID = getUID();
    std::string thatUID = event.getUID();

    return  !thisUID.empty() &&
            !thatUID.empty() &&
             thisUID == thatUID;
}


bool ICalendarEvent::operator != (const ICalendarEvent& event) const
{
    return !(*this == event);
}


bool ICalendarEvent::operator >  (const ICalendarEvent& event) const
{
    return getUID() > event.getUID();
}


bool ICalendarEvent::operator >= (const ICalendarEvent& event) const
{
    return getUID() >= event.getUID();
}


bool ICalendarEvent::operator <  (const ICalendarEvent& event) const
{
    return getUID() < event.getUID();
}


bool ICalendarEvent::operator <= (const ICalendarEvent& event) const
{
    return getUID() <= event.getUID();
}

std::string ICalendarEvent::getProperty(icalproperty_kind kind) const
{
    icalcomponent* pEventComponent = getEventComponent();

    if (pEventComponent)
    {
        icalproperty* pProperty = icalcomponent_get_first_property(pEventComponent, kind);

        if (pProperty)
        {
            const char* pPropertyString = 0;

            switch (kind)
            {
                case ICAL_DESCRIPTION_PROPERTY:
                    pPropertyString = icalproperty_get_description(pProperty);
                    break;
                case ICAL_SUMMARY_PROPERTY:
                    pPropertyString = icalproperty_get_summary(pProperty);
                    break;
                case ICAL_UID_PROPERTY:
                    pPropertyString = icalproperty_get_uid(pProperty);
                    break;
                case ICAL_LOCATION_PROPERTY:
                    pPropertyString = icalproperty_get_location(pProperty);
                    break;
                case ICAL_ORGANIZER_PROPERTY:
                    pPropertyString = icalproperty_get_organizer(pProperty);
                    break;
                case ICAL_OWNER_PROPERTY:
                    pPropertyString = icalproperty_get_owner(pProperty);
                    break;
                case ICAL_NAME_PROPERTY:
                    pPropertyString = icalproperty_get_name(pProperty);
                    break;
                case ICAL_STATUS_PROPERTY:
                    pPropertyString = icalproperty_status_to_string(icalproperty_get_status(pProperty));
                    break;
                default:
                    ofLogError("Event::getProperty()") << "Unsupported property type: " << kind;
                    return "";
            }

            if (pPropertyString)
            {
                return pPropertyString;
            }
            else
            {
                ofLogVerbose("Event::getProperty()") << "Property string was 0.";
                return "";
            }
        }
        else
        {
            ofLogError("Event::getProperty()") << "No property of type: " << kind << " was found.";
            return "";
        }
    }
    else
    {
        ofLogError("Event::getProperty()") << "The icalcomponent is not loaded.";
        return "";
    }
}


icalcomponent* ICalendarEvent::getEventComponentForUID(icalcomponent* pParentComponent,
                                                       const std::string& uid)
{
    if (pParentComponent)
    {
        icalcomponent* pEventComponent = icalcomponent_get_first_component(pParentComponent,
                                                                           ICAL_VEVENT_COMPONENT);

        while (pEventComponent)
        {
            icalproperty* pUIDProperty = icalcomponent_get_first_property(pEventComponent,
                                                                          ICAL_UID_PROPERTY);

            if (pUIDProperty)
            {
                const char* pUID = icalproperty_get_uid(pUIDProperty);

                if (pUID)
                {
                    if(uid == std::string(pUID))
                    {
                        return pEventComponent;
                    }
                    else
                    {
                        // didn't match
                    }
                }
                else
                {
                    ofLogError("ICalendar::getEventComponentForUID()") << "Event had invalid UID.";
                }
            }
            else
            {
                ofLogError("ICalendar::getEventComponentForUID()") << "Event did not have UID.";
            }

            pEventComponent = icalcomponent_get_next_component(pParentComponent,
                                                               ICAL_VEVENT_COMPONENT);
        }

        // No event with the given UID was found.
        return 0;
    }
    else
    {
        ofLogError("ICalendar::getEventComponentForUID()") << "Parent is not valid.";
        return 0;
    }
}


bool ICalendarEvent::isValid() const
{
    return getEventComponentForUID(_pParent->getComponent(), _uid);
}


bool ICalendarEvent::isValidInterval(const Interval& interval) const
{
    ICalendarEvent::Instances instances = getInstances(interval);

    ICalendarEvent::Instances::iterator iter = instances.begin();

    while (iter != instances.end())
    {
        if (interval == *iter) return true;

        ++iter;
    }

    return false;
}


icalcomponent* ICalendarEvent::getEventComponent() const
{
    return getEventComponentForUID(_pParent->getComponent(), _uid);
}


void ICalendarEvent::recurrencesCallback(icalcomponent* component,
                                         struct icaltime_span* timeSpan,
                                         void* data)
{
    if (data)
    {
        std::vector<Interval>* intervals = reinterpret_cast<std::vector<Interval>*>(data);

        intervals->push_back(Interval(Poco::Timestamp::fromEpochTime(timeSpan->start),
                                      Poco::Timestamp::fromEpochTime(timeSpan->end)));
    }
    else
    {
        ofLogError("Event::recurrancesCallback()") << "Instances vector is == 0";
    }
}


} } // namespace ofx::Time
