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


/// \brief A Watcher watches a given Calendar for changes.
///
/// Additionally it provides events for ICalendarEvent beginnings and endings.
/// These events can be used to trigger any behavior in the
/// ICalendarWatcherEvent listener.
class ICalendarWatcher
{
public:
    /// \brief A shared pointer.
    typedef std::shared_ptr<ICalendarWatcher> SharedPtr;

    /// \brief Creates a Watcher.
    /// \param calendar A shared pointer to a calendar to watch.
    ICalendarWatcher(ICalendar::SharedPtr calendar);

    /// \brief Destroys the watcher.
    virtual ~ICalendarWatcher();

    /// \brief Sets the update interval in milliseconds.
    /// \param updateInterval the update interval in milliseconds.
    void setUpdateInterval(unsigned long long updateInterval);

    /// \returns the current update interval in milliseconds.
    unsigned long long getUpdateInterval() const;

    /// \brief A utility method for registering a ListenerClass to listen
    /// for ICalendarWatcherEvents.
    ///
    /// The registering class must implement all of the following callback methods:
    ///     void ListenerClass::onCalendarWatcherEventAdded(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherEventRemoved(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherEventModified(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherEventStarted(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherEventEnded(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherError(const Poco::Exception& args);
    /// Users interested in custom callback names should manually register
    /// callbacks for specific events using the ofAddListener() method.
    /// \param listener a pointer to the listener class that implements
    /// the callback methods.
    /// \param order The event callback order.
    template<class ListenerClass>
    void registerAllEvents(ListenerClass* listener, int order = OF_EVENT_ORDER_AFTER_APP)
    {
        ofAddListener(events.onEventAdded, listener, &ListenerClass::onCalendarWatcherEventAdded, order);
        ofAddListener(events.onEventRemoved, listener, &ListenerClass::onCalendarWatcherEventRemoved, order);
        ofAddListener(events.onEventModified, listener, &ListenerClass::onCalendarWatcherEventModified, order);
        ofAddListener(events.onEventStarted, listener, &ListenerClass::onCalendarWatcherEventStarted, order);
        ofAddListener(events.onEventEnded, listener, &ListenerClass::onCalendarWatcherEventEnded, order);
        ofAddListener(events.onError, listener, &ListenerClass::onCalendarWatcherError, order);
    }

    /// \brief A utility method for unregistering a ListenerClass to listen
    /// for ICalendarWatcherEvents.
    ///
    /// The registering class must implement all of the following callback methods:
    ///     void ListenerClass::onCalendarWatcherEventAdded(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherEventRemoved(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherEventModified(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherEventStarted(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherEventEnded(const ICalendarWatcherEventArgs& args);
    ///     void ListenerClass::onCalendarWatcherError(const Poco::Exception& args);
    /// Users interested in custom callback names should manually register
    /// callbacks for specific events using the ofRemoveListener() method.
    /// \param listener a pointer to the listener class that implements
    /// the callback methods.
    template<class ListenerClass>
    void unregisterAllEvents(ListenerClass* listener)
    {
        ofRemoveListener(events.onEventAdded, listener, &ListenerClass::onCalendarWatcherEventAdded);
        ofRemoveListener(events.onEventRemoved, listener, &ListenerClass::onCalendarWatcherEventRemoved);
        ofRemoveListener(events.onEventModified, listener, &ListenerClass::onCalendarWatcherEventModified);
        ofRemoveListener(events.onEventStarted, listener, &ListenerClass::onCalendarWatcherEventStarted);
        ofRemoveListener(events.onEventEnded, listener, &ListenerClass::onCalendarWatcherEventEnded);
        ofRemoveListener(events.onError, listener, &ListenerClass::onCalendarWatcherError);
    }

    /// \brief A class containing all events for this watcher.
    ///
    /// Listener classes can call the utility methods registerAllEvents() or
    /// unregisterAllEvents() to register callbacks or manually register for
    /// individual events using ofAddListener() and ofRemoveListener().
    ICalendarWatcherEvents events;

    /// \brief The default update interval for updating the watch.
    static const Poco::Timespan DEFAULT_UPDATE_INTERVAL;

    /// \brief Make a shared instance.
    static SharedPtr makeShared(ICalendar::SharedPtr calendar)
    {
        return SharedPtr(new ICalendarWatcher(calendar));
    }

private:
    /// \brief The calendar being watched.
    ICalendar::SharedPtr _calendar;

    /// \brief A collection of all current watches.
    ICalendar::EventInstances _watches;

    /// \brief A map of the last updated times for the current watches.
    std::map<std::string, Poco::Timestamp> _watchesLastUpdated;

    /// \brief The last time the watches were updated.
    Poco::Timestamp _lastUpdate;

    /// \brief The timespan for updating the watches.
    Poco::Timespan _updateInterval;

    /// \brief A callback for the ofApp to keep everything in the
    /// main thread.  Is automatically registered and unregistered
    /// upon Watcher construction and destruction.
    void update(ofEventArgs& args);

    /// \brief Manually refresh the watch. Overrides auto watcher and
    /// updates the _lastUpdate variable.
    void refresh();

    /// \brief Compare two ICalendarEventInstances.
    /// \param lhs the left hand side instance.
    /// \param rhs the right hand side instance.
    /// \returns true iff the left event's uid is before the right event's uid.
    static bool compareUID(const ICalendarEventInstance& lhs,
                           const ICalendarEventInstance& rhs);

};


} } // namespace ofx::Time
