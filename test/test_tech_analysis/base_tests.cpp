//
// Created by egordm on 20-12-2017.
//

#include "gtest/gtest.h"
#include "../../tech_analysis/candle_math.hpp"

class MathTests : public ::testing::Test {

};

struct TestTT {
    double a;
    double b;
    double c;
    double d;

    TestTT(double a) : a(a) {}

    TestTT(double a, double b, double c) : a(a), b(b), c(c) {}

    TestTT(double a, double b, double c, double d) : a(a), b(b), c(c), d(d) {}
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

    SMAIndicator indicator(1,3);
    indicator.feed(nodes.begin(), nodes.end(), &TestTT::a);
    const auto res = indicator.get_data();

    std::stringstream ss;
    for(const auto num : res)  ss << "," << num;
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",20,25,30"); // TODO: whatever
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
    EMAIndicator indicator(1,3);
    indicator.feed(nodes.begin(), nodes.end(), &TestTT::a);
    const auto res = indicator.get_data();

    std::stringstream ss;
    for(const auto num : res)  ss << "," << num;
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",20.625,25.3125,30.1562"); // TODO: whatever
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
    BollIndicator indicator(1,3);
    indicator.feed(nodes.begin(), nodes.end(), &TestTT::a);
    const auto res = indicator.get_data();

    std::stringstream ss;
    for(const auto num : res)  ss << ",(" << std::get<0>(num) << "|" << std::get<1>(num) << "|" << std::get<2>(num) << ")";
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",(10|20|30),(15|25|35),(20|30|40)"); // TODO: whatever
}


TEST_F(MathTests, test_macd) {
    std::vector<TestTT> nodes {
            {10},
            {15},
            {20},
            {25},
            {30},
            {35}
    };
    MACDIndicator indicator(1,3);
    indicator.feed(nodes.begin(), nodes.end(), &TestTT::a);
    const auto res = indicator.get_data();

    std::stringstream ss;
    for(const auto num : res)  ss << ",(" << std::get<0>(num) << "|" << std::get<1>(num) << "|" << std::get<2>(num) << ")";
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",(3.56961|2.98631|0.583293),(4.55865|4.03454|0.524111),(5.33207|4.89956|0.432511)"); // TODO: whatever
}

TEST_F(MathTests, test_rsi) {
    std::vector<TestTT> nodes {
            {10},
            {12},
            {10},
            {25},
            {20},
            {15},
            {100},
            {80},
            {70}
    };
    RSIIndicator indicator(1,3);
    indicator.feed(nodes.begin(), nodes.end(), &TestTT::a);
    const auto res = indicator.get_data();

    std::stringstream ss;
    for(const auto num : res)  ss << "," << num;
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",92.9825,66.6667,46.7991,93.8134,71.5096,60.6883"); // TODO: whatever
}



TEST_F(MathTests, test_cci) {
    std::vector<TestTT> nodes {
            {14, 10, 12},
            {16, 12, 14},
            {19, 18, 18.3},
            {14, 12, 11},
            {12, 9, 10},
            {9, 4, 5},
            {20, 4, 19},
            {22, 19, 21},
    };
    CCIIndicator indicator(1,3);
    indicator.feed(nodes.begin(), nodes.end(), &TestTT::a, &TestTT::c, &TestTT::b);
    const auto res = indicator.get_data();

    std::stringstream ss;
    for(const auto num : res)  ss << "," << num;
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",-38.49,-58.1914,-71.4815,-38.49,76.98"); // TODO: whatever
}


TEST_F(MathTests, test_cmo) {
    std::vector<TestTT> nodes {
            {10},
            {12},
            {10},
            {25},
            {20},
            {15},
            {100},
            {80},
            {70}
    };
    CMOIndicator indicator(1,3);
    indicator.feed(nodes.begin(), nodes.end(), &TestTT::a);
    const auto res = indicator.get_data();

    std::stringstream ss;
    for(const auto num : res)  ss << "," << num;
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",78.9474,41.6667,17.2414,78.9474,54.5455,48.9051"); // TODO: whatever
}

TEST_F(MathTests, test_mfr) {
    std::vector<TestTT> nodes {
            {14, 10, 12, 10},
            {16, 12, 14, 12},
            {19, 18, 18.3, 14},
            {14, 12, 11, 40},
            {12, 9, 10, 90},
            {9, 4, 5, 70},
            {20, 4, 19, 40},
            {22, 19, 21, 60},
    };
    MFIIndicator indicator(1,3);
    indicator.feed(nodes.begin(), nodes.end(), &TestTT::a,  &TestTT::b,  &TestTT::c, &TestTT::d);
    const auto res = indicator.get_data();

    std::stringstream ss;
    for(const auto num : res)  ss << "," << num;
    std::cout << std::endl << ss.str() << std::endl;
    EXPECT_EQ(ss.str(), ",49.0859,24.0449,20.0643,41.201,57.3899"); // TODO: whatever
}



/*
TEST_F(MathTests, test_cci) {

}*/
