
#include <iostream>

#include <tuple>

#include <range/v3/all.hpp>

#include "catch2/catch.hpp"

#include "sim/simulation.h"


SCENARIO ("samples are used to summarise simulations..", "[sample]") {
  WHEN("a sample is constructed") {
    using namespace vol::stats;

    std::vector<double> sample = {0., 1., 2., 3., 4.};

    THEN("sample_sums delivers sample size, mean and variance") {
      auto summary = variance(sample.begin(), sample.end());

      CHECK( std::get<0>(summary) == 5u );
      CHECK( std::get<1>(summary) == 2. );
      CHECK( std::get<2>(summary) == 2.5 );
    }
  }
  
  WHEN("a 2d sample is constructed") {
    using namespace vol::stats;
    std::vector<std::array<double, 2>> sample_pair = {{0., 4.}, {1., 3.}, {2., 2.}, {3., 1.}, {4., 0.}}; 
    THEN("sample_sums delivers sample size, mean and variance") {
      auto summary = covariance(sample_pair.begin(), sample_pair.end());

      CHECK( std::get<0>(summary) == 5u );
      CHECK( std::get<1>(summary) == 2. );
      CHECK( std::get<2>(summary) == 2. );
      CHECK( std::get<3>(summary) == 2.5 );
      CHECK( std::get<4>(summary) == -2.5 );
      CHECK( std::get<5>(summary) == 2.5 );
    }

    WHEN("a control variate is presented") {
      double c_mean = 2.1;

      THEN("we can calculate a mean and variance.") {
        auto result = summary(sample_pair.begin(), sample_pair.end(), c_mean);

        CHECK(std::get<1>(result) == 1.9);
        CHECK(std::get<2>(result) == 0.);
      }
    }
  }
}

