//
// Created by egordm on 20-12-2017.
//

#include "gtest/gtest.h"
#include "../../tech_analysis/math.hpp"

using namespace tech_analysis;

class MathTests : public ::testing::Test {

};

struct TestTT {
    double a;

    TestTT(double a) : a(a) {}
};


TEST_F(MathTests, test_sma) {
    std::vector<TestTT> nodes {
            {10},
            {15},
            {20},
            {25},
            {30},
            {35}
    };
    const auto smaa = SMA(1, 3, nodes.begin(), nodes.end(), &TestTT::a);
    std::stringstream ss;
    for(const auto num : smaa)  ss << "," << num;
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",10,12.5,15,20,25,30"); // TODO: whatever
}

TEST_F(MathTests, test_ema) {
    std::vector<TestTT> nodes {
            {10},
            {15},
            {20},
            {25},
            {30},
            {35}
    };
    const auto sma = EMA(1, 3, nodes.begin(), nodes.end(), &TestTT::a);
    std::stringstream ss;
    for(const auto num : sma)  ss << "," << num;
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",10,12.5,16.25,20.625,25.3125,30.1562"); // TODO: whatever
}


TEST_F(MathTests, test_boll) {
    std::vector<TestTT> nodes {
            {10},
            {15},
            {20},
            {25},
            {30},
            {35}
    };
    const auto sma = BOLL(1, 3, nodes.begin(), nodes.end(), &TestTT::a);
    std::stringstream ss;
    for(const auto num : sma)  ss << ",(" << std::get<0>(num) << "|" << std::get<1>(num) << "|" << std::get<2>(num) << ")";
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",10,12.5,16.25,20.625,25.3125,30.1562"); // TODO: whatever
}


