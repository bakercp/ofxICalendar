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
#include "Poco/Exception.h"
#include "ofEvents.h"
#include "ofx/Time/ICalendarEventInstance.h"


namespace ofx {
namespace Time {


/// \brief ICalendarWatcherEvents is a collection of events that might be
/// triggered while watching an ICalendar.
///
/// Before using an ICalendarEventInstance that has been passed with
/// and event, the user should verify that the instance is valid by
/// calling isValidEventInstance().  Depending on the situation,
/// the event instance may become invalid if the backing ICalendar
/// no longer contains the event.
class ICalendarWatcherEvents
{
public:
    /// \brief Creates an ICalendarWatcherEvents object.
    ICalendarWatcherEvents()
    {
    }
    /// \brief Destroys an ICalendarWatcherEvents object.
    virtual ~ICalendarWatcherEvents()
    {
    }

    ofEvent<const ICalendarEventInstance> onEventAdded;
        ///< Called when a new in-progress event first appears to the watcher.
        ///< This might happen when a calendar is first loaded and an event
        ///< is already in progress, or when a calendar is externally updated
        ///< and the watcher the beginning of the event.

    ofEvent<const ICalendarEventInstance> onEventRemoved;
        ///< Called when a tracked event is removed from the watcher before
        ///< it ends.  This might happen if a calendar is removed from the
        ///< watch list or when an event is externally removed from the watched
        ///< calendar before its natural end time.

    ofEvent<const ICalendarEventInstance> onEventModified;
        ///< Called when the details of a watched in-progress event is modified
        ///< modified externally.  This might happen when an in-progress event
        ///< is not moved, but a title, description, or beginning or end time
        ///< is modified.  If an in-progress event is "rescheduled", thereby
        ///< abuptly ending the event, the onEventRemoved will be called
        ///< rather than the onEventModified.

    ofEvent<const ICalendarEventInstance> onEventStarted;
        ///< Called when an event in a watched calendar begins.

    ofEvent<const ICalendarEventInstance> onEventEnded;
        ///< Called when an event in a watched calendar ends.

    ofEvent<const Poco::Exception> onError;
        ///< Called when an exception is passed.  Currently this is not used.
    
};
    
    
} } // namespace ofx::Time
