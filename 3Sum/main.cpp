// https://leetcode.com/problems/3sum/

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
    vector<vector<int>> threeSum(vector<int> &nums) {
        vector<vector<int>> res;
        if (int size = nums.size(); size >= 3) {
            set<multiset<int>> checked; // хранит и упорядочивает уникальные удовлетворительные результаты поиска
            auto check = [&](int i, int j, int k) {
                if ((nums[i] + nums[j] + nums[k]) == 0) {
                    checked.emplace(std::multiset<int>{nums[i], nums[j], nums[k]});
                }
            };
            auto firstPositive = std::stable_partition(nums.begin(), nums.end(), [](int x) {return x < 0; });
            int p  = std::distance(nums.begin(), firstPositive); // индекс первого положительного элемента массива
            if (p == 0) {
                if (nums[0] == 0 && nums[1] == 0 && nums[2] == 0) {
                    res.emplace_back(std::vector<int>{0, 0, 0});
                }
                return res;
            }
            std::sort(nums.begin(), firstPositive, [](int x, int y) { return x < y; });
            std::sort(firstPositive, nums.end(), [](int x, int y) { return x > y; });
            if (nums[size - 3] == 0) {
                checked.emplace(std::multiset<int>{0, 0, 0});
            }
            /*
             * TODO: расспаралелить по первому циклу for.
             * Замечание: уникальный доступ к рессурсу "checked"
             */
            for (int i = 0; i < size - 2; ++i) {
                for (int j = i + 1; j < size - 1; ++j) {
                    for (int k = j + 1; k < size; ++k) {
                        if (i < p) {
                            if (j < p) {
                                if (k < p) {
                                    k = p - 1; // -1 т.к. на следующей итерации цикла значение k инкрементируется
                                    goto next_k;
                                } else {
                                    if (-(nums[i] + nums[j]) > nums[k]) {
                                        goto next_j;
                                    } else {
                                        check(i, j, k);
                                        goto next_k;
                                    }
                                }
                            } else {
                                if (-nums[i] > (nums[j] + nums[k])) {
                                    goto next_j;
                                } else {
                                    check(i, j, k);
                                    goto next_k;
                                }
                            }
                        } else {
                            goto end;
                        }
                        next_k:
                        continue;
                    }
                    next_j:
                    continue;
                }
            }
            end:
            for (auto &v : checked) {
                res.emplace_back(std::vector<int>(v.begin(), v.end()));
            }
        }
        return res;
    }
};

int main() {
    Solution().threeSum(std::vector<int>{ -4, -2, 1, -5, -4, -4, 4, -2, 0, 4, 0, -2, 3, 1, -5, 0 });
    return 0;
}