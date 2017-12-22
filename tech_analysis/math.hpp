//
// Created by egordm on 20-12-2017.
//

#ifndef CRYPTO_TOOLS_MATH_HPP
#define CRYPTO_TOOLS_MATH_HPP

#include <vector>
#include <algorithm>
#include <cmath>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include "accumulators.hpp"

using namespace boost::accumulators;
using namespace std::placeholders;

namespace tech_analysis {
    template<typename T, typename ST, typename It, typename N, typename F>
    static std::vector<T> calc_graph(accumulator_set<T, stats<ST>> acc, It begin, It end, F N::*val) {
        std::vector<T> ret;
        extractor<ST> ex;
        for (auto iter = begin; iter != end; ++iter) {
            acc(*iter.*val);
            ret.push_back(ex(acc));
        }
        return ret;
    }

    /**
     * Exponential moving average on a sequence
     * @tparam It
     * @tparam N
     * @tparam F
     * @param interval interval between points in seconds
     * @param period over which the sma is calculated
     * @param begin
     * @param end
     * @param val
     * @return
     */
    template<typename It, typename N, typename F>
    static std::vector<double> EMA(unsigned int interval, unsigned int period, It begin, It end, F N::*val) {
        const auto no = std::ceil(static_cast<double>(period) / interval);
        accumulator_set<double, stats<tag::ema_accumulator>> acc(tag::rolling_window::window_size = no);
        return calc_graph(acc, begin, end, val);
    }


    /**
    * Simple moving average on a sequence
    * @tparam It
    * @tparam N
    * @tparam F
    * @param interval interval between points in seconds
    * @param period over which the sma is calculated
    * @param begin
    * @param end
    * @param val
    * @return
    */
    template<typename It, typename N, typename F>
    static std::vector<double> SMA(unsigned int interval, unsigned int period, It begin, It end, F N::*val) {
        const auto no = std::ceil(static_cast<double>(period) / interval);
        accumulator_set<double, stats<tag::rolling_mean>> acc(tag::rolling_window::window_size = no);
        return calc_graph(acc, begin, end, val);
    }

    typedef std::tuple<double, double, double> triplet;

    /**
     * Calculate bollinger bands and store them in a vector of triplets. (bottom band, mid band (sma), top band)
     * @tparam It
     * @tparam N
     * @tparam F
     * @param interval
     * @param period
     * @param begin
     * @param end
     * @param val
     * @return
     */
    template<typename It, typename N, typename F>
    static std::vector<triplet> BOLL(unsigned int interval, unsigned int period, It begin, It end, F N::*val) {
        const auto no = std::ceil(static_cast<double>(period) / interval);
        accumulator_set<double, stats<tag::rolling_mean, tag::rolling_variance>>
        acc(tag::rolling_window::window_size = no);
        std::vector<triplet> ret;

        for (auto iter = begin; iter != end; ++iter) {
            acc(*iter.*val);
            const auto sdv = sqrt(extract_result<tag::rolling_variance>(acc));
            const auto sma = extract_result<tag::rolling_mean>(acc);
            std::cout << sma << " and " << sdv << std::endl;
            ret.push_back(std::make_tuple(sma - 2 * sdv, sma, sma + 2 * sdv));
        }

        return ret;
    }
}

#endif //CRYPTO_TOOLS_MATH_HPP
