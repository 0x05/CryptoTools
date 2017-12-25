//
// Created by egordm on 24-12-2017.
//

#ifndef CRYPTO_TOOLS_CANDLE_MATH_HPP
#define CRYPTO_TOOLS_CANDLE_MATH_HPP

#include <cmath>
#include <vector>
#include <boost/optional.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/statistics/rolling_variance.hpp>
#include "accumulators.hpp"

using namespace boost::accumulators;

template<typename Out, typename ... In>
class Indicator {
public:
    Indicator(const unsigned int interval, const unsigned int period)
            : npp(static_cast<const unsigned int>(std::ceil(static_cast<double>(period) / interval))) {}

    void feed_single(In ... in) {
        const auto p = _feed_single(in...);
        if (p && sample_count >= npp) data.push_back(*p);
        sample_count++;
    }

    template<typename It, typename ... Fields>
    void feed(It begin, It end, Fields... fields) {
        for (auto iter = begin; iter != end; ++iter) feed_single(*iter.*fields...);
    }

    const std::vector<Out> &get_data() const {
        return data;
    }

protected:
    const unsigned int npp;
    unsigned int sample_count = 0;
    std::vector<Out> data;

    virtual boost::optional<Out> _feed_single(In ...) = 0;
};

class SMAIndicator : public Indicator<double, double> {
public:
    SMAIndicator(const unsigned int interval, const unsigned int period)
            : Indicator(interval, period), acc(tag::rolling_window::window_size = npp) {}

protected:
    accumulator_set<double, stats<tag::rolling_mean>> acc;

    boost::optional<double> _feed_single(double close) override {
        acc(close);
        return extract_result<tag::rolling_mean>(acc);
    };
};

class EMAIndicator : public Indicator<double, double> {
protected:
    accumulator_set<double, stats<tag::ema_accumulator>> acc;

public:
    EMAIndicator(const unsigned int interval, const unsigned int period)
            : Indicator(interval, period), acc(tag::rolling_window::window_size = npp) {}

    boost::optional<double> _feed_single(double close) override {
        acc(close);
        return extract_result<tag::ema_accumulator>(acc);
    };
};

class BollIndicator : public Indicator<std::tuple<double, double, double>, double> {
protected:
    accumulator_set<double, stats<tag::rolling_mean, tag::rolling_variance>> acc;
public:
    BollIndicator(const unsigned int interval, const unsigned int period)
            : Indicator(interval, period), acc(tag::rolling_window::window_size = npp) {}

    boost::optional<std::tuple<double, double, double>> _feed_single(double close) override {
        acc(close);
        const auto sdv = sqrt(extract_result<tag::rolling_variance>(acc));
        const auto sma = extract_result<tag::rolling_mean>(acc);
        return std::make_tuple(sma - 2 * sdv, sma, sma + 2 * sdv);
    };
};

class MACDIndicator : public Indicator<std::tuple<double, double, double>, double> {
protected:
    accumulator_set<double, stats<tag::ema_accumulator>> acc_long;
    accumulator_set<double, stats<tag::ema_accumulator>> acc_mid;
    accumulator_set<double, stats<tag::ema_accumulator>> acc_signal;
    extractor<tag::ema_accumulator> ex;
public:
    MACDIndicator(const unsigned int interval, const unsigned int period)
            : Indicator(interval, period),
              acc_long(tag::rolling_window::window_size = std::round(npp * 2.166)),
              acc_mid(tag::rolling_window::window_size = npp),
              acc_signal(tag::rolling_window::window_size = std::round(npp * .75)) {}

    boost::optional<std::tuple<double, double, double>> _feed_single(double close) override {
        acc_long(close);
        acc_mid(close);
        const auto macd = ex(acc_mid) - ex(acc_long);
        acc_signal(macd);
        const auto signal = ex(acc_signal);
        const auto his = macd - signal;
        return std::make_tuple(macd, signal, his);
    };
};

class RSIIndicator : public Indicator<double, double> {
protected:
    accumulator_set<double, stats<tag::rolling_mma>> acc_gain;
    accumulator_set<double, stats<tag::rolling_mma>> acc_loss;
    extractor<tag::rolling_mma> ex;
    double last_close;

public:
    RSIIndicator(const unsigned int interval, const unsigned int period)
            : Indicator(interval, period), acc_gain(tag::rolling_window::window_size = npp),
              acc_loss(tag::rolling_window::window_size = npp) {}

    boost::optional<double> _feed_single(double close) override {
        if(sample_count == 0) {
            last_close = close;
            return boost::optional<double>();
        }

        const auto gain = close - last_close;
        last_close = close;
        acc_gain(gain > 0 ? gain : 0);
        acc_loss(gain < 0 ? std::abs(gain) : 0);

        return 100 - 100 / (1 + ex(acc_gain) / ex(acc_loss));
    };
};

class CCIIndicator : public Indicator<double, double, double, double> {
protected:
    accumulator_set<double, stats<tag::rolling_mean, tag::rolling_variance>> acc;
public:
    CCIIndicator(const unsigned int interval, const unsigned int period)
            : Indicator(interval, period), acc(tag::rolling_window::window_size = npp) {}

    boost::optional<double> _feed_single(double high, double low, double close) override {
        const auto tp = (high + low, close) / 3.0;
        acc(tp);

        const auto sma = extract_result<tag::rolling_mean>(acc);
        const auto sdv = sqrt(extract_result<tag::rolling_variance>(acc));
        return (tp - sma) / (.015 * sdv);
    };
};

class CMOIndicator : public Indicator<double, double> {
protected:
    accumulator_set<double, stats<tag::rolling_sum>> acc_pos_sum;
    accumulator_set<double, stats<tag::rolling_sum>> acc_neg_sum;
    extractor<tag::rolling_sum> ex;
    double last_close;
public:
    CMOIndicator(const unsigned int interval, const unsigned int period)
            : Indicator(interval, period), acc_pos_sum(tag::rolling_window::window_size = npp),
              acc_neg_sum(tag::rolling_window::window_size = npp) {}

    boost::optional<double> _feed_single(double close) override {
        if(sample_count == 0) {
            last_close = close;
            return boost::optional<double>();
        }
        const auto diff = close - last_close;
        if(diff > 0) acc_pos_sum(diff);
        else acc_neg_sum(std::abs(diff));
        last_close = close;

        const auto pos_sum = ex(acc_pos_sum);
        const auto neg_sum = ex(acc_neg_sum);
        return ((pos_sum - neg_sum) / (pos_sum + neg_sum)) * 100;
    };
};

class MFIIndicator : public Indicator<double, double, double, double, double> {
protected:
    accumulator_set<double, stats<tag::rolling_sum>> acc_pmf;
    accumulator_set<double, stats<tag::rolling_sum>> acc_nmf;
    extractor<tag::rolling_sum> ex;
    double last_tp;
public:
    MFIIndicator(const unsigned int interval, const unsigned int period)
            : Indicator(interval, period), acc_pmf(tag::rolling_window::window_size = npp),
              acc_nmf(tag::rolling_window::window_size = npp){}

    boost::optional<double> _feed_single(double high, double low, double close, double volume) override {
        const auto tp = (high + low, close) / 3.0;
        if(sample_count == 0) {
            last_tp = tp;
            return boost::optional<double>();
        }

        const auto rmf = tp * volume;
        if(tp > last_tp) acc_pmf(rmf);
        else acc_nmf(rmf);
        last_tp = tp;

        const auto mfr = ex(acc_pmf) / ex(acc_nmf);
        return 100 - 100 / (1 + mfr);
    };
};

#endif //CRYPTO_TOOLS_CANDLE_MATH_HPP
