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


#include <string>
#include <vector>
#include <libical/ical.h>
#include "Poco/Timestamp.h"
#include "ofx/Time/ICalendarInterface.h"
#include "ofx/Time/ICalendarUtils.h"
#include "ofx/Time/Interval.h"
#include "ofx/Time/Utils.h"
#include "ofLog.h"


namespace ofx {
namespace Time {


class ICalendarEvent
    // The ICalendarEvent class stores a VEVENT and provides
    // access to event information including recurrances.
{
public:
    typedef std::vector<Interval> Instances;
        ///< A typdef for a collection of instances.

    virtual ~ICalendarEvent();
        ///< Frees the ICalendarEvent.

    std::string getDescription() const;
        ///< Returns the event's description iff the DESCRIPTION tag exists,
        ///< otherwise returns an empty empty std::string.

    std::string getSummary() const;
        ///< Returns the event's summary iff the SUMMARY tag exists,
        ///< otherwise returns an empty empty std::string.

    std::string getUID() const;
        ///< Returns the event's unique id iff the UID tag exists,
        ///< otherwise returns an empty empty std::string.

    std::string getLocation() const;
        ///< Returns the event's location iff the LOCATION tag exists,
        ///< otherwise returns an empty empty std::string.

    int getSequence() const;
        ///< Returns the event's sequence number iff the SEQUENCE tag exists,
        ///< otherwise returns a -1.

    std::string getStatus() const;
        ///< Returns the event's status iff the STATUS tag exists,
        ///< otherwise returns an empty empty std::string.

    std::string getName() const;
        ///< Returns the event's status iff the STATUS tag exists,
        ///< otherwise returns an empty empty std::string.

    Poco::Timestamp getLastModified() const;
        ///< Returns the event's last modified time iff the LAST-MODIFIED tag
        ///< exists, otherwise returns Poco::Timestamp(0).

    Poco::Timestamp getTimestamp() const;
        ///< Returns the event's created time iff the DTSTAMP tag exists,
        ///< otherwise returns Poco::Timestamp(0).

    Poco::Timestamp getCreated() const;
        ///< Returns the event's timestamp iff the CREATED tag exists,
        ///< otherwise returns Poco::Timestamp(0).

    std::string getOrganizer() const;
        ///< Returns the event's organizer iff the ORGANIZER tag exists,
        ///< otherwise returns an empty empty std::string.

    std::string getOwner() const;
        ///< Returns the event's location iff the OWNER tag exists,
        ///< otherwise returns an empty empty std::string.

    Poco::Timestamp getStart() const;
        ///< Returns the event's start time iff the DTSTART tag exists,
        ///< otherwise returns Poco::Timestamp(0).

    Poco::Timestamp getEnd() const;
        ///< Returns the event's start time iff the DTEND tag exists,
        ///< otherwise returns Poco::Timestamp(0).

//    bool isBounded() const;
//        ///< Returns true iff the event will not repeat indefinitely.
//        ///< Unbounded events are events with recurrences that do not terminate
//        ///< with a COUNT or UNTIL property.

    bool isValid() const;
        ///< Returns true iff a valid icalcomponent* is allocated AND
        ///< if that icalcomponent has a VEVENT with the this event's UID.

    bool isValidInterval(const Interval& interval) const;
        ///< Returns true iff this event includes the interval.

    bool hasInstances(const Interval& interval) const;
        ///< Returns true iff this event has any instances (including the
        ///< the primary start / end times) that overlap with the given Interval.

    bool hasInstances(const Poco::Timestamp& timestamp) const;
        ///< Returns true iff this event has any instances (including the
        ///< the primary start / end times) that contain the given timestamp.

    Instances getInstances(const Interval& Interval) const;
        ///< Returns a collection of instances (including the
        ///< the primary start / end times) that overlap with the given Interval.

    Instances getInstances(const Poco::Timestamp& timestamp) const;
        ///< Returns a collection of instances (including the
        ///< the primary start / end times) that contain the given timestamp.

    bool operator == (const ICalendarEvent& event) const;
        ///< Returns true iff the UID fields are not empty and are identical.

    bool operator != (const ICalendarEvent& event) const;
        ///< Returns true iff the UID fields are empty or not identical.

	bool operator >  (const ICalendarEvent& event) const;
        ///< Returns true iff this event's start time
        ///< is greater than the given event's start time.

	bool operator >= (const ICalendarEvent& event) const;
        ///< Returns true iff this event's start time is greater
        ///< than or equal to the given event's start time.

    bool operator <  (const ICalendarEvent& event) const;
        ///< Returns true iff this event's start time
        ///< is less than the given event's start time.

    bool operator <= (const ICalendarEvent& event) const;
        ///< Returns true iff this event's start time is less
        ///< than or equal to the given event's start time.

	friend std::ostream& operator << (std::ostream& os, const ICalendarEvent& event);
        ///< Passes the internal icalcomponent text to the output stream.
        ///< (e.g. std::cout << myEvent << std::endl will dump the VEVENT
        ///< element to the standard output as a RFC 2445 icalendar string).

    static icalcomponent* getEventComponentForUID(icalcomponent* parent, const std::string& uid);
        ///< Returns 0 iff a VEVENT in the parent with the given UID is NOT found.
        ///< Returns pointer to the VEVENT icalcomponent within the parent if found.
        ///< Returned component is not cloned.  Given parent retains ownership.


private:
    ICalendarEvent(ICalendarInterface* pParent, std::string uid);
        ///< Constructs an ICalendarEvent class.
        ///< An ICalendar Event holds a shared pointer to the parent
        ///< calendar and a unique event identification.

    ICalendarInterface* _pParent;
        ///< The ICalendar Store

    std::string _uid;
        ///< Event uid;

    icalcomponent* getEventComponent() const;
        ///< A method to return a pointer to this event's component.

    std::string getProperty(icalproperty_kind kind) const;
        ///< A helper method for getting certain string properties.

    static void recurrencesCallback(icalcomponent* component,
                                    struct icaltime_span* timeSpan,
                                    void* data);
        ///< The internal callback for querying recurrance information.

    friend class ICalendar;
        ///< The Calendar class is allowed to create Event
        ///< classes using the private Event constructor.

};


inline std::ostream& operator << (std::ostream& os, const ICalendarEvent& event)
{
    icalcomponent* evt = event.getEventComponent();

    if (evt)
    {
        char* pString = icalcomponent_as_ical_string(evt);
        os.write(pString, std::strlen(pString));
    }

    return os;
}


} } // namespace ofx::Time
