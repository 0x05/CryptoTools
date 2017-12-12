//
// Created by egordm on 3-12-2017.
//

#ifndef CRYPTO_TOOLS_API_INTERFACE_H
#define CRYPTO_TOOLS_API_INTERFACE_H


#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <curl/curl.h>
#include <ostream>
#include "net_utils.hpp"
#include "json.hpp"

namespace api {
    static size_t write_callback(void *contents, size_t size, size_t nmemb, void *stream) {
        ((std::string *) stream)->append((char *) contents, size * nmemb);
        return size * nmemb;
    }

    static constexpr const char* POST = "POST";
    static constexpr const char* GET = "GET";
    static constexpr const char* DELETE = "DELETE";
    static constexpr const char* PUT = "PUT";

    template <typename T>
    struct Transformer {
        virtual T transform(nlohmann::json json) = 0;
    };

    struct Response {
        const CURLcode code;
        const std::string response;

        Response(CURLcode code, const std::string &response) : code(code), response(std::move(response)) {}

        nlohmann::json get_json() const;

        bool is_succesful() const { return code == CURLE_OK; }

        friend std::ostream &operator<<(std::ostream &os, const Response &response) {
            os << "code: " << (int) response.code << " response: " << response.response;
            return os;
        }
    };

    class Request {
    public:
        const std::string url;
        /**
         * Request method
         */
        const std::string method;
        /**
         * Headers
         */
        std::unordered_map<std::string, std::string> headers{};
        /**
         * Get params
         */
        std::unordered_map<std::string, std::string> params{};
        /**
         * Post data
         */
        std::unordered_map<std::string, std::string> post_data{}; //TODO: this is a encoding format. Can be substituted by json
    protected:
        /**
         * Format data in desired format json or x-www-form-urlencoded
         * @return
         */
        const std::string format_data() {
            return utils::encode_url_params(post_data);
        }

        /**
         * Decorate the request. Add headers, post data, etc etc
         * @param curl
         */
        virtual void decorate_request(CURL * curl);

    public:
        const std::string get_url() {
            return (url + "?" + utils::encode_url_params(params));
        }

        Request(const std::string &url, const std::string &method) : url(std::move(url)), method(std::move(method)) {}

        /**
         * Make the request
         * @return
         */
        Response request();
    };

    class APIInterface {
    protected:
        const std::string api_token;

        virtual std::string get_base_endpoint() const = 0;

        virtual void append_api_token(std::unordered_map<std::string, std::string> &headers) const = 0;

        virtual std::unordered_map<std::string, std::string> get_base_headers() const {
            auto ret = std::unordered_map<std::string, std::string>();
            //ret.emplace("Content-Type", "application/json");
            ret.emplace("X-Requested-With", "XMLHttpRequest");
            return ret;
        };
    public:
        explicit APIInterface(const std::string &api_token) : api_token(std::move(api_token)) {};

        virtual Request get_request(const std::string &path, const std::string &method);
    };

}


#endif //CRYPTO_TOOLS_API_INTERFACE_H
