#include "gtest/gtest.h"
#include <coffee_machine/moving_average.h>

class TestMovingAverage : public ::testing::Test
{
};

TEST_F(TestMovingAverage, testMovingAverageWithEmptyQueue)
{
    ASSERT_EQ(MovingAverage<int>(100).get(), 0);
    ASSERT_EQ(MovingAverage<int>(10).get(), 0);
    ASSERT_EQ(MovingAverage<int>(1).get(), 0);
    // If window size is 0, it is set to 1
    ASSERT_EQ(MovingAverage<int>(0).get(), 0);

    // Same test with the default constructor
    ASSERT_EQ(MovingAverage<int>().get(), 0);
}

TEST_F(TestMovingAverage, testMovingAverageWithNonFullHistory)
{
    // Test that when the history is not full, the empty values are not considered
    // in the average computation

    auto avg = MovingAverage<int>(5);
    ASSERT_EQ(avg.get(), 0);

    avg.add(10);
    ASSERT_EQ(avg.get(), 10);

    avg.add(20);
    ASSERT_EQ(avg.get(), 15);

    avg.add(30);
    ASSERT_EQ(avg.get(), 20);

    avg.add(40);
    ASSERT_EQ(avg.get(), 25);

    avg.add(50);
    ASSERT_EQ(avg.get(), 30);
}

TEST_F(TestMovingAverage, testReset)
{
    auto avg = MovingAverage<int>(5);
    avg.add(10);
    avg.add(20);
    avg.add(30);
    avg.add(40);
    avg.add(50);
    ASSERT_EQ(avg.get(), 30);
    avg.reset();
    ASSERT_EQ(avg.get(), 0);
}

TEST_F(TestMovingAverage, testHistoryisOverwrittenWhenFull)
{
    auto avg = MovingAverage<int>(5);
    avg.add(10);
    avg.add(20);
    avg.add(30);
    avg.add(40);
    avg.add(50);
    ASSERT_EQ(avg.get(), 30);

    avg.add(100);
    // expect average of 100 20 30 40 50
    ASSERT_EQ(avg.get(), 48);
    avg.add(200);
    // expect average of 100 200 30 40 50
    ASSERT_EQ(avg.get(), 84);
    avg.add(300);
    // expect average of 100 200 300 40 50
    ASSERT_EQ(avg.get(), 138);
    avg.add(400);
    // expect average of 100 200 300 400 50
    ASSERT_EQ(avg.get(), 210);
    avg.add(500);
    // expect average of 100 200 300 400 500
    ASSERT_EQ(avg.get(), 300);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}