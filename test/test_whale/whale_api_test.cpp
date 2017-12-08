//
// Created by egordm on 5-12-2017.
//

#include <gmock/gmock-matchers.h>
#include "gtest/gtest.h"
#include "../../whale_api/whale_api.hpp"

class WhaleAPIFixture : public ::testing::Test {
protected:
    api::WhaleAPI *api_interface;

    void SetUp() override {
        api_interface = new api::WhaleAPI(WHALE_CLUB_KEY);
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


TEST_F(WhaleAPIFixture, markets) {
    const auto &response = TestRequest(api_interface->get_markets());
    EXPECT_THAT(response.response, testing::HasSubstr(R"("display_name":)"));
}

TEST_F(WhaleAPIFixture, market_btc) {
    const auto &response = TestRequest(api_interface->get_markets({"BTC-USD"}));
    EXPECT_THAT(response.response, testing::HasSubstr(R"("display_name":)"));
}

TEST_F(WhaleAPIFixture, price) {
    const auto &response = TestRequest(api_interface->get_price({"BTC-USD"}));
    EXPECT_THAT(response.response, testing::HasSubstr(R"("BTC-USD":)"));
}

TEST_F(WhaleAPIFixture, balance) {
    const auto &response = TestRequest(api_interface->get_balance());
    EXPECT_THAT(response.response, testing::HasSubstr(R"("available_amount":)"));
}

TEST_F(WhaleAPIFixture, transactions) {
    const auto &response = TestRequest(api_interface->get_tranactions(api::WhaleAPI::TYPE_BONUSSES, 10));
    // TODO: check if json array?
}

TEST_F(WhaleAPIFixture, new_order) {
    const auto &response = TestRequest(api_interface->new_order(api::WhaleAPI::DIRECTION_SELL, "BTC-USD", 2000000, 18000));
    EXPECT_THAT(response.response, testing::HasSubstr(R"("id":)"));
}

TEST_F(WhaleAPIFixture, get_order) {
    const auto &response = TestRequest(api_interface->get_order("GbQaHMEh5JSS9K6Rq"));
    EXPECT_THAT(response.response, testing::HasSubstr(R"("id":)"));
}

TEST_F(WhaleAPIFixture, get_orders) {
    const auto &response = TestRequest(api_interface->get_orders(api::WhaleAPI::STATE_ACTIVE));
    // TODO: check if json array?
}

// TODO: create orders before trying?
TEST_F(WhaleAPIFixture, cancel_order) {
    const auto &response = TestRequest(api_interface->cancel_order("EQDKWuj8oiZ96PgA7"));
    EXPECT_THAT(response.response, testing::HasSubstr(R"("cancelled": true)"));
}

TEST_F(WhaleAPIFixture, close_order) {
    const auto &response = TestRequest(api_interface->close_order("EQDKWuj8oiZ96PgA7"));
    EXPECT_THAT(response.response, testing::HasSubstr(R"("id":)"));
}
