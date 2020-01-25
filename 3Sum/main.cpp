// https://leetcode.com/problems/3sum/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <variant>
#include <vector>
#include <tuple>
#include <list>
#include <set>
#include <map>

#include <omp.h>

using namespace std;

/*
 * Вариант решения №1:
 * Особенности:
 * - не проходит проверку на letcode т.к. превышает отпущенный лимит времени вычислений;
 * - сложность O(n^3).
 */
class Solution1 {
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
            int p = std::distance(nums.begin(), firstPositive); // индекс первого положительного элемента массива
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


/*
 * Вариант решения №2:
 * Особенности:
 * - данный вариант многопоточный; однако letcode не поддерживает расспараллеливание средствами OpenMP;
 * - сложность в однопоточном варианте 3*O(n^2), а в многопоточном O(n^2).
 */
class Solution2 {
public:
    vector<vector<int>> threeSum(vector<int> &nums) {
        std::vector<std::vector<int>> res;
        if (size_t size = nums.size(); size >= 3) {
            std::set<std::multiset<int>> checked; // хранит и упорядочивает уникальные удовлетворительные результаты поиска

            std::map<int, int> zeros{};
            std::map<int, int, std::less<int>> negative{};
            std::map<int, int, std::greater<int>> positive{};

            while (nums.size()) {
                if (auto num = nums.back(); num > 0) {
                    ++positive[num];
                } else if (num < 0) {
                    ++negative[num];
                } else {
                    ++zeros[num];
                }
                nums.pop_back();
            }

            const auto
                endPositive = positive.end(),
                endNegative = negative.end();

            bool is2Multiply;
            int halfKey1, key1, key2, key3;
        #pragma omp parallel sections private(halfKey1, key1, key2, key3, is2Multiply)
            {
                // Поиск удовлетворяющих триплетов типа {-X1, +X2, +X3} и {-X1, +X2, +X2}
            #pragma omp section
                {
                    printf("[%d] Finding of correct triplets like {-X1, +X2, +X3} и {-X1, +X2, +X2}\n", omp_get_thread_num());
                    for (const auto &value : negative) {
                        key1 = value.first;
                        key2 = -key1 - 1;
                        key3 = 1;
                        is2Multiply = (key1 & 1 == 1) ? false : true;
                        halfKey1 = -key1 / 2;
                        for (int i = 0; i < (!is2Multiply ? halfKey1 : halfKey1 - 1); ++i) {
                            if (positive.find(key2) != endPositive && positive.find(key3) != endPositive) {
                                checked.emplace(std::multiset<int>{key1, key2, key3});
                            }
                            --key2;
                            ++key3;
                        }

                        if (is2Multiply) {
                            if (const auto &value = positive.find(halfKey1); value != endPositive && value->second >= 2) {
                                checked.emplace(std::multiset<int>{key1, halfKey1, halfKey1});
                            }
                        }
                    }
                } // opm section

                // Поиск удовлетворяющих триплетов типа {-X1, -X2, +X3} и {-X1, -X1, +X2}
            #pragma omp section
                {
                    printf("[%d] Finding of correct triplets like {-X1, -X2, +X3} и {-X1, -X1, +X2}\n", omp_get_thread_num());
                    for (const auto &value : positive) {
                        key1 = value.first;
                        key2 = -(key1 - 1);
                        key3 = -1;
                        is2Multiply = (key1 & 1 == 1) ? false : true;
                        halfKey1 = key1 / 2;
                        for (int i = 0; i < (!is2Multiply ? halfKey1 : halfKey1 - 1); ++i) {
                            if (negative.find(key2) != endNegative && negative.find(key3) != endNegative) {
                                checked.emplace(std::multiset<int>{key1, key2, key3});
                            }
                            ++key2;
                            --key3;
                        }

                        if (is2Multiply) {
                            if (const auto &value = negative.find(-halfKey1); value != endNegative && value->second >= 2) {
                                checked.emplace(std::multiset<int>{-halfKey1, -halfKey1, key1});
                            }
                        }
                    }
                } // omp section

                // Поиск удовлетворяющих триплетов типа {-X1, 0, +X1} и {0, 0, 0}
            #pragma omp section
                {
                    printf("[%d] Finding of correct triplets like {-X1, 0, +X1} и {0, 0, 0}\n", omp_get_thread_num());
                    if (size_t size = zeros.size(); size) {
                        if (zeros[0] >= 1) {
                            for (const auto &[key1, _] : positive) {
                                if (negative.find(-key1) != endNegative) {
                                    checked.emplace(std::multiset<int>{-key1, 0, key1});
                                }
                            }
                        }

                        if (zeros[0] >= 3) {
                            checked.emplace(std::multiset<int>{0, 0, 0});
                        }
                    }
                } // omp section
            } // omp parallel sections

            while (checked.size()) {
                auto x = checked.extract(*checked.begin()).value();
                res.emplace_back(std::vector<int>(x.begin(), x.end()));
            }
        }
        return res;
    }
};

int main() {
    Solution2().threeSum(std::vector<int>{ -4, -2, 1, -5, -4, -4, 4, -2, 0, 4, 0, 2, 2, -2, 3, 1, -5, 0, 5 });
    return 0;
}