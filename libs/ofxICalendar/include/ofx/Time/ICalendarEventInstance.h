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


/// \brief Describes a specific instance of a given instance of an event.
///
/// An ICalendarEventInstance class stores a Range representing an instance
/// of an event and a copy to the original event's unique idetification key.
/// An ICalendarEventInstance can represent a singular event associated with
/// an Event or one instantiation of an Event recurrence.
///
/// Before using an ICalendarEventInstance the user should verify that the
/// instance is valid by calling isValidEventInstance().  Depending on the
/// situation, the event instance may become invalid if the backing ICalendar
/// no longer contains the event.
class ICalendarEventInstance
{
public:
    /// \brief Creates an ICalendarEventInstance.
    /// \param event The original ICalendarEvent.
    /// \param interval the specific interval associated with the event.
    ICalendarEventInstance(const ICalendarEvent& event,
                           const Interval& interval);

    /// \brief Destroys the ICalendarEventInstance.
    virtual ~ICalendarEventInstance();

    /// \brief Get the origina ICalendarEvent.
    /// \returns the original ICalendarEvent.
    ICalendarEvent getEvent() const;

    /// \returns the Interval of the ICalendarEventInstance.
    Interval getInterval() const;

    /// \returns true iff the event is valid and the interval
    /// is a valid interval for this instance.
    bool isValidEventInstance() const;

    /// \returns true iff all fields of the
    /// ICalendarEventInstance are exactly the same.
    bool operator == (const ICalendarEventInstance& other) const;

    /// \returns true iff all fields of the
    /// ICalendarEventInstance not exactly the same.
    bool operator != (const ICalendarEventInstance& other) const;

    /// \returns true iff this instance's minimum
    /// is greater than the given instance's minimum.
	bool operator >  (const ICalendarEventInstance& other) const;

    /// \returns true iff the this instance's minimum
    /// is greater than or equal to the given instance's minimum.
	bool operator >= (const ICalendarEventInstance& other) const;

    /// \returns true iff the instance instance's minimum is
    /// less than the given instance's minimum.
    bool operator <  (const ICalendarEventInstance& other) const;

    /// \returns true iff the this instance's minimum
    /// is less than or equal to the given instance's minimum.
    bool operator <= (const ICalendarEventInstance& other) const;

private:
    /// \brief A copy of the event source for this instance
    ICalendarEvent _event;

    /// \brief The time range Interval for the instance of the event.
    Interval _interval;

};


} } // namespace ofx::Time
