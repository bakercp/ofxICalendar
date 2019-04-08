//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofx/Time/ICalendarWatcher.h"


namespace ofx {
namespace Time {


const Poco::Timespan ICalendarWatcher::DEFAULT_UPDATE_INTERVAL_MILLIS = 1 * Poco::Timespan::MINUTES;


ICalendarWatcher::ICalendarWatcher(std::shared_ptr<ICalendar> calendar):
    _calendar(calendar),
    _updateIntervalMillis(DEFAULT_UPDATE_INTERVAL_MILLIS)
{
    _lastUpdateMillis.update();
    _lastUpdateMillis -= DEFAULT_UPDATE_INTERVAL_MILLIS.totalMicroseconds();

    ofAddListener(ofEvents().update, this, &ICalendarWatcher::update);
}


ICalendarWatcher::~ICalendarWatcher()
{
    ofRemoveListener(ofEvents().update, this, &ICalendarWatcher::update);
}


void ICalendarWatcher::setUpdateIntervalMillis(uint64_t updateIntervalMillis)
{
    _updateIntervalMillis = Poco::Timespan(Poco::Timespan::MILLISECONDS * updateIntervalMillis);
}


uint64_t ICalendarWatcher::getUpdateIntervalMillis() const
{
    return _updateIntervalMillis.milliseconds();
}


void ICalendarWatcher::refresh()
{
    Poco::Timestamp now; // the current time

    if (_calendar)
    {
        ICalendar::EventInstances oldInstances = _watches;
        ICalendar::EventInstances newInstances = _calendar->getEventInstances(now);

        // sort uids lexigraphically
        std::sort(oldInstances.begin(), oldInstances.end(), compareUID);
        std::sort(newInstances.begin(), newInstances.end(), compareUID);

        // unmatchedOldInstances are those events instances that are NOT
        // seen in the current instanaces.  if an instance is NOT seen,
        // that means that an instance has ended (we can check for that)
        // OR the event was "modified" -- that is -- the equality
        // relationship didn't hold and we need to check for UID equality
        // in this case -- OR the event was deleted altogether from the
        // calendar.

        ICalendar::EventInstances unmatchedOldInstances;

        // unmatchedNewInstances are those instances that did not have an
        // equality match inside of the watchedInstances vector.  Events in
        // this array can fall into three categories: 1) a modified event
        // -- == opererator failed because of a different modified date,
        // 2) an event that has just started 3) an in-progress event that
        // was added after its start time.

        ICalendar::EventInstances unmatchedNewInstances;

        // newWatches will eventually replace _watches when the refresh is done
        ICalendar::EventInstances newWatches;

        // std::set_difference - what is in first set, but not in the second one

        // TODO: combine all three of these steps into a single loop

        // find any instances that are in the current watch list that are no longer there
        std::set_difference(oldInstances.begin(),
                            oldInstances.end(),
                            newInstances.begin(),
                            newInstances.end(),
                            std::inserter(unmatchedOldInstances,
                                          unmatchedOldInstances.end()),
                            compareUID);

        // find any new instances that are not matched in the current watchlist
        std::set_difference(newInstances.begin(),
                            newInstances.end(),
                            oldInstances.begin(),
                            oldInstances.end(),
                            std::inserter(unmatchedNewInstances,
                                          unmatchedNewInstances.end()),
                            compareUID);

        // find all instances that are still are the same in both sets.
        // these instances must be checked for modification times.
        std::set_intersection(newInstances.begin(),
                              newInstances.end(),
                              oldInstances.begin(),
                              oldInstances.end(),
                              std::inserter(newWatches,
                                            newWatches.end()),
                              compareUID);

        // process the unmatched old instances
        ICalendar::EventInstances::iterator unmatchedOldInstancesIter = unmatchedOldInstances.begin();

        while (unmatchedOldInstancesIter != unmatchedOldInstances.end())
        {
            ICalendarEventInstance& instance = *unmatchedOldInstancesIter;
            Poco::Timestamp startTime = instance.getInterval().getStart();
            Poco::Timestamp endTime = instance.getInterval().getEnd();

            if (!instance.isValidEventInstance() || endTime.epochTime() < _lastUpdateMillis.epochTime())
            {
                ofNotifyEvent(events.onEventRemoved, instance, this);
            }
            else
            {
                ofNotifyEvent(events.onEventEnded, instance, this);
            }

            ++unmatchedOldInstancesIter;
        }

        // process the old matches and check them for updates
        // process the unmatched old instances
        ICalendar::EventInstances::iterator newWatchesIter = newWatches.begin();

        while (newWatchesIter != newWatches.end())
        {
            const ICalendarEvent& evt = (*newWatchesIter).getEvent();

            std::string uid = evt.getUID();
            Poco::Timestamp lastModified = evt.getLastModified();

            std::map<std::string, Poco::Timestamp>::iterator lastUpdatedIter = _watchesLastUpdated.find(uid);

            if (lastUpdatedIter != _watchesLastUpdated.end() &&
                lastModified.epochTime() > _watchesLastUpdated[uid].epochTime())
            {
                ofNotifyEvent(events.onEventModified, *newWatchesIter, this);
            }

            ++newWatchesIter;
        }

        // process the new instances (add them to the watch list and the updated group)
        ICalendar::EventInstances::iterator unmatchedNewInstancesIter = unmatchedNewInstances.begin();

        while (unmatchedNewInstancesIter != unmatchedNewInstances.end())
        {
            ICalendarEventInstance& instance = *unmatchedNewInstancesIter;
            std::string uid = instance.getEvent().getUID();
            Poco::Timestamp startTime = instance.getInterval().getStart();
            Poco::Timestamp endTime = instance.getInterval().getEnd();

            if (startTime.epochTime() >= _lastUpdateMillis.epochTime())
            {
                ofNotifyEvent(events.onEventStarted, instance, this);
            }
            else
            {
                ofNotifyEvent(events.onEventAdded, instance, this);
            }

            newWatches.push_back(instance); // add to current watches

            ++unmatchedNewInstancesIter;
        }

        // make a record of the watches
        _watches = newWatches;

        // clear out old last udpated lists
        _watchesLastUpdated.clear();

        ICalendar::EventInstances::iterator iter = _watches.begin();

        while (iter != _watches.end())
        {
            const ICalendarEvent& evt = (*iter).getEvent();
            _watchesLastUpdated[evt.getUID()] = evt.getLastModified();
            ++iter;
        }
    }
    else
    {
        ofLogError("ICalendarWatcher::refresh()") << "Calendar null.";
    }

    _lastUpdateMillis = now;
}

bool ICalendarWatcher::compareUID(const ICalendarEventInstance& lhs,
                                  const ICalendarEventInstance& rhs)
{
    return lhs.getEvent().getUID() < rhs.getEvent().getUID();
}


void ICalendarWatcher::update(ofEventArgs& args)
{
    if (_lastUpdateMillis.isElapsed(_updateIntervalMillis.totalMicroseconds()))
    {
        refresh();
    }
    else
    {
        // wait until next time
    }
}


} } // namespace ofx::Time
