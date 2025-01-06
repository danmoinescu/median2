#include "median2.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>


static void usage(const char *me)
{
    std::cerr << "Usage: " << me << " a1[,a2,a3...] b1[,b2,b3...]\n";
}


static std::vector<int> parseNumbers(const char *allNumbers)
{
    std::vector<int> retval;
    std::stringstream ss(allNumbers);
    std::string numberStr;
    while(std::getline(ss, numberStr, ','))
    {
        char *end;
        int number = (int) strtol(numberStr.c_str(), &end, 10);
        if(*end != 0)
        {
            throw std::invalid_argument(std::string("Invalid character: ") + *end);
        }
        retval.push_back(number);
    }
    return retval;
}


int main(int argc, const char *argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        return 1;
    }

    std::vector<int> v1,v2;
    try
    {
        v1 = parseNumbers(argv[1]);
        v2 = parseNumbers(argv[2]);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        usage(argv[0]);
        return 1;
    }
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    Solution s;
    double median = s.findMedianSortedArrays(v1, v2);
    std::cout << median << '\n';
    return 0;
}
