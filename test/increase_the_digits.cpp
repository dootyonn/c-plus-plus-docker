#include "gtest/gtest.h"
#include <vector>
#include <print>
#include <math.h>

using namespace std;

vector<int> calcDiff(const vector<int>& input, const vector<int>& target) {
    vector<int> result(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == target[i]) {
            result[i] = 0;
        }
        else if (input[i] < target[i]) {
            result[i] = target[i] - input[i];
        }
        else {
            result[i] = 10 - input[i] + target[i];
        }
    }

    return result;
}

int findMinIndex(const vector<int>& values, int start, int end) {
    assert(end > start);
    int minIndex = start;
    for (int i = start + 1; i < end; ++i) {
        if (values[i] < values[minIndex]) {
            minIndex = i;
        }
    }

    return minIndex;
}

int SolveImpl(
    int start, 
    int end,
    const vector<int>& input, 
    const vector<int>& target
){
    assert(end >= start);
    
    if (start < 0 || end > input.size()) {
        return -1;
    }

    int length = end - start;
    if (length == 0) {
        return 0;
    }

    if (length == 1) {
        if (input[start] == target[start]) {
            return 0;
        }
        return -1;
    }

    auto diff = calcDiff(input, target);

    if (length == 2) {
        if (diff[start] == diff[start + 1]) {
            return diff[start];
        }
        return -1;
    }
    
    int minIndex = findMinIndex(diff, start, end);
    int increment = diff[minIndex];

    int value1 = -1;
    int value2 = -1;

    if ((minIndex - 1 ) > start) {
        vector<int> newInput = input;
        newInput[minIndex - 1] = (newInput[minIndex - 1] + increment) % 10;
        newInput[minIndex] = (newInput[minIndex] + increment) % 10;

        int left = SolveImpl(start, minIndex, newInput, target);
        int right = SolveImpl(minIndex + 1, end, newInput, target);

        if (left == -1 || right == -1) {
            value1 = -1;
        }
        else {
            value1 = increment + left + right;
        }
    }

    if ((minIndex + 1) < end) {
        vector<int> newInput = input;
        newInput[minIndex] = (newInput[minIndex] + increment) % 10; 
        newInput[minIndex + 1] = (newInput[minIndex + 1] + increment) % 10;

        int left = SolveImpl(start, minIndex, newInput, target);
        int right = SolveImpl(minIndex + 1, end, newInput, target);

        if (left == -1 || right == -1) {
            value2 = -1;
        }
        else {
            value2 = increment + left + right;
        }
    }

    if (value1 != -1 && value2 != -1) {
        return min(value1, value2);
    }
    if (value1 != -1) {
        return value1;
    }
    if (value2 != -1) {
        return value2;
    }
    return -1;
}

int Solve(const vector<int>& input, const vector<int>& target) {
    assert(input.size() == target.size());
    return SolveImpl(0, input.size(), input, target);
}


TEST(IncreaseTheDigits, oneElement) {
    vector<int> input = { 1 };
    vector<int> target = { 5 };

    int result = Solve(input, target);
    EXPECT_EQ(result, -1);
}

TEST(IncreaseTheDigits, twoElementSuccessful) {
    vector<int> input = { 3, 3 };
    vector<int> target = { 5, 5 };

    int result = Solve(input, target);
    EXPECT_EQ(result, 2);
}

TEST(IncreaseTheDigits, twoElementFail) {
    vector<int> input = { 3, 3 };
    vector<int> target = { 5, 4 };

    int result = Solve(input, target);
    EXPECT_EQ(result, -1);
}

TEST(IncreaseTheDigits, threeElementFail) {
    vector<int> input = { 3, 3, 3};
    vector<int> target = { 4, 4, 4 };

    int result = Solve(input, target);
    EXPECT_EQ(result, -1);
}

TEST(IncreaseTheDigits, fiveElement) {
    vector<int> input = { 1, 3, 4, 7, 1 };
    vector<int> target = { 5, 9, 6, 0, 4 };

    int result = Solve(input, target);
    EXPECT_EQ(result, 9);
}

TEST(IncreaseTheDigits, sixElementSucc) {
    vector<int> input = { 1, 3, 4, 7, 1, 1 };
    vector<int> target = { 5, 9, 6, 0, 4, 1 };

    int result = Solve(input, target);
    EXPECT_EQ(result, 9);
}

TEST(IncreaseTheDigits, sixElementFail) {
    vector<int> input = { 1, 3, 4, 7, 1, 1 };
    vector<int> target = { 5, 9, 6, 0, 4, 8 };

    int result = Solve(input, target);
    EXPECT_EQ(result, -1);
}