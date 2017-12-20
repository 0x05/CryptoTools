//
// Created by egordm on 20-12-2017.
//

#ifndef CRYPTO_TOOLS_MATH_HPP
#define CRYPTO_TOOLS_MATH_HPP

#include <vector>
#include <algorithm>

namespace tech_analysis {

    /**
     * Sum all objects in an iterator by field. Inclusion is like this [begin, end>
     * @tparam It
     * @tparam N
     * @tparam F
     * @param begin
     * @param end
     * @param mp
     * @return
     */
    template<typename It, typename N, typename F>
    static double SUM(It begin, It end, F N::*mp) {
        double ret = 0;
        for (auto iter = begin; iter != end; ++iter) ret += *iter.*mp;
        return ret;
    }

    /**
     * Simple moving average on a sequence
     * @tparam It
     * @tparam N
     * @tparam F
     * @param period
     * @param begin
     * @param end
     * @param mp
     * @return
     */
    template<typename It, typename N, typename F>
    static std::vector<double> SMA(unsigned int period, It begin, It end, F N::*mp) {
        std::vector<double> ret;
        auto dist = std::distance(begin, end);
        for (auto iter = end; iter != begin; --iter, --dist) {
            const auto n = std::min(static_cast<long>(period), dist);
            ret.insert(ret.begin(), SUM(iter - n, iter, mp) / n);
        }
        return ret;
    }

    /**
     * Exponential moving average on a sequence
     * @tparam It
     * @tparam N
     * @tparam F
     * @param period
     * @param begin
     * @param end
     * @param mp
     * @return
     */
    template<typename It, typename N, typename F>
    static std::vector<double> EMA(unsigned int period, It begin, It end, F N::*mp) {
        std::vector<double> ret;
        const double k = 2.0 / (period + 1);
        for (auto iter = begin; iter != end; ++iter) {
            const auto prev_ema = ret.empty() ? *iter.*mp : ret.back();
            ret.push_back((*iter.*mp * k) + (prev_ema * (1 - k))); // (P(t) * k) + (P(t-1) * (1 - k))
        }
        return ret;
    }

}

#endif //CRYPTO_TOOLS_MATH_HPP
