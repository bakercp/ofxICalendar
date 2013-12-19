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
#include <map>
#include <algorithm>
#include "ofEvents.h"
#include "ofEventUtils.h"
#include "ofx/Time/ICalendar.h"
#include "ofx/Time/ICalendarWatcherEvents.h"


namespace ofx {
namespace Time {


class ICalendarWatcher
    // A Watcher watches a given Calendar for changes.  Additionally
    // it provides events for ICalendarEvent beginnings and endings.
    // These events can be used to trigger any behavior in the
    // ICalendarWatcherEvent listener.
{
public:
    typedef std::shared_ptr<ICalendarWatcher> SharedPtr;
        ///< A shared pointer.

    ICalendarWatcher(ICalendar::SharedPtr calendar);
        ///< Creates a Watcher.

    virtual ~ICalendarWatcher();
        ///< Destroys a watcher.

    void setUpdateInterval(unsigned long long updateInterval);
        ///< Sets the update interval in milliseconds.

    unsigned long long getUpdateInterval() const;
        ///< Returns the current update interval in milliseconds.

    template<class ListenerClass>
    void registerAllEvents(ListenerClass* listener, int order = OF_EVENT_ORDER_AFTER_APP)
        ///< A utility method for registering a ListenerClass to listen
        ///< for ICalendarWatcherEvents. The registering class must implement
        ///< all of the following callback methods:
        ///<     void ListenerClass::onCalendarWatcherEventAdded(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherEventRemoved(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherEventModified(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherEventStarted(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherEventEnded(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherError(const Poco::Exception& args);
        ///< Users interested in custom callback names should manually register
        ///< callbacks for specific events using the ofAddListener() method.
    {
        ofAddListener(events.onEventAdded, listener, &ListenerClass::onCalendarWatcherEventAdded, order);
        ofAddListener(events.onEventRemoved, listener, &ListenerClass::onCalendarWatcherEventRemoved, order);
        ofAddListener(events.onEventModified, listener, &ListenerClass::onCalendarWatcherEventModified, order);
        ofAddListener(events.onEventStarted, listener, &ListenerClass::onCalendarWatcherEventStarted, order);
        ofAddListener(events.onEventEnded, listener, &ListenerClass::onCalendarWatcherEventEnded, order);
        ofAddListener(events.onError, listener, &ListenerClass::onCalendarWatcherError, order);
    }

    template<class ListenerClass>
    void unregisterAllEvents(ListenerClass* listener)
        ///< A utility method for unregistering a ListenerClass to listen
        ///< for ICalendarWatcherEvents. The registering class must implement
        ///< all of the following callback methods:
        ///<     void ListenerClass::onCalendarWatcherEventAdded(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherEventRemoved(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherEventModified(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherEventStarted(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherEventEnded(const ICalendarWatcherEventArgs& args);
        ///<     void ListenerClass::onCalendarWatcherError(const Poco::Exception& args);
        ///< Users interested in custom callback names should manually register
        ///< callbacks for specific events using the ofRemoveListener() method.
    {
        ofRemoveListener(events.onEventAdded, listener, &ListenerClass::onCalendarWatcherEventAdded);
        ofRemoveListener(events.onEventRemoved, listener, &ListenerClass::onCalendarWatcherEventRemoved);
        ofRemoveListener(events.onEventModified, listener, &ListenerClass::onCalendarWatcherEventModified);
        ofRemoveListener(events.onEventStarted, listener, &ListenerClass::onCalendarWatcherEventStarted);
        ofRemoveListener(events.onEventEnded, listener, &ListenerClass::onCalendarWatcherEventEnded);
        ofRemoveListener(events.onError, listener, &ListenerClass::onCalendarWatcherError);
    }

    ICalendarWatcherEvents events;
        ///< A class containing all events for this watcher.
        ///< Listener classes can call the utility methods
        ///< registerAllEvents() or unregisterAllEvents() to
        ///< register callbacks or manually register for individual
        ///< events using ofAddListener() and ofRemoveListener().

    static const Poco::Timespan DEFAULT_UPDATE_INTERVAL;
        ///< The default update interval in microseconds for updating the watch.

    static SharedPtr makeShared(ICalendar::SharedPtr calendar)
    {
        return SharedPtr(new ICalendarWatcher(calendar));
    }

private:
    ICalendar::SharedPtr _calendar;
        ///< The calendar being watched.

    ICalendar::EventInstances _watches;
        ///< A collection of all current watches.

    std::map<std::string, Poco::Timestamp> _watchesLastUpdated;
        ///< A map of the last updated times for the current watches.

    Poco::Timestamp _lastUpdate;
        ///< The last time the watches were updated.

    Poco::Timespan _updateInterval;
        ///< The timespan for updating the watches.

    void update(ofEventArgs& args);
        ///< A callback for the ofApp to keep everything in the
        ///< main thread.  Is automatically registered and unregistered
        ///< upon Watcher construction and destruction.

    void refresh();
        ///< Manually refresh the watch.  Overrides auto watcher and
        ///< updates the _lastUpdate variable.

    static bool compareUID(const ICalendarEventInstance& lhs, const ICalendarEventInstance& rhs);
        ///< Returns true iff the left event's uid
        ///< goes before the right event's uid.

};


} } // namespace ofx::Time
