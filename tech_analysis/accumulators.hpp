//
// Created by egordm on 22-12-2017.
//

#ifndef CRYPTO_TOOLS_ACCUMULATORS_HPP
#define CRYPTO_TOOLS_ACCUMULATORS_HPP

#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/statistics/rolling_variance.hpp>


namespace boost { namespace accumulators {
    namespace impl {
        /**
         * Exponential movign average
         * @tparam Sample
         */
        template<typename Sample>
        struct ema_impl : accumulator_base {
            typedef Sample result_type;

            template<typename Args>
            ema_impl(Args const &args) : k(2.0 / (args[rolling_window_size] + 1)) {}

            template<typename Args>
            void operator()(Args const &args) {
                last_ema = last_ema < 0 ? args[sample] : (args[sample] * k) + (last_ema * (1 - k));
            }

            template<typename Args>
            result_type result(Args const &args) const {
                return last_ema;
            }

        private:
            double last_ema = -1;
            double k;
        };

        template<typename Sample>
        struct rolling_marginal_mean_impl : accumulator_base {
            typedef Sample result_type;

            template<typename Args>
            rolling_marginal_mean_impl(Args const &args) : window(args[rolling_window_size]) {}

            template<typename Args>
            void operator()(Args const &args) {
                last_mean = last_mean < 0 ? args[sample] : (args[sample] + last_mean * (window - 1)) / window;
            }

            template<typename Args>
            result_type result(Args const &args) const {
                return last_mean;
            }

        private:
            double last_mean = -1;
            double window;
        };
    }

    namespace tag {
        struct ema_accumulator : depends_on<>, tag::rolling_window_size {
            typedef accumulators::impl::ema_impl<mpl::_1> impl;
        };
        struct rolling_mma : depends_on<>, tag::rolling_window_size {
            typedef accumulators::impl::rolling_marginal_mean_impl<mpl::_1> impl;
        };
    }

    namespace extract {
        extractor<tag::ema_accumulator> const ema_accumulator = {};
        extractor<tag::rolling_mma> const rolling_mma = {};

        BOOST_ACCUMULATORS_IGNORE_GLOBAL(ema_accumulator)
        BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_mma)
    }

    using extract::ema_accumulator;
    using extract::rolling_mma;
}}

#endif //CRYPTO_TOOLS_ACCUMULATORS_HPP
