#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Engine/Common/HashMap.h"

#include <algorithm>
#include <vector>
#include <utility>
#include <random>

namespace {
    using namespace Engine;

    ///TODO: Better tests

    TEST(HashMap, insert_and_access) {
        std::random_device rd;
        std::mt19937 g(rd());

        std::vector<std::pair<int, int>> vec(100000);
        int prevA = 0;
        for (auto&[a, b] : vec) {
            //to make them unique
            a = prevA + g() % 100 + 1;
            prevA = a;
            b = g() % 100;
        }
        std::shuffle(vec.begin(), vec.end(), g);

        HashMap<int, int> map;
        for (auto[a, b] : vec)
            map[a] = b;

        std::shuffle(vec.begin(), vec.end(), g);

        for (auto[a, b] : vec)
            ASSERT_EQ(map[a], b);
    }
}