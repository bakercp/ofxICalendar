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
#include <cstring>
#include <map>
#include <libical/ical.h>
#include "Poco/File.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Timezone.h"
#include "Poco/Timer.h"
#include "Poco/URI.h"
#include "ofx/Time/ICalendarInterface.h"
#include "ofx/Time/ICalendarEvent.h"
#include "ofx/Time/ICalendarEventInstance.h"
#include "ofx/Time/Interval.h"
#include "ofUtils.h"
#include "ofURLFileLoader.h"
#include "ofFileUtils.h"
#include "ofEvents.h"


namespace ofx {
namespace Time {

    
class ICalendar: public ICalendarInterface
    // The ICalendar class implements the RFC 2445 icalendar format.
{
public:
    typedef std::shared_ptr<ICalendar> SharedPtr;
        ///< A shared pointer typedef.

    typedef std::weak_ptr<ICalendar> WeakPtr;
        ///< A weak pointer typedef.

    typedef std::vector<ICalendarEvent> Events;
        ///< A collection of events.

    typedef std::vector<ICalendarEventInstance> EventInstances;
        ///< A collection of events.

    ICalendar(const std::string& uri, unsigned long long autoRefreshInterval = 0);
        ///< Creates a calendar with the given uri.
        ///< Update interval specifies the auto refresh

    ICalendar(const ICalendar& other);
        ///< Copy constructor.

    ICalendar& operator = (ICalendar other);
        ///< Assignment.

    virtual ~ICalendar();
        ///< Frees the internal icalcomponent* if it was allocated.

    bool isLoaded() const;
        ///< Returns true iff a valid icalcomponent* is allocated.
        ///< When the calendar is created with a default constructor or when a
        ///< load or parse operation has failed, this method will return false.

    void setURI(const std::string& uri);
        ///< Sets the URI for the ICalendar file / url (e.g. *.ics) path.
        ///< The uri can be specify a remote location
        ///< (e.g. https://example.com/cal.ics) or can be specified as a
        ///< local file (e.g. basic.ics).  Local files without an absolute
        ///< path will be located relative to the `bin/data` directory.
        ///< The file must conform to the RFC 2445 specification.

    Poco::URI getURI() const;
        ///< Returns the URI for the ICalendar file.
        ///< If none is set, the returned Poco::URI will be empty.

    void setAutoRefreshInterval(unsigned long long autoRefreshInterval);
        ///< Set the auto update interval.
        ///< Auto updates will attempt to reload the last URI loaded.
        ///< If no URI has been loaded, no auto updates will be attempted.
        ///< Auto update is disabled if the interval is set to 0.

    unsigned long long getAutoRefreshInterval() const;
        ///< Returns the number of milliseconds in the refresh interval.
        ///< Returns 0 if auto refresh is disabled.

    bool parse(const ofBuffer& buffer);
        ///< Loads data from a text buffer containing an icalendar file.
        ///< The buffered data must conform to the RFC 2445 specification.
        ///< If the user is manually parsing a buffer (as opposed to using
        ///< a URI and auto refresh interval, the user must be certain to
        ///< maintain thread safety.  Additionally, the user must disable
        ///< auto refresh by setting the URI to an empty string and / or
        ///< setting the auto update interval to 0.

    std::string getProductID() const;
        ///< Returns the calendar's product id
        ///< (e.g. -//Google Inc//Google Calendar 70.9054//EN)
        ///< or an empty std::string if no PRODID field exists.

    std::string getVersion() const;
        ///< Returns the calendar's icalendar version number.

    std::string getName() const;
        ///< Returns the name of the calendar by examining the X-WR-CALNAME
        ///< field or an empty std::string if no X-WR-CALNAME field exists.
        ///< This extended field is not part of the RFC 2445
        ///< specification, but used by Google Calendar.

    std::string getDescription() const;
        ///< Returns the name of the calendar by examining the X-WR-CALDESC
        ///< field or an empty std::string if no X-WR-CALDESC field exists.
        ///< This extended field is not part of the RFC 2445
        ///< specification, but used by Google Calendar.

    std::string getScale() const;
        ///< Returns the value of the CALSCALE field or an empty std::string
        ///< if no CALSCALE tag exists.

    std::size_t getNumEvents() const;
        ///< Returns the number of VEVENT elements in the calendar.

    Poco::Timestamp getLastModified() const;
        ///< Returns the most recent LAST-MODIFIED tag from all events.
        ///< If no LAST-MODIFIED tag exists, returns Poco::Timestamp(0).

//    bool getEventByUID(const std::string& uid, ICalendarEvent& event) const;
//        ///< Returns true iff an event with a UID matching uid is found.  If true
//        ///< the first matching event will be assigned to the event argument.
//        ///< Returns false if a matching event is not found.

    Events getEvents() const;
        ///< Returns all calendar events.

    Events getEvents(const Interval& interval) const;
        ///< Returns all events that have some overlap with the given range.
        ///< All event recurrences are checked for overlap.

    Events getEvents(const Poco::Timestamp& timestamp) const;
        ///< Returns all events that contain the given timestamp.
        ///< All event recurrences are checked for overlap.

    EventInstances getEventInstances(const Interval& interval) const;
        ///< Returns all event instances that have some
        ///< overlap with the given range.

    EventInstances getEventInstances(const Poco::Timestamp& timestamp) const;
        ///< Returns all event instances that contain
        ///< the given timestamp.

    icalcomponent* getComponent();
        ///< Returns the underlying libicalcomponent.

    icalcomponent* getComponent() const;
        ///< Returns the underlying libicalcomponent.

	friend std::ostream& operator << (std::ostream& os, const ICalendar& vec);
        ///< Passes the internal icalcomponent text to the output stream.
        ///< (e.g. std::cout << myCalendar << std::endl will dump the
        ///< calendar to the standard output as a RFC 2445 icalendar string).

    static SharedPtr makeShared(const std::string& uri,
                                unsigned long long autoRefreshInterval = 0)
    {
        return SharedPtr(new ICalendar(uri, autoRefreshInterval));
    }

private:
    icalcomponent* _pICalendar;
        ///< The underlying libical representation of the calendar.
        ///< The C++ wrapper manages the memory internally.

    Poco::URI _uri;
        ///< The URI of the store.

    Poco::Timer _autoUpdateTimer;
        ///< A threaded timer to

    void update(ofEventArgs& args);
        ///< A callback for the ofApp to keep everything in the
        ///< main thread.  Is automatically registered and unregistered
        ///< upon ICalendar construction and destruction.

    void onAutoUpdate(Poco::Timer& timer);
        ///< onUpdate is called when the calendar is refreshed

    ofBuffer _calendarBuffer;
        ///< A string buffer to hold the auto-refreshed ICalendar buffer.

    mutable ofMutex _mutex;
        ///< The mutex used to prevent simultaneous calls to parse.


    bool loadURI(const Poco::URI& uri, ofBuffer& buffer);
        ///< loads a URI to a string

};


inline std::ostream& operator << (std::ostream& os, const ICalendar& calendar)
{
    if (calendar.isLoaded())
    {
        char* pString = icalcomponent_as_ical_string(calendar._pICalendar);
        os.write(pString, std::strlen(pString));
    }

    return os;
}


} } // namespace ofx::Time
