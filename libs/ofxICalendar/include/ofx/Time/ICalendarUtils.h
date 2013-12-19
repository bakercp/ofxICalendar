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
#include <libical/ical.h>
#include "Poco/File.h"
#include "Poco/Timestamp.h"
#include "Poco/Timezone.h"
#include "Poco/DateTimeFormatter.h"
#include "ofLog.h"


namespace ofx {
namespace Time {


class ICalendarUtils
    /// Utils contains a collection of utilities for simplifying interactions
    /// between openFrameworks, Poco and libical.
{
public:
    static bool timeToTimestamp(struct icaltimetype time,
                                Poco::Timestamp& timestamp);
        ///< Returns true iff the passed icaltimetype is both non-null
        ///< and valid.  A valid time is assigned to the Poco::Timestamp.

    static bool getExtensionValue(icalcomponent* pComponent,
                                  const std::string& key,
                                  std::string& value);
        ///< Returns true iff the icalendar X property defined by the key
        ///< was found icalcomponent.  A key with a valid value is assigned
        ///< to the passed value.

//    static void sortByStartTime(std::vector<ICalendarEvent>& events);
//
//    static void sortByStartTime(std::vector<ICalendarEventInstance>& events);

//    static icaltimezone* getTimezoneForTZID(ICalendar& calendar,
//                                            const std::string& tzid);
//        ///< Returns a pointer to a known icaltimezone if the zone is defined
//        ///< in the icalendar file.  If the zone is not defined, 0 is returned.


};


} } // namespace ofx::Time
