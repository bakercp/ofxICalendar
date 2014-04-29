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

    
/// \brief The ICalendar class implements the RFC 2445 icalendar format.
class ICalendar:
    public ICalendarInterface,
    public ofThread
{
public:
    /// \brief A shared pointer typedef.
    typedef std::shared_ptr<ICalendar> SharedPtr;

    /// \brief A weak pointer typedef.
    typedef std::weak_ptr<ICalendar> WeakPtr;

    /// \brief A collection of events.
    typedef std::vector<ICalendarEvent> Events;

    /// \brief A collection of events.
    typedef std::vector<ICalendarEventInstance> EventInstances;

    /// \brief Creates a calendar with the given uri.
    /// \param uri the uri of the calnedar.
    /// \param autoRefreshInterval the automatic refresh interval.
    ICalendar(const std::string& uri, unsigned long long autoRefreshInterval = 0);

    /// \brief Copy constructor.
    ///
    /// This copy constructor manages the internal libical objects.
    ICalendar(const ICalendar& other);

    /// \brief Assignment.
    ///
    /// This copy constructor manages the internal libical objects.
    ICalendar& operator = (ICalendar other);

    /// \brief Frees the internal  memory.
    ///
    /// This copy constructor manages the internal libical objects.
    virtual ~ICalendar();

    /// \brief Check to see if the calendar is loaded.
    ///
    /// When the calendar is created with a default constructor or when a
    /// load or parse operation has failed, this method will return false.
    ///
    /// \returns true iff a valid icalcomponent* is allocated.
    bool isLoaded() const;

    /// \brief Sets the URI for the ICalendar file / url (e.g. *.ics) path.
    ///
    /// The uri can be specify a remote location
    /// (e.g. https://example.com/cal.ics) or can be specified as a
    /// local file (e.g. basic.ics).  Local files without an absolute
    /// path will be located relative to the `bin/data` directory.
    /// The file must conform to the RFC 2445 specification.
    ///
    /// \param uri the URI to set.
    void setURI(const std::string& uri);

    /// \brief get the URI for the ICalendar file.
    /// \returns the URI for the ICalendar file.  If none is set, the returned
    /// Poco::URI will be empty.
    Poco::URI getURI() const;

    /// \brief Set the auto update interval.
    ///
    /// Auto updates will attempt to reload the last URI loaded.
    /// If no URI has been loaded, no auto updates will be attempted.
    /// Auto update is disabled if the interval is set to 0.
    ///
    /// \param autoRefreshInterval automatic refresh interval in milliseconds.
    void setAutoRefreshInterval(unsigned long long autoRefreshInterval);

    /// \brief Get the auto refresh interval.
    /// \returns Returns the number of milliseconds in the refresh interval.
    /// or 0 if auto refresh is disabled.
    unsigned long long getAutoRefreshInterval() const;

    /// \brief Loads data from a text buffer containing an icalendar file.
    ///
    /// The buffered data must conform to the RFC 2445 specification.
    /// If the user is manually parsing a buffer (as opposed to using
    /// a URI and auto refresh interval, the user must be certain to
    /// maintain thread safety.  Additionally, the user must disable
    /// auto refresh by setting the URI to an empty string and / or
    /// setting the auto update interval to 0.
    ///
    /// \param buffer the buffer containing icalendar data to parse.
    /// \returns true iff successful.
    bool parse(const ofBuffer& buffer);

    /// \returns the calendar's product id
    /// (e.g. -//Google Inc//Google Calendar 70.9054//EN)
    /// or an empty std::string if no PRODID field exists.
    std::string getProductID() const;

    /// \returns the calendar's icalendar version number.
    std::string getVersion() const;

    /// \returns the name of the calendar by examining the X-WR-CALNAME
    /// field or an empty std::string if no X-WR-CALNAME field exists.
    /// This extended field is not part of the RFC 2445
    /// specification, but used by Google Calendar.
    std::string getName() const;

    /// \returns the name of the calendar by examining the X-WR-CALDESC
    /// field or an empty std::string if no X-WR-CALDESC field exists.
    /// This extended field is not part of the RFC 2445
    /// specification, but used by Google Calendar.
    std::string getDescription() const;

    /// \returns the value of the CALSCALE field or an empty std::string
    /// if no CALSCALE tag exists.
    std::string getScale() const;

    /// \brief Get the number of VEVENT elements in the calendar.
    /// \returns the number of VEVENT elements in the calendar.
    std::size_t getNumEvents() const;

    /// \brief Get the most recent LAST-MODIFIED tag from all events.
    /// \returns the most recent LAST-MODIFIED tag from all events.
    /// If no LAST-MODIFIED tag exists, returns Poco::Timestamp(0).
    Poco::Timestamp getLastModified() const;

//        ///< Returns true iff an event with a UID matching uid is found.  If true
//        ///< the first matching event will be assigned to the event argument.
//        ///< Returns false if a matching event is not found.
//    bool getEventByUID(const std::string& uid, ICalendarEvent& event) const;

    /// \returns all calendar events.
    Events getEvents() const;

    /// \returns all events that have some overlap with the given range.
    /// All event recurrences are checked for overlap.
    Events getEvents(const Interval& interval) const;

    /// \returns all events that contain the given timestamp.
    /// All event recurrences are checked for overlap.
    Events getEvents(const Poco::Timestamp& timestamp) const;

    /// \returns all event instances that overlap with the given range.
    EventInstances getEventInstances(const Interval& interval) const;

    /// \returns all event instances that contain the given timestamp.
    EventInstances getEventInstances(const Poco::Timestamp& timestamp) const;

    /// \brief Get the raw icalcomponent pointer.
    /// \returns a pointer the underlying libicalcomponent.
    icalcomponent* getComponent();

    /// \brief Get the raw icalcomponent pointer.
    /// \returns a pointer to the the underlying libicalcomponent.
    icalcomponent* getComponent() const;

    /// \brief Passes the internal icalcomponent text to the output stream.
    ///
    /// (e.g. std::cout << myCalendar << std::endl will dump the
    /// calendar to the standard output as a RFC 2445 icalendar string).
	friend std::ostream& operator << (std::ostream& os, const ICalendar& vec);

    /// \brief Make a shared instance.
    static SharedPtr makeShared(const std::string& uri,
                                unsigned long long autoRefreshInterval = 0)
    {
        return SharedPtr(new ICalendar(uri, autoRefreshInterval));
    }

    /// \brief The default update interval updating the watch.
    static const Poco::Timespan DEFAULT_UPDATE_INTERVAL;

    /// \brief The thread function used for auto updates.
    virtual void threadedFunction()
    {
        while (isThreadRunning())
        {
            Poco::Timestamp now;

            if (_autoUpdateInterval > 0 && now >= _nextUpdate)
            {
                reload();
                _nextUpdate = now + _autoUpdateInterval * Poco::Timespan::MILLISECONDS;
            }

            sleep(1000);
        }
    }

    /// \brief Reload the calendar from the URI.
    void reload()
    {
        if (!_uri.empty())
        {
            // cout << "reloading calendar." << endl;
            ofBuffer buffer;

            if (loadURI(_uri, buffer))
            {
                //cout << "success!." << endl;

                ofScopedLock lock(_mutex);
                _calendarBuffer = buffer; // lock while we set the buffer
            }
            else
            {
                //cout << "not successsful." << endl;

            }
        }
    }

private:
    /// \brief The underlying libical representation of the calendar.
    ///
    /// The C++ wrapper manages the memory internally.
    icalcomponent* _pICalendar;

    /// \brief The URI of the store.
    Poco::URI _uri;

    /// \brief A record of the nextupdate;
    Poco::Timestamp _nextUpdate;

    /// \brief An automatic update interval.
    unsigned long long _autoUpdateInterval;

    /// \brief A string buffer to hold the auto-refreshed ICalendar buffer.
    ofBuffer _calendarBuffer;

    /// \brief The mutex used to prevent simultaneous calls to parse.
    mutable ofMutex _mutex;

    /// \brief A callback for the ofApp to keep everything in the main thread.
    ///
    /// Is automatically registered and unregistered upon ICalendar construction
    /// and destruction.
    void update(ofEventArgs& args);

    // \brief onUpdate is called when the calendar is refreshed.
    // \param timer is the poco timer that was used.
    // void onAutoUpdate(Poco::Timer& timer);

    /// \brief Loads a URI to a string
    bool loadURI(const Poco::URI& uri, ofBuffer& buffer);

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
