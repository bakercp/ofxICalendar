//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


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
