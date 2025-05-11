#include "gtest/gtest.h"
#include <vector>
#include <print>
#include <numeric>
#include <algorithm>
#include <print>

using namespace std;

struct Point {
    int x;
    int y;
};

bool areColinear(
    const Point& p1,
    const Point& p2, 
    const Point& p3
) {
    return (p2.y - p1.y) * (p3.x - p2.x) == (p3.y - p2.y) * (p2.x - p1.x);
}

int Solve(const vector<Point>& values) {
    if (values.size() == 0 || values.size() == 1) {
        return 0;
    }

    if (values.size() == 2) {
        return 2;
    }

    vector<int> areColinearMemory(values.size());
    {
        iota(areColinearMemory.begin(), areColinearMemory.end(), 0);
    }

    {
        size_t i = 0;
        size_t j = 1;
        size_t k = 2;

        while (i < values.size()) {
            if (areColinearMemory[i] == i) {
                while (j < values.size()) {
                    if (areColinearMemory[j] == j) {
                        while (k < values.size()) {
                            if (areColinearMemory[k] == k) {
                                if (areColinear(values[i], values[j], values[k])) {
                                    areColinearMemory[i] = i;
                                    areColinearMemory[j] = i;
                                    areColinearMemory[k] = i;
                                }
                            }

                            ++k;
                        }    
                    }
                    
                    ++j;
                    k = j + 1;
                }
            }

            ++i;
            j = i + 1;
            k = j + 1;
        }
    }

    int count = 1;
    {
        sort(areColinearMemory.begin(), areColinearMemory.end());
        
        for (size_t i = 1; i < areColinearMemory.size(); ++ i) {
            if (areColinearMemory[i - 1] != areColinearMemory[i]) {
                ++count;
            }
        }
    }

    return count;
}

TEST(LaserShooter2d, test) {
    vector<Point> values = {
        { .x = 0, .y = 1 },
        { .x = 2, .y = 3 },
        { .x = 4, .y = 5 },
        { .x = 4, .y = 3 },
    };

    int result = Solve(values);
    println("result: {}", result);
    EXPECT_EQ(result, 2);
}