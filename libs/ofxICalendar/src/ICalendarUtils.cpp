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


#include "ofx/Time/ICalendarUtils.h"


namespace ofx {
namespace Time {


bool ICalendarUtils::timeToTimestamp(struct icaltimetype time,
                                     Poco::Timestamp& timestamp)
{
    if (!icaltime_is_null_time(time))
    {
        if (icaltime_is_valid_time(time))
        {
            timestamp = Poco::Timestamp::fromEpochTime(icaltime_as_timet(time));
            return true;
        }
        else
        {
            ofLogError("Utils::timeToTimestamp()") << "icaltimetype is invalid";
            timestamp = Poco::Timestamp(0);;
            return false;
        }
    }
    else
    {
        ofLogError("Utils::timeToTimestamp()") << "icaltimetype is null";
        timestamp = Poco::Timestamp(0);;
        return false;
    }
}


bool ICalendarUtils::getExtensionValue(icalcomponent* pComponent,
                                       const std::string& key,
                                       std::string& value)
{
    value.clear();

    if (pComponent)
    {
        icalproperty* pProperty = 0;
        const char* pKey = 0;
        const char* pValue = 0;

        pProperty = icalcomponent_get_first_property(pComponent,
                                                     ICAL_X_PROPERTY);

        while (pProperty)
        {
            pKey = icalproperty_get_x_name(pProperty);

            if (0 == std::strcmp(pKey, key.c_str()))
            {
                pValue = icalproperty_get_value_as_string(pProperty);

                if (pValue)
                {
                    value = std::string(pValue);
                    return true;
                }
            }

            pProperty = icalcomponent_get_next_property(pComponent,
                                                        ICAL_X_PROPERTY);
        }

        return false;
    }
    else
    {
        return false;
    }

}


//icaltimezone* ICalendarUtils::getTimezoneForTZID(icalcomponent* component, const std::string& tzid)
//{
//    if (0 != component)
//    {
//        return icalcomponent_get_timezone(component, tzid.c_str());
//    }
//    else
//    {
//        return 0;
//    }
//
//}


} } // namespace ofx::Time
