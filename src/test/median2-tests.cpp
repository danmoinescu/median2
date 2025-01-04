#include <iostream>
#include <iterator>
#include <algorithm>
#include <random>
#include "median2.hpp"

#define TEST_PASSED 0
#define TEST_FAILED -1

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
{
    out << "[ ";
    std::copy(v.cbegin(), v.cend(), std::ostream_iterator<T>(out, " "));
    out << "]";
    return out;
}


static std::vector<int> random_sorted_vector(
        int size, int starting_point=-1, int max_increment=10)
{
    std::vector<int> retval;
    retval.reserve(size);

    std::minstd_rand rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, max_increment+1);

    int val = starting_point<=0? dist(rng) : starting_point;
    for(int i = 0; i<size; ++i)
    {
        val += dist(rng);
        retval.push_back(val);
    }

    return retval;
}


static bool basic_test(
        const std::vector<int> &v1,
        const std::vector<int> &v2)
{

    auto v12 = v1;
    v12.insert(v12.end(), v2.begin(), v2.end());
    std::sort(v12.begin(), v12.end());
    int size = v12.size();
    int half = size/2;
    double expected = ((size & 1) == 0)?
        (v12[half-1] + v12[half])/2.0 : v12[half];

    Solution s;
    double actual = s.findMedianSortedArrays(v1, v2);

#ifdef DEBUG
    std::cout << "Sorted combined sequence: " << v12 << '\n';
    if((size & 1) == 0)
    {
        std::cout << "Even size: " << size << '\n';
        std::cout << "Median from "
            << v12[half-1] << ", " << v12[half] << '\n';
    }
    else
    {
        std::cout << "Odd size: " << size << '\n';
        std::cout << "Median: " << v12[half] << '\n';
    }
#endif

    if(actual != expected)
    {
        std::cerr << "Test failed for " << v1 << " and " << v2 << '\n';
        std::cerr << "Expected " << expected << ", got " << actual << '\n';
        return false;
    }
    else
    {
        return true;
    }
}


static bool basic_test(
        std::vector<int> &&v1,
        std::vector<int> &&v2)
{
    return basic_test(v1, v2);
}


int repeated_tests_random_increasing_sizes()
{
    const int max_size = 25;
    for(int size1 = 1; size1 <= max_size; ++size1)
    {
        for(int size2 = size1; size2 <= max_size; ++size2)
        {
            for(int i=50; i; --i)
            {
                auto v1 = random_sorted_vector(size1);
                auto v2 = random_sorted_vector(size2);

                if(!basic_test(v1, v2)) { return TEST_FAILED; }
            }
        }
    }
    return TEST_PASSED;
}


int test_odd_total()
{
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2 = {2, 5, 5, 6};
    return basic_test(v1, v2)? TEST_PASSED : TEST_FAILED;
}


int test_even_total()
{
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2 = {2, 5, 5, 6, 7};
    return basic_test(v1, v2)? TEST_PASSED : TEST_FAILED;
}


int test_1_smallest()
{
    return basic_test({1}, {2, 5, 6, 7})?  TEST_PASSED : TEST_FAILED;
}


int test_1_largest()
{
    return basic_test({9}, {2, 5, 7})?  TEST_PASSED : TEST_FAILED;
}


int test_1_middle()
{
    return basic_test({6}, {2, 5, 7, 8})?  TEST_PASSED : TEST_FAILED;
}


int repeated_tests_1_vs_empty()
{
    const std::vector<int> empty;
    for(int size = 1; size <= 20; ++size)
    {
        for(int i=50; i; --i)
        {
            auto v1 = random_sorted_vector(size);

            if(!basic_test(v1, empty)) { return TEST_FAILED; }
        }
    }
    return TEST_PASSED;
}


int repeated_tests_empty_vs_2()
{
    const std::vector<int> empty;
    for(int size = 1; size <= 20; ++size)
    {
        for(int i=50; i; --i)
        {
            auto v2 = random_sorted_vector(size);

            if(!basic_test(empty, v2)) { return TEST_FAILED; }
        }
    }
    return TEST_PASSED;
}
