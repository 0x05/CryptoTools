//
// Created by egordm on 12-12-2017.
//

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../../whale_api/poloniex_api.hpp"
#include "../../whale_api/temp_keys.hpp"

class PoloniexAPIFixture : public ::testing::Test {
protected:
    api::PoloniexAPI *api_interface;

    void SetUp() override {
        api_interface = new api::PoloniexAPI(POLONIEX_KEY);
    }

    void TearDown() override {
        delete api_interface;
    }

    const api::Response TestRequest(api::Request request) {
        std::cout << std::endl << "Requesting: " << request.get_url() << std::endl;
        const auto &response = request.request();
        std::cout << std::endl<< "Succesful " << response.is_succesful() << "; Response: " << std::endl
                  << response.response << std::endl << std::endl;
        EXPECT_EQ(response.is_succesful(), true);
        return response;
    }
};


TEST_F(PoloniexAPIFixture, ticker) {
    const auto &response = TestRequest(api_interface->get_ticker());
    EXPECT_THAT(response.response, testing::HasSubstr(R"("last":)"));
}

TEST_F(PoloniexAPIFixture, order_book) {
    const auto &response = TestRequest(api_interface->get_order_book("BTC_BCN"));
    EXPECT_THAT(response.response, testing::HasSubstr(R"("asks":)"));
}

TEST_F(PoloniexAPIFixture, chart_data) {
    const auto &response = TestRequest(api_interface->get_chart_data("BTC_BCN", 1800, std::time(nullptr) - 1800 * 200, std::time(nullptr)));
    EXPECT_THAT(response.response, testing::HasSubstr(R"("date":)"));
}