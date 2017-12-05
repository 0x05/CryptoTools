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
};


TEST_F(WhaleAPIFixture, test_market) {
    const auto &response = api_interface->get_markets().request();
    //std::cout << response.get_json();
    EXPECT_EQ(response.is_succesful(), true);
    EXPECT_THAT(response.response, testing::HasSubstr(R"("display_name":)"));
}
