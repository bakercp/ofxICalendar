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


#include "ofx/Time/ICalendarWatcher.h"


namespace ofx {
namespace Time {


const Poco::Timespan ICalendarWatcher::DEFAULT_UPDATE_INTERVAL = 1 * Poco::Timespan::MINUTES;


ICalendarWatcher::ICalendarWatcher(ICalendar::SharedPtr calendar):
    _calendar(calendar),
    _updateInterval(DEFAULT_UPDATE_INTERVAL)
{
    _lastUpdate = Poco::Timestamp::Timestamp() - DEFAULT_UPDATE_INTERVAL.totalMicroseconds();

    ofAddListener(ofEvents().update, this, &ICalendarWatcher::update);
}


ICalendarWatcher::~ICalendarWatcher()
{
    ofRemoveListener(ofEvents().update, this, &ICalendarWatcher::update);
}


void ICalendarWatcher::setUpdateInterval(unsigned long long updateInterval)
{
    _updateInterval = Poco::Timespan(Poco::Timespan::MILLISECONDS * updateInterval);
}


unsigned long long ICalendarWatcher::getUpdateInterval() const
{
    return _updateInterval.milliseconds();
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

            if (!instance.isValidEventInstance() || endTime.epochTime() < _lastUpdate.epochTime())
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

            if (startTime.epochTime() >= _lastUpdate.epochTime())
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

    _lastUpdate = now;
}

bool ICalendarWatcher::compareUID(const ICalendarEventInstance& lhs,
                                  const ICalendarEventInstance& rhs)
{
    return lhs.getEvent().getUID() < rhs.getEvent().getUID();
}


void ICalendarWatcher::update(ofEventArgs& args)
{
    if (_lastUpdate.isElapsed(_updateInterval.totalMicroseconds()))
    {
        refresh();
    }
    else
    {
        // wait until next time
    }
}


} } // namespace ofx::Time
