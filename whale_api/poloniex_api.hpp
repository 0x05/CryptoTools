//
// Created by egordm on 12-12-2017.
//

#ifndef CRYPTO_TOOLS_POLONIEX_API_HPP
#define CRYPTO_TOOLS_POLONIEX_API_HPP

#include "api_interface.h"

namespace api {
    class PoloniexAPI : public APIInterface {
    protected:
        std::string get_base_endpoint() const override { return "https://poloniex.com/public"; }

        void append_api_token(std::unordered_map<std::string, std::string> &headers) const override {

        }

    public:
        PoloniexAPI(const std::string &api_token) : APIInterface(api_token) {}

        Request get_ticker() {
            auto ret = get_request("", GET);
            ret.params.emplace("command", "returnTicker");
            return ret;
        }

        Request get_order_book(const std::string &market) {
            auto ret = get_request("", GET);
            ret.params.emplace("command", "returnOrderBook");
            ret.params.emplace("currencyPair", market);
            return ret;
        }

        /**
         *
         * @param market
         * @param period
         * @param start unix time
         * @param end unix time
         * @return
         */
        Request get_chart_data(const std::string &market, const int &period, time_t start, time_t end) {
            auto ret = get_request("", GET);
            ret.params.emplace("command", "returnChartData");
            ret.params.emplace("currencyPair", market);
            ret.params.emplace("period", std::to_string(period));
            ret.params.emplace("start", std::to_string(start));
            ret.params.emplace("end", std::to_string(end));
            return ret;
        }
    };
}

#endif //CRYPTO_TOOLS_POLONIEX_API_HPP
