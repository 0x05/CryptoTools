//
// Created by egordm on 4-12-2017.
//

#include <gtest/gtest.h>
#include <unordered_map>
#include <gmock/gmock-matchers.h>
#include "../../whale_api/net_utils.hpp"
#include "../../whale_api/api_interface.h"

class APIInterfaceTest : public ::testing::Test {

};


TEST_F(APIInterfaceTest, test_url_param_encoding) {
    std::unordered_map<std::string, std::string> params;
    params.emplace("foo", "bar");
    params.emplace("pi", "");
    params.emplace("lazy", "dog");

    const auto &url_params = api::utils::encode_url_params(params);
    EXPECT_EQ(url_params, "lazy=dog&pi&foo=bar");
}


TEST_F(APIInterfaceTest, test_get_request) {
    const auto &url = "https://jsonplaceholder.typicode.com/posts/1";
    const auto &res = api::Request(url, api::Request::GET).request();
    EXPECT_EQ(res.is_succesful(), true);
    EXPECT_THAT(res.response, testing::HasSubstr("sunt aut facere"));
}


TEST_F(APIInterfaceTest, test_get_parameters) {
    std::unordered_map<std::string, std::string> params;
    params.emplace("userId", "1337");

    const auto &url = "https://jsonplaceholder.typicode.com/posts";
    auto req = api::Request(url, api::Request::GET);
    req.params = params;

    const auto &res = req.request();
    EXPECT_EQ(res.is_succesful(), true);
    EXPECT_THAT(res.response, testing::HasSubstr("[]"));
}


TEST_F(APIInterfaceTest, test_post_parameters) {
    std::unordered_map<std::string, std::string> params;
    params.emplace("userId", "1337");
    const auto &url = "http://jsonplaceholder.typicode.com/posts";

    auto req = api::Request(url, api::Request::POST);
    req.post_data = params;

    const auto &res = req.request();
    EXPECT_EQ(res.is_succesful(), true);
    EXPECT_THAT(res.response, testing::HasSubstr(R"("userId": "1337",)"));
}

