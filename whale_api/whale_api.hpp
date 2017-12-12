//
// Created by egordm on 5-12-2017.
//

#ifndef CRYPTO_TOOLS_WHALE_API_H
#define CRYPTO_TOOLS_WHALE_API_H

#include "api_interface.h"
#include "temp_keys.hpp"
#include <boost/algorithm/string/join.hpp>

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
            return get_request(path, GET);
        }

        Request get_markets(const std::vector<std::string> &symbols) {
            const std::string path = "markets/" +  boost::algorithm::join(symbols, ",");
            return get_request(path, GET);
        }

        Request get_price(const std::vector<std::string> &symbols) {
            const std::string path = "price/" +  boost::algorithm::join(symbols, ",");
            return get_request(path, GET);
        }

        Request get_balance() {
            return get_request("balance", GET);
        }

        static constexpr const char* TYPE_DEPOSITS = "deposits";
        static constexpr const char* TYPE_WITHDRAWALS = "withdrawals";
        static constexpr const char* TYPE_REFERRALS = "referrals";
        static constexpr const char* TYPE_BONUSSES = "bonuses";

        Request get_tranactions(const std::string &type, const unsigned int limit) {
            auto ret = get_request("transactions/"+type, GET);
            ret.params.emplace("limit", std::to_string(limit));
            return ret;
        }

        static constexpr const char* DIRECTION_BUY = "long";
        static constexpr const char* DIRECTION_SELL = "short";
        static constexpr double MARKET_PRICE = -1337;


        Request get_order(const std::string &id) {
            return get_request("position/" + id, GET);
        }

        static constexpr const char* STATE_PENDING = "pending";
        static constexpr const char* STATE_ACTIVE = "active";
        static constexpr const char* STATE_CLOSED = "closed";

        Request get_orders(const std::string &state) {
            return get_request("positions/" + state, GET);
        }

        //TODO: return a order object which can make cancel requests etc
        Request new_order(const std::string &direction, const std::string &market, const double &quantity, const double &price) {
            auto ret = get_request("position/new/", POST);
            ret.post_data.emplace("direction", direction);
            ret.post_data.emplace("market", market);
            ret.post_data.emplace("size", std::to_string((int)quantity));
            ret.post_data.emplace("leverage", "1");
            if(price > 0) ret.post_data.emplace("entry_price", std::to_string(price));
            return ret;
        }

        Request cancel_order(const std::string &id) {
            return get_request("position/cancel/" + id, PUT);
        }

        Request close_order(const std::string &id) {
            return get_request("position/close/" + id, PUT);
        }
    };
}

#endif //CRYPTO_TOOLS_WHALE_API_H
