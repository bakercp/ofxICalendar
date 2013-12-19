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
#include "ofx/Time/ICalendarEvent.h"
#include "ofx/Time/Interval.h"
#include "ofLog.h"


namespace ofx {
namespace Time {


class ICalendarEventInstance
    // An ICalendarEventInstance class stores a Range representing an instance
    // of an event and a copy to the original event's unique idetification key.
    // An ICalendarEventInstance can represent a singular event associated with
    // an Event or one instantiation of an Event recurrence.
    //
    // Before using an ICalendarEventInstance the user should verify that the
    // instance is valid by calling isValidEventInstance().  Depending on the
    // situation, the event instance may become invalid if the backing ICalendar
    // no longer contains the event.
{
public:
    ICalendarEventInstance(const ICalendarEvent& event,
                           const Interval& interval);
        ///< Creates an ICalendarEventInstance.

    virtual ~ICalendarEventInstance();
        ///< Destroys an ICalendarEventInstance.

    ICalendarEvent getEvent() const;
        ///< Returns the original ICalendarEvent

    Interval getInterval() const;
        ///< Returns the Interval of the ICalendarEventInstance.

    bool isValidEventInstance() const;
        ///< Returns true iff the event is valid and the interval
        ///< is a valid interval for this instance.

    bool operator == (const ICalendarEventInstance& other) const;
        ///< Returns true iff all fields of the
        ///< ICalendarEventInstance are exactly the same.

    bool operator != (const ICalendarEventInstance& other) const;
        ///< Returns true iff all fields of the
        ///< ICalendarEventInstance not exactly the same.

	bool operator >  (const ICalendarEventInstance& other) const;
        ///< Returns true iff this instance's minimum
        ///< is greater than the given instance's minimum.

	bool operator >= (const ICalendarEventInstance& other) const;
        ///< Returns true iff the this instance's minimum
        ///< is greater than or equal to the given instance's minimum.

    bool operator <  (const ICalendarEventInstance& other) const;
        ///< Returns true iff the instance instance's minimum is
        ///< less than the given instance's minimum.

    bool operator <= (const ICalendarEventInstance& other) const;
        ///< Returns true iff the this instance's minimum
        ///< is less than or equal to the given instance's minimum.

private:
    ICalendarEvent _event;  ///< A copy of the event source for this instance
    Interval _interval;     ///< The time range for the instance of the event.

};


} } // namespace ofx::Time
