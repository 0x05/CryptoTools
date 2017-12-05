//
// Created by egordm on 3-12-2017.
//

#include <iostream>
#include "api_interface.h"

namespace api {
    const std::string Request::POST = "POST";
    const std::string Request::GET = "GET";
    const std::string Request::DELETE = "DELETE";
    const std::string Request::PUT = "PUT";

    void Request::decorate_request(CURL *curl) {
        // Add headers to the request
        if (!headers.empty()) {
            struct curl_slist *chunk = nullptr;
            for (const auto &header : headers) {
                chunk = curl_slist_append(chunk, utils::format_header(header.first, header.second).c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        }

        // If we are posting data. Encode and add it. Also handle special request types
        if (method != GET && !post_data.empty()) {
            if (method == PUT || method == DELETE) curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, format_data().c_str());
        }
    }

    Response Request::request() {
        CURL *curl;
        CURLcode res = CURLE_RECV_ERROR;
        std::string str_res;

        curl_global_init(CURL_GLOBAL_DEFAULT); // TODO: run once every startup not request :S
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, get_url().c_str());
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_res);

           decorate_request(curl);

            res = curl_easy_perform(curl);
            //if (res != CURLE_OK) throw std::runtime_error("Houston, we have got a problem!");
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();

        return Response(res, str_res);
    }

    nlohmann::json Response::get_json() const {
        return nlohmann::json::parse(response);
    }

    Request APIInterface::get_request(const std::string &path, const std::string &method) {
        auto request = Request(get_base_endpoint() + path, method);
        auto headers = get_base_headers();

        append_api_token(headers);
        request.headers.insert(headers.begin(), headers.end());
        return request;
    }
}