#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Engine/Common/SmallVector.h"

namespace {
    using namespace Engine;

    TEST(SmallVector, emplace_back_and_access_to_fit) {
        SmallVector<int, 3> vector;
        vector.push_back(2);
        ASSERT_EQ(vector[0], 2);
        vector.push_back(3);
        vector.push_back(4);
        ASSERT_EQ(vector[2], 4);
        ASSERT_EQ(vector[1], 3);
    }

    TEST(SmallVector, emplace_back_and_access_to_reallocate) {
        SmallVector<int, 3> vector;
        vector.push_back(2);
        ASSERT_EQ(vector[0], 2);
        vector.push_back(3);
        vector.push_back(4);
        ASSERT_EQ(vector[2], 4);
        ASSERT_EQ(vector[1], 3);
        vector.push_back(7);
        vector.push_back(5);
        ASSERT_EQ(vector[2], 4);
        ASSERT_EQ(vector[1], 3);
        ASSERT_EQ(vector[3], 7);
        ASSERT_EQ(vector[4], 5);
        ASSERT_EQ(vector[3], 7);
    }

    TEST(SmallVector, size_stack) {
        SmallVector<int, 3> vector;
        ASSERT_EQ(vector.size(), 0);
        for (int i = 0; i < 2; ++i)
            vector.push_back(i);
        ASSERT_EQ(vector.size(), 2);
    }

    TEST(SmallVector, size_heap) {
        SmallVector<int, 3> vector;
        ASSERT_EQ(vector.size(), 0);
        for (int i = 0; i < 5; ++i)
            vector.push_back(i);
        ASSERT_EQ(vector.size(), 5);
    }

    TEST(SmallVector, to_std_vector_conversion_stack) {
        SmallVector<int, 3> vector;
        ASSERT_EQ(vector.size(), 0);
        for (int i = 0; i < 2; ++i)
            vector.push_back(i);
        auto vec = (std::vector<int>)vector;

        ASSERT_EQ(vector.size(), 2);
        //checking for not breaking SmallVector
        for (int i = 0; i < 2; ++i)
            ASSERT_EQ(vector[i], i);

        ASSERT_EQ(vec.size(), 2);
        //checking for getting proper result
        for (int i = 0; i < 2; ++i)
            ASSERT_EQ(vec[i], i);
    }

    TEST(SmallVector, to_std_vector_conversion_heap) {
        SmallVector<int, 3> vector;
        ASSERT_EQ(vector.size(), 0);
        for (int i = 0; i < 5; ++i)
            vector.push_back(i);
        auto vec = (std::vector<int>)vector;

        //checking for not breaking SmallVector
        ASSERT_EQ(vector.size(), 5);
        for (int i = 0; i < 5; ++i)
            ASSERT_EQ(vector[i], i);

        //checking for getting proper result
        ASSERT_EQ(vec.size(), 5);
        for (int i = 0; i < 5; ++i)
            ASSERT_EQ(vec[i], i);
    }

    TEST(SmallVector, iterators_stack) {
        SmallVector<int, 4> vector;
        vector.push_back(2);
        vector.push_back(1);
        vector.push_back(3);

        std::vector<int> vec;
        for (int v : vector)
            vec.push_back(v);

        auto it = vector.begin();
        ASSERT_EQ(*it, 2);
        ASSERT_EQ(*(it + 2), 3);
        ASSERT_EQ(*(it++), 2);
        ASSERT_EQ(*it, 1);

        ASSERT_EQ((std::vector<int>)vector, vec);
    }

    TEST(SmallVector, iterators_heap) {
        SmallVector<int, 1> vector;
        vector.push_back(2);
        vector.push_back(1);

        std::vector<int> vec;
        for (int v : vector)
            vec.push_back(v);

        auto it = vector.begin();
        ASSERT_EQ(*it, 2);
        ASSERT_EQ(*(it++), 2);
        ASSERT_EQ(*it, 1);

        ASSERT_EQ((std::vector<int>)vector, vec);
    }
}