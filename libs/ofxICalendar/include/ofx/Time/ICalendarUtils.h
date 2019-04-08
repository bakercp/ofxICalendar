//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <string>
#include <cstring>
#include <libical/ical.h>
#include "Poco/File.h"
#include "Poco/Timestamp.h"
#include "Poco/Timezone.h"
#include "Poco/DateTimeFormatter.h"
#include "ofLog.h"


namespace ofx {
namespace Time {


/// \brief A collection of utilities for ICalendar manipulations.
class ICalendarUtils
{
public:
    /// \brief Convert a time in icaltimetype format to as Poco::Timestamp.
    /// \param time a timein icaltimetype format.
    /// \param timestamp the timestamp to be filled after conversion.
    /// \returns true iff the passed icaltimetype is both non-null
    /// and valid.  A valid time is assigned to the Poco::Timestamp.
    static bool timeToTimestamp(struct icaltimetype time,
                                Poco::Timestamp& timestamp);

    /// \brief Get the value of an X-property extension via key.
    /// \param pComponent a pointer to the icalendar component.
    /// \param key the value of the extension value's key.
    /// \param value the value to be filled upon success.
    /// \returns true iff the icalendar X property defined by the key
    /// was found icalcomponent.  A key with a valid value is assigned
    /// to the passed value.
    static bool getExtensionValue(icalcomponent* pComponent,
                                  const std::string& key,
                                  std::string& value);

//    static void sortByStartTime(std::vector<ICalendarEvent>& events);
//
//    static void sortByStartTime(std::vector<ICalendarEventInstance>& events);

//    static icaltimezone* getTimezoneForTZID(ICalendar& calendar,
//                                            const std::string& tzid);
//        ///< Returns a pointer to a known icaltimezone if the zone is defined
//        ///< in the icalendar file.  If the zone is not defined, 0 is returned.


};


} } // namespace ofx::Time
