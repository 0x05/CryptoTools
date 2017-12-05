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

namespace api {
    static size_t write_callback(void *contents, size_t size, size_t nmemb, void *stream) {
        ((std::string *) stream)->append((char *) contents, size * nmemb);
        return size * nmemb;
    }

    struct Response {
        const CURLcode code;
        const std::string response;

        Response(CURLcode code, const std::string &response) : code(code), response(std::move(response)) {}

        bool is_succesful() const { return code == CURLE_OK; }

        friend std::ostream &operator<<(std::ostream &os, const Response &response) {
            os << "code: " << (int) response.code << " response: " << response.response;
            return os;
        }
    };

    class Request {
    public:
        static const std::string POST;
        static const std::string GET;
        static const std::string DELETE;
        static const std::string PUT;

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

    public:
        Request(const std::string &url, const std::string &method,
                std::unordered_map<std::string, std::string> headers,
                std::unordered_map<std::string, std::string> params,
                std::unordered_map<std::string, std::string> post_data)
                : url(std::move(url)), method(std::move(method)), headers(std::move(headers)),
                  params(std::move(params)),
                  post_data(std::move(post_data)) {}

        Request(const std::string &url, const std::string &method,
                std::unordered_map<std::string, std::string> headers)
                : url(std::move(url)), method(std::move(method)), headers(std::move(headers)) {}

        Request(const std::string &url, const std::string &method) : url(std::move(url)), method(std::move(method)) {}

        /**
         * Make the request
         * @return
         */
        Response request();
    };

    class APIInterface {
    protected:
        virtual const std::string get_base_endpoint() = 0;

        virtual const std::vector<std::string> get_base_headers() = 0;

    public:
    };

}


#endif //CRYPTO_TOOLS_API_INTERFACE_H
