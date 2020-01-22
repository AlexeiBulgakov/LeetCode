// https://leetcode.com/problems/median-of-two-sorted-arrays/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <tuple>
#include <list>
#include <set>

using namespace std;

class Solution {
public:
    // Решение №1
    double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2) {
        std::multiset<int> setVar;
        std::multiset<int>::iterator midleElement;
        size_t i = 0;
        int element = 0;
        for (auto& nums : std::vector<std::vector<int>>{ std::move(nums1), std::move(nums2) }) {
            while (nums.size()) {
                element = nums.back(); // получаем поcледний элемент
                nums.pop_back(); // удаляем последний элемент

                setVar.emplace(element); // добавляем в дерево новый элемент
                ++i; // фиксируем текущее количество элементов
                if (i > 1) {
                    // смещаем итератор владеющий средним элементом только если:
                    if (i % 2 != 0) { // добавлен не чётный по счёту элемент
                        if (element >= *midleElement) { // добавлен элемент больше предыдущего
                            ++midleElement;
                        }
                    } else { // добавлен чётныё по счёту элемент
                        if (element <= *midleElement) { // добавлен элемент меньше предыдушего
                            --midleElement;
                        }
                    }
                } else if (i == 1) {
                    midleElement = setVar.begin();
                }
            }
        }
        return setVar.size() % 2 != 0
            ? *midleElement
            : static_cast<double>(*midleElement + *++midleElement) / 2;
    }

    // Решение №2: используя только STL
    double findMedianSortedArrays2(vector<int> &nums1, vector<int> &nums2) {
        std::move(nums2.begin(), nums2.end(), std::back_inserter(nums1));
        std::sort(nums1.begin(), nums1.end());
        double res;
        if (size_t size = nums1.size(); size > 1) {
            if (size_t midle = size / 2; size % 2 == 0) {
                res = static_cast<double>(nums1[midle] + nums1[--midle]) / 2;
            } else {
                res = static_cast<double>(nums1[midle]);
            }
        } else {
            res = nums1[0];
        }
        return res;
    }
};

int main() {

    std::vector<std::tuple<uint8_t, std::vector<int>, std::vector<int>, double>> tests {
        { 0, {1,2,3,2,1},     {2,3},              3 },
        { 0, {1,2},           {3,4},              3 },
        { 0, {1,2,3,4,5,6,7}, {2,3},              3.5 },
        { 0, {0,0},           {0,0},              0 },
        { 0, {1,2,3,4,5,6,7}, {2,6,4,5,6,7,8,9},  4 },
        { 1, {1,3},           {2},                2 },
        { 2, {1,3,5,8},       {2},                3 },
        { 3, {1,2,3,4,5},     {6},                3.5 },
        { 4, {},              {6,7,8,9,100},      8 },
        { 5, {1,1},           {1,2},              1 },
    };
    auto run = [](
        double (Solution::*func)(vector<int>&, vector<int>&),
        Solution *solution,
        vector<int> &nums1,
        vector<int> &nums2) -> double {
            return (solution->*func)(nums1, nums2);
    };
    Solution sol{};
    for (auto [testNum, nums1, nums2, result] : tests) {
        if (auto res = run(&Solution::findMedianSortedArrays2, &sol, nums1, nums2); res != result) {
            std::cout << "Test " << testNum << " was failed: " << res << " != " << result << endl;
        }
    }
    return 0;
}