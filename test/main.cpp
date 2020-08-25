#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Engine/Common/SmallVectorTest.h"
#include "Engine/Common/HashMapTest.h"
#include "Engine/Physics/AABBTreeTest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}