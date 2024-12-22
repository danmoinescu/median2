#include "median2.hpp"

#include <iostream>
using std::cout;

int main()
{
    Solution s;
    double median = s.findMedianSortedArrays(
            {10, 20, 30, 40, 50},
            {2, 4, 7, 11, 12, 35, 36}
            );
    std::cout << median << "\n";
    return 0;
}
