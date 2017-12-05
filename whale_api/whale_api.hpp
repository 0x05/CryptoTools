//
// Created by egordm on 5-12-2017.
//

#ifndef CRYPTO_TOOLS_WHALE_API_H
#define CRYPTO_TOOLS_WHALE_API_H

#include "api_interface.h"
#include "temp_keys.hpp"

namespace api {
    class WhaleAPI : public APIInterface {
    protected:
        std::string get_base_endpoint() const override { return "https://api.whaleclub.co/v1/"; }

        void append_api_token(std::unordered_map<std::string, std::string> &headers) const override {
            headers.emplace("Authorization", "Bearer " + api_token);
        }
    public:
        explicit WhaleAPI(const std::string &api_token) : APIInterface(api_token) {}

        Request get_markets() {
            const std::string path = "markets";
            return get_request(path, Request::GET);
        }
    };
}

#endif //CRYPTO_TOOLS_WHALE_API_H
