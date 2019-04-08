//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <libical/ical.h>


namespace ofx {
namespace Time {

    
/// \brief The provides access to underlying libicalcomponent pointers.
class ICalendarInterface
{
public:
    /// \brief Frees the internal icalcomponent* if it was allocated.
    virtual ~ICalendarInterface()
    {
    }

    /// \returns the underlying libicalcomponent.
    virtual icalcomponent* getComponent() = 0;

    /// \returns the underlying libicalcomponent.
    virtual const icalcomponent* getComponent() const = 0;

};


} } // namespace ofx::Time
