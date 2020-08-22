#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Engine/Physics/AABBTree.h"

namespace {
    using namespace Engine::Physics;

    void insertAndQuery(AABBTree &tree) {
        int a, b, c, d;
        tree.insert(&a, {{0, 0}, {2, 1}});
        tree.insert(&b, {{3, 0}, {1, 2}});
        tree.insert(&c, {{1, 2}, {2, 2}});
        tree.insert(&d, {{4, 3}, {2, 1}});

        std::vector<void *> expectedRes {&b, &c};
        sort(expectedRes.begin(), expectedRes.end());

        auto res = (std::vector<void *>)tree.query({{2, 1}, {2, 2}});
        sort(res.begin(), res.end());

        EXPECT_EQ(expectedRes, res);
    }

    TEST(AABBTree, insert_and_query) {
        AABBTree tree;
        insertAndQuery(tree);
    }

    TEST(AABBTree, insert_and_query_clear_and_again) {
        AABBTree tree;
        insertAndQuery(tree);
        tree.clear();
        insertAndQuery(tree);
    }

    TEST(AABBTree, insert_query_erase_query) {
        AABBTree tree;
        int a, b, c, d;
        tree.insert(&a, {{0, 0}, {2, 1}});
        tree.insert(&b, {{3, 0}, {1, 2}});
        tree.insert(&c, {{1, 2}, {2, 2}});
        tree.insert(&d, {{4, 3}, {2, 1}});

        std::vector<void *> expectedRes1 {&b, &c};
        sort(expectedRes1.begin(), expectedRes1.end());

        auto res1 = (std::vector<void *>)tree.query({{2, 1}, {2, 2}});
        sort(res1.begin(), res1.end());

        EXPECT_EQ(expectedRes1, res1);

        tree.erase(&c, {{1, 2}, {2, 2}});

        std::vector<void *> expectedRes2 {&b};
        auto res2 = (std::vector<void *>)tree.query({{2, 1}, {2, 2}});

        EXPECT_EQ(expectedRes2, res2);
    }

}