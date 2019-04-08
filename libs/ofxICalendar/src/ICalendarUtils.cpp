//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


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
