//
// Created by egordm on 4-12-2017.
//

#ifndef CRYPTO_TOOLS_NET_UTILS_H
#define CRYPTO_TOOLS_NET_UTILS_H

#include <string>
#include <unordered_map>
#include <sstream>

namespace api { namespace utils {
    static std::string encode_url_params(const std::unordered_map<std::string, std::string> &params) {
        std::stringstream ss;
        for(const auto &param : params) {
            if(ss.tellp() != 0) ss << "&";
            ss << param.first;
            if(!param.second.empty()) ss << "=" << param.second;
        }

        return ss.str();
    }

    static std::string format_header(const std::string &key, const std::string &val) {
        return key + ": " + val;
    }
}}

#endif //CRYPTO_TOOLS_NET_UTILS_H
