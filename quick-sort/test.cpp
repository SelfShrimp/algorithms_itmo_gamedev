#include "gtest/gtest.h"
#include <iostream>
#include <windows.h>

template<typename T>
bool comp(const T& a, const T& b)
{
    return a < b;
}

template<typename T, typename Compare>
T* partitionHoare(T* low, T* high, Compare comp)
{
    T pivot = pivotSelection(low, (low + (high - low) / 2), high, comp);
    T* i = low;
    T* j = high;
    while (true)
    {
        while (comp(*i, pivot)) i += 1;
        while (comp(pivot, *j)) j -= 1;
        if (i >= j)
        {
            return j;
        }
        std::swap(*i, *j);
        i += 1;
        j -= 1;
    }
}

template<typename T, typename Compare>
T pivotSelection(T* low, T* middle, T* high, Compare comp)
{
    if (comp(*middle, *low)) std::swap(*low, *middle);
    if (comp(*high, *low)) std::swap(*low, *high);
    if (comp(*high, *middle)) std::swap(*high, *middle);
    return *middle;
}

template<typename T, typename Compare>
void quickSort(T* low, T* high, Compare comp)
{
    while (high > low)
    {
        T* pivot = partitionHoare(low, high, comp);
        if (pivot - low < high - (pivot + 1))
        {
            quickSort(low, pivot, comp);
            low = pivot + 1;
        }
        else
        {
            quickSort(pivot + 1, high, comp);
            high = pivot;
        }
    }
}

template<typename T, typename Compare>
void insertionSort(T* low, T* high, Compare comp)
{
    T key;
    int i, j;
    for (i = 1; i < high - low + 1; i++)
    {
        key = std::move(*(low + i));
        j = i - 1;

        while (j >= 0 && comp(key, *(low + j)))
        {
            *(low + j + 1) = std::move(*(low + j));
            j -= 1;
        }
        *(low + j + 1) = std::move(key);
    }
}

template<typename T, typename Compare>
void sort(T* low, T* high, Compare comp)
{
    if (high - low + 1 < SHIFT_BORDER) insertionSort(low, high, comp);
    else quickSort(low, high, comp);
}

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart) / 1000000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}


TEST(QuickSort, IntQuickSort) {
    int arr[10] = { 10, 5, 9, 6, 3, 4, 1, 8, 2, 7 };
    quickSort(arr, arr + 9, [](int a, int b) { return a < b; });
    for (int i = 0; i < 9; i++) EXPECT_LT(arr[i], arr[i + 1]);
}

TEST(QuickSort, IntInsSort) {
    int arr[10] = { 10, 5, 9, 6, 3, 4, 1, 8, 2, 7 };
    insertionSort(arr, arr + 9, [](int a, int b) { return a < b; });
    for (int i = 0; i < 9; i++) EXPECT_LT(arr[i], arr[i + 1]);
}

TEST(QuickSort, IntQuickSortSingle) {
    int arr[1] = { 10 };
    quickSort(arr, arr, [](int a, int b) { return a < b; });
    EXPECT_EQ(*arr, 10);
}

TEST(QuickSort, IntInsSortSingle) {
    int arr[1] = { 10 };
    insertionSort(arr, arr, [](int a, int b) { return a < b; });
    EXPECT_EQ(*arr, 10);
}

TEST(QuickSort, IntQuickSortEqual) {
    int arr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    quickSort(arr, arr, [](int a, int b) { return a < b; });
    for (int i = 0; i < 9; i++) EXPECT_TRUE(arr[i] <= arr[i + 1]);
}

TEST(QuickSort, IntInsSortEqual) {
    int arr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    insertionSort(arr, arr, [](int a, int b) { return a < b; });
    for (int i = 0; i < 9; i++) EXPECT_TRUE(arr[i], arr[i + 1]);
}

TEST(QuickSort, StrQuickSort) {
    std::string arr[10] = { "Here is 19 symbols!", "But here only 16", "12345678", "", "testtesttest", "12345678", "Yet another string",
                "And another one", "And another one", "Meh..." };
    quickSort(arr, arr + 9, [](std::string a, std::string b) { return a.size() < b.size(); });
    for (int i = 0; i < 9; i++) EXPECT_TRUE(arr[i].size() <= arr[i + 1].size());
    EXPECT_TRUE(arr[9] == "Here is 19 symbols!");
    EXPECT_TRUE(arr[0] == "");
}

TEST(QuickSort, StrInsSort) {
    std::string arr[10] = { "Here is 19 symbols!", "But here only 16", "12345678", "", "testtesttest", "12345678", "Yet another string",
                "And another one", "And another one", "Meh..." };
    insertionSort(arr, arr + 9, [](std::string a, std::string b) { return a.size() < b.size(); });
    for (int i = 0; i < 9; i++) EXPECT_TRUE(arr[i].size() <= arr[i + 1].size());
    EXPECT_TRUE(arr[9] == "Here is 19 symbols!");
    EXPECT_TRUE(arr[0] == "");
}