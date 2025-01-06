#pragma once
#include <memory>
#include <vector>

typedef std::vector<int>::const_iterator IntIterator;


struct Interval
{
    const IntIterator start;
    const IntIterator end;

    Interval(IntIterator s, IntIterator e): start(s), end(e)
    {}

    Interval(const Interval &other): Interval(other.start, other.end)
    {}

    int size() const { return end - start; }
};


class Solution {
    private:
        int foundSoFar = 0;
        int lastInPrevInterval = 0;
        std::unique_ptr<Interval> pLastInterval;

    public:
        double findMedianSortedArrays(
                const std::vector<int> &nums1,
                const std::vector<int> &nums2);

        double findMedianSortedArrays(
                std::vector<int> &&nums1, std::vector<int> &&nums2)
        {
            return findMedianSortedArrays(nums1, nums2);
        }

    private:
        template<typename Predicate>
        IntIterator findFirstMatchingBinarySearch(
                IntIterator startIterator,
                IntIterator endIterator,
                Predicate condition);

        IntIterator findFirstGreaterThan(
                int val,
                IntIterator startIterator,
                IntIterator endIterator);

        void findSortedIntervals(
                const std::vector<int> &nums1,
                const std::vector<int> &nums2);

        std::vector<Interval> buildSortedIntervals(
                IntIterator &hostStart,
                IntIterator hostEnd,
                IntIterator &guestStart,
                IntIterator guestEnd);

        double findMedian(int totalSize);

        int requiredSortedCount(int totalSize);

        double findMedianSortedArray(
                const std::vector<int> &nums);
};

#ifdef DEBUG
#include <iostream>
std::ostream& operator<<(std::ostream& out, const Interval& i);
#endif
