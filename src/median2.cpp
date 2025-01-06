#include "median2.hpp"
#include <algorithm>

#ifdef DEBUG
#include <iterator>


std::ostream& operator<<(std::ostream& out, const Interval& i)
{
    out << "[ ";
    std::copy(i.start, i.end, std::ostream_iterator<int>(out, " "));
    out << "]";
    return out;
}


static void printRange(IntIterator start, IntIterator end, const char *msg = NULL)
{
    if(msg != NULL)
    {
        std::cout << msg << ": ";
    }
    std::cout << "[ ";
    std::copy(start, end, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "]";
}
#endif // #ifdef DEBUG


static bool isOdd(int x) { return (x & 1) != 0; }


double Solution::findMedianSortedArrays(
        const std::vector<int> &nums1,
        const std::vector<int> &nums2)
{
    //edge cases (one vector is empty)
    if(nums1.empty()) { return findMedianSortedArray(nums2); }
    else if(nums2.empty()) { return findMedianSortedArray(nums1); }

    foundSoFar = 0;
    findSortedIntervals(nums1, nums2);
    return findMedian(nums1.size() + nums2.size());
}


int Solution::requiredSortedCount(int totalSize)
{
    /* The median of a sorted container A of size S is:
       * A[S/2], if S is odd.
         Assuming we only have sequential access (e.g. linked list),
         we need to iterate through S/2+1 elements to
         find the median.
         Example: S=5 -> median is A[2], to find it we must
         iterate through elements at 0, 1, 2.
       * (A[S/2-1] + A[S/2])/2, if S is even.
         We need to iterate through S/2+1 elements to find it.
         Example: S=6 -> median is (A[2]+A[3])/2, to find it
         we must iterate through elements at 0, 1, 2, 3.
       So in both cases we need to iterate through S/2 +1 elements.
    */
    return totalSize / 2 + 1;
}


void Solution::findSortedIntervals(
        const std::vector<int> &nums1,
        const std::vector<int> &nums2)
{
    int totalSize = nums1.size() + nums2.size();

    const int requiredCount = requiredSortedCount(totalSize);

#ifdef DEBUG
    std::cout << "Combined size is " << totalSize
        << ", need " << requiredCount << " combined elements\n";
#endif // #ifdef DEBUG

    /* Successively determine contiguous intervals from either of the
       2 vectors, that if put together would create a sorted vector.

       Example:
       nums1 = { 10, 20, 30, 40 }
       nums2 = { 21, 22, 23, 31, 32, 33 }
       Then:
       - *intervalStart = 21
       - *pastIntervalEnd = 30
       - *intervalEnd = 31
    */
    IntIterator hostStart = nums1.begin();
    IntIterator hostEnd = nums1.end();
    IntIterator guestStart = nums2.begin();
    IntIterator guestEnd = nums2.end();
    bool done = false;
    while(! done)
    {
        std::vector<Interval> newIntervals = buildSortedIntervals(
                hostStart, hostEnd, guestStart, guestEnd);
        for(const Interval &i: newIntervals)
        {
            foundSoFar += i.size();
#ifdef DEBUG
            std::cout << "Found " << foundSoFar << " elements so far, "
                << requiredCount << " needed\n";
#endif // #ifdef DEBUG
            if(foundSoFar >= requiredCount)
            {
                pLastInterval = std::make_unique<Interval>(i);
                done = true;
                break;
            }
            lastInPrevInterval = *(i.end-1);
        }
    }
}


std::vector<Interval> Solution::buildSortedIntervals(
        IntIterator &hostStart,
        IntIterator hostEnd,
        IntIterator &guestStart,
        IntIterator guestEnd)
{
#ifdef DEBUG
    std::cout << '\n';
    printRange(hostStart, hostEnd, "Host"); std::cout << '\n';
    printRange(guestStart, guestEnd, "Guest"); std::cout << '\n';
#endif // #ifdef DEBUG
    std::vector<Interval> retval;
    retval.reserve(2);

    if(guestStart == guestEnd)
    {
        retval.emplace_back(hostStart, hostEnd);
        hostStart = hostEnd;
        return retval;
    }
    // the guest interval will be "inserted" before hostSplicePoint
    auto hostSplicePoint = findFirstGreaterThan(
            *guestStart, hostStart, hostEnd);
#ifdef DEBUG
    std::cout << "Host splice point = "
        << (hostSplicePoint==hostEnd?
                "end" : std::to_string(*hostSplicePoint))
        << '\n';
#endif // #ifdef DEBUG
    if(hostSplicePoint != hostStart)
    {
        retval.emplace_back(hostStart, hostSplicePoint);
        hostStart = hostSplicePoint;
    }
    if(hostSplicePoint == hostEnd)
    {
        //the entire guest belongs at the end of host
        retval.emplace_back(guestStart, guestEnd);
        guestStart = guestEnd;
#ifdef DEBUG
        std::cout << "Guest interval: all\n";
#endif // #ifdef DEBUG
    }
    else
    {
        /* Now that we know that guestStart belongs just in front of
           hostSplicePoint, let's see how many other elements of
           guest also belong there.
        */
        IntIterator guestIntervalEnd = (guestStart + 1 != guestEnd)?
            findFirstGreaterThan(*hostSplicePoint, guestStart+1, guestEnd) :
            guestEnd;
        retval.emplace_back(guestStart, guestIntervalEnd);
        guestStart = guestIntervalEnd;
#ifdef DEBUG
        std::cout << "guestIntervalEnd = "
            << (guestIntervalEnd == guestEnd?
                    "end" : std::to_string(*guestIntervalEnd))
            << '\n';
#endif // #ifdef DEBUG
    }
#ifdef DEBUG
    std::cout << "Produced: ";
    for(const Interval &i: retval) { std::cout << i << " "; }
    std::cout << '\n';
#endif // #ifdef DEBUG
    return retval;
}


/* Precondition: startIterator < endIterator
*/
IntIterator Solution::findFirstGreaterThan(
        int val,
        IntIterator startIterator,
        IntIterator endIterator)
{
    return findFirstMatchingBinarySearch(
            startIterator, endIterator,
            [val](int crt){ return crt > val;});
}


/* Precondition: startIterator < endIterator
*/
template<typename Predicate>
IntIterator Solution::findFirstMatchingBinarySearch(
        IntIterator startIterator,
        IntIterator endIterator,
        Predicate condition)
{
    // perform a binary search, which is O(log n)
    if(condition(*startIterator))
    {
        return startIterator;
    }
    while(true)
    {
        int size = endIterator - startIterator;
        if(size <= 2)
        {
            return std::find_if(
                    startIterator, endIterator,
                    condition);
        }
        IntIterator median = startIterator + (size>>1);
        if(condition(*median))
        {
            // restrict the search to [start, median]
            endIterator = median+1;
        }
        else
        {
            // restrict the search to (median, end)
            startIterator = median+1;
        }
    }
}


double Solution::findMedian(int totalSize)
{
    int foundBeforeLast = foundSoFar - pLastInterval->size();
    int neededFromLastInterval =
        requiredSortedCount(totalSize) - foundBeforeLast; // can be 1 or 2
    double median;

    if(isOdd(totalSize))
    {
        // The median is a single number which can be found in the last interval
        median = *(pLastInterval->start + neededFromLastInterval -1);
#ifdef DEBUG
        std::cout << "median = "
            << *(pLastInterval->start + neededFromLastInterval -1)
            << '\n';
#endif // #ifdef DEBUG
    }
    else
    {
        /* Even number of elements: the median is the average of 2 values
           which can be either both in the last interval, or one in the
           last interval and one in next-to-last.
        */
#ifdef DEBUG
        std::cout << "neededFromLastInterval: " << neededFromLastInterval << '\n';
        std::cout << "last interval: " << *pLastInterval << '\n';
#endif
        if(neededFromLastInterval == 1)
        {
            median = (*(pLastInterval->start) + lastInPrevInterval) / 2.0;
#ifdef DEBUG
            std::cout << "Found before last: " << foundBeforeLast <<
                ", need 1 more from last\n";
            std::cout << "median = ("
                << *(pLastInterval->start) << " + "
                << lastInPrevInterval
                << ") / 2\n";
#endif // #ifdef DEBUG
        }
        else // neededFromLastInterval == 2
        {
            median = *(pLastInterval->start + neededFromLastInterval -2);
            median += *(pLastInterval->start + neededFromLastInterval -1);
            median /= 2;
#ifdef DEBUG
            std::cout << "median = ("
                << *(pLastInterval->start + neededFromLastInterval -2)
                << " + "
                << *(pLastInterval->start + neededFromLastInterval -1)
                << ") / 2\n";
#endif // #ifdef DEBUG
        }
    }
    return median;
}


double Solution::findMedianSortedArray(const std::vector<int> &nums)
{
    pLastInterval = std::make_unique<Interval>(
            nums.cbegin(), nums.cend());
    foundSoFar = nums.size();
    return findMedian(nums.size());
}
