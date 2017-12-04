//
// Created by egordm on 4-12-2017.
//

#include <gtest/gtest.h>
#include <unordered_map>
#include <gmock/gmock-matchers.h>
#include "../../whale_api/net_utils.hpp"
#include "../../whale_api/api_interface.h"


TEST(api_interface_test, test_url_param_encoding) {
    std::unordered_map<std::string, std::string> params;
    params.emplace("foo", "bar");
    params.emplace("pi", "");
    params.emplace("lazy", "dog");

    const auto &url_params = api::utils::encode_url_params(params);
    EXPECT_EQ(url_params, "lazy=dog&pi&foo=bar");
}


TEST(api_interface_test, test_get_request) {
    const auto &url = "https://jsonplaceholder.typicode.com/posts/1";
    const auto &res = api::Request(url, api::Request::GET).request();
    EXPECT_EQ(res.is_succesful(), true);
    EXPECT_THAT(res.response, testing::HasSubstr("sunt aut facere"));
}
