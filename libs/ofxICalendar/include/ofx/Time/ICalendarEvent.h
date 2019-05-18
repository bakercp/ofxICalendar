//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


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


/// \brief The ICalendarEvent class stores a VEVENT.
///
/// The ICalendarEvent class stores a VEVENT object and provides access to event
/// information including recurrances.
class ICalendarEvent
{
public:
    /// \brief A typdef for a collection of instances.
    typedef std::vector<Interval> Instances;

    /// \brief Destroys the ICalendarEvent.
    virtual ~ICalendarEvent();

    /// \brief Get the event's description.
    /// \returns the event's description iff the DESCRIPTION tag exists,
    /// otherwise returns an empty empty std::string.
    std::string getDescription() const;

    /// \brief Get the event's summary.
    /// \returns the event's summary iff the SUMMARY tag exists,
    /// otherwise returns an empty empty std::string.
    std::string getSummary() const;

    /// \brief Get the event's unique id.
    /// \returns the event's unique id iff the UID tag exists,
    /// otherwise returns an empty empty std::string.
    std::string getUID() const;

    /// \brief Get the event's location.
    /// \returns the event's location iff the LOCATION tag exists,
    /// otherwise returns an empty empty std::string.
    std::string getLocation() const;

    /// \brief Get the event's sequence.
    /// \returns the event's sequence number iff the SEQUENCE tag exists,
    /// otherwise returns a -1.
    int getSequence() const;

    /// \brief Get the event's status.
    /// \returns the event's status iff the STATUS tag exists,
    /// otherwise returns an empty empty std::string.
    std::string getStatus() const;

    /// \brief Get the event's name.
    /// \returns the event's status iff the NAME tag exists,
    /// otherwise returns an empty empty std::string.
    std::string getName() const;

    /// \brief Get the event's last modified event.
    /// \returns the event's last modified time iff the LAST-MODIFIED tag
    /// exists, otherwise returns Poco::Timestamp(0).
    Poco::Timestamp getLastModified() const;

    /// \brief Get the event's timestamp.
    /// \returns the event's created time iff the DTSTAMP tag exists,
    /// otherwise returns Poco::Timestamp(0).
    Poco::Timestamp getTimestamp() const;

    /// \brief Get the event's created time.
    /// \returns the event's timestamp iff the CREATED tag exists,
    /// otherwise returns Poco::Timestamp(0).
    Poco::Timestamp getCreated() const;

    /// \brief Get the event's organizer.
    /// \returns the event's organizer iff the ORGANIZER tag exists,
    /// otherwise returns an empty empty std::string.
    std::string getOrganizer() const;

    /// \brief Get the event's owner.
    /// \returns the event's location iff the OWNER tag exists,
    /// otherwise returns an empty empty std::string.
    std::string getOwner() const;

    /// \brief Get the event's start time.
    /// \returns the event's start time iff the DTSTART tag exists,
    /// otherwise returns Poco::Timestamp(0).
    Poco::Timestamp getStart() const;

    /// \brief Get the event's end time.
    /// \returns the event's start time iff the DTEND tag exists,
    /// otherwise returns Poco::Timestamp(0).
    Poco::Timestamp getEnd() const;

    // \brief Query if the event will repeat indefinitely.
    // \returns true iff the event will not repeat indefinitely.
    // Unbounded events are events with recurrences that do not terminate
    // with a COUNT or UNTIL property.
    // bool isBounded() const;

    /// \brief Query if the component is valid.
    /// \returns true iff a valid icalcomponent* is allocated AND
    /// if that icalcomponent has a VEVENT with the this event's UID.
    bool isValid() const;

    /// \returns true iff this event includes the interval.
    bool isValidInterval(const Interval& interval) const;

    /// \returns true iff this event has any instances (including the
    /// the primary start / end times) that overlap with the given Interval.
    bool hasInstances(const Interval& interval) const;

    /// \returns true iff this event has any instances (including the
    /// the primary start / end times) that contain the given timestamp.
    bool hasInstances(const Poco::Timestamp& timestamp) const;

    /// \returns a collection of instances (including the
    /// the primary start / end times) that overlap with the given Interval.
    Instances getInstances(const Interval& Interval) const;

    /// \returns a collection of instances (including the
    /// the primary start / end times) that contain the given timestamp.
    Instances getInstances(const Poco::Timestamp& timestamp) const;

    /// \returns true iff the UID fields are not empty and are identical.
    bool operator == (const ICalendarEvent& event) const;

    /// \returns true iff the UID fields are empty or not identical.
    bool operator != (const ICalendarEvent& event) const;

    /// \returns true iff this event's start time
    /// is greater than the given event's start time.
	bool operator >  (const ICalendarEvent& event) const;

    /// \returns true iff this event's start time is greater
    /// than or equal to the given event's start time.
	bool operator >= (const ICalendarEvent& event) const;

    /// \returns true iff this event's start time
    /// is less than the given event's start time.
    bool operator <  (const ICalendarEvent& event) const;

    /// \returns true iff this event's start time is less
    /// than or equal to the given event's start time.
    bool operator <= (const ICalendarEvent& event) const;

    /// Passes the internal icalcomponent text to the output stream.
    /// (e.g. std::cout << myEvent << std::endl will dump the VEVENT
    /// element to the standard output as a RFC 2445 icalendar string).
	friend std::ostream& operator << (std::ostream& os, const ICalendarEvent& event);

    /// \brief Get the event component for a given event uid.
    ///
    /// Returned component is not cloned.  The parent retains ownership.
    ///
    /// \returns 0 iff a VEVENT in the parent with the given UID is NOT found or
    /// a pointer to the VEVENT icalcomponent within the parent if found.
    static icalcomponent* getEventComponentForUID(icalcomponent* parent,
                                                  const std::string& uid);


private:
    /// \brief Constructs an ICalendarEvent class.
    ///
    /// An ICalendar Event holds a shared pointer to the parent
    /// calendar and a unique event identification.
    ///
    /// \param pParent A pointer to the parent ICalendar store.
    /// \param uid The uid of this event.
    ICalendarEvent(const ICalendarInterface* pParent, std::string uid);

    /// \brief The ICalendar store.
    const ICalendarInterface* _pParent = nullptr;

    /// \brief The event's uid
    std::string _uid;

    /// \brief Get this event's icalcomponent.
    /// \returns a pointer to this event's icalcomponent.
    icalcomponent* getEventComponent() const;

    /// \brief A helper method for getting properties as strings.
    /// \param kind The icalproperty_kind that the user is seeking.
    /// \returns the string representing the property value.
    std::string getProperty(icalproperty_kind kind) const;

    /// \param component the internal icalcomponent returned with the callback.
    /// \param timeSpan the icaltime_span returned with the callback.
    /// \param data the user data returned with the callback.
    /// \return The internal callback for querying recurrance information.
    static void recurrencesCallback(icalcomponent* component,
                                    struct icaltime_span* timeSpan,
                                    void* data);

    // The Calendar class is allowed to create Event
    // classes using the private Event constructor.
    friend class ICalendar;

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
