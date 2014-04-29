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
