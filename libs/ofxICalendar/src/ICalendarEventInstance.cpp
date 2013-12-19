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


#include "ofx/Time/ICalendarEventInstance.h"


namespace ofx {
namespace Time {


ICalendarEventInstance::ICalendarEventInstance(const ICalendarEvent& event,
                                               const Interval& interval):
    _event(event),
    _interval(interval)
{
}


ICalendarEventInstance::~ICalendarEventInstance()
{
}


ICalendarEvent ICalendarEventInstance::getEvent() const
{
    return _event;
}


Interval ICalendarEventInstance::getInterval() const
{
    return _interval;
}


bool ICalendarEventInstance::isValidEventInstance() const
{
    return _event.isValid() && _event.isValidInterval(_interval);
}


bool ICalendarEventInstance::operator == (const ICalendarEventInstance& other) const
{
    return _event == other._event && _interval == other._interval;
}


bool ICalendarEventInstance::operator != (const ICalendarEventInstance& other) const
{
    return _event != other._event || _interval != other._interval;
}


bool ICalendarEventInstance::operator >  (const ICalendarEventInstance& other) const
{
    return _event > other._event;
}


bool ICalendarEventInstance::operator >= (const ICalendarEventInstance& other) const
{
    return _event >= other._event;
}


bool ICalendarEventInstance::operator <  (const ICalendarEventInstance& other) const
{
    return _event < other._event;
}


bool ICalendarEventInstance::operator <= (const ICalendarEventInstance& other) const
{
    return _event <= other._event;
}


} } // namespace ofx::Time
