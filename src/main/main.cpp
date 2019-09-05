#include <iostream>
#include <functional>

#include <range/v3/range/conversion.hpp>

#include "market/buildmarket.h"
#include "market/market.h"
#include "sim/buildprocess.h"
#include "sim/process.h"
#include "sim/simulation.h"
#include "utility/interval.h"
#include "utility/lambda.h"


int main (int argc, char* argv[]) {

  std::cout << "This is a start for a vol demo." << std::endl;

  /**
  * setup is an arithmetic asian option.
  */
  using namespace vol::market;
  using namespace vol::proc;
  using namespace vol;

  double begin = 0.;
  double end = 1.;
  double dt = 1. / 5.;
  double strike = 100.;
  double fut = 100.;
  double rate = 0.01;
  double vol = 0.2;
 

  //FIXME - that could be prettier
  auto logNormPath = buildLogNormalPath(fut, rate, vol, begin, end, dt);
 
  std::cout << "built lognormal paths." << std::endl; 
  auto log = [](double t) {return std::log(t);};
  auto exp = [](double t) {return std::exp(t);};

  auto asian = buildAsian(begin, end, dt);
  auto geomAsian = buildGeometricAsian(begin, end, dt);

  auto agg = utility::aggregate(asian, geomAsian);
  auto asians = utility::compose(agg, logNormPath);
  auto payoff = vanilla::payoff(option::CALL, strike);
  auto calls = utility::replicate(payoff, std::array<double, 2>());

  auto asianCalls = vol::utility::compose(calls, asians);
  std::cout << "built asian options that share an asset path." << std::endl;
  //calculate the geometric asiam price
  double geoPrice = asian::geomAsian(option::CALL, rate, fut, end, vol, strike, dt);
 
  //create payoffs with price processes
  auto paired_sample = ranges::to<std::vector>(
    ranges::views::generate_n(
    [end, asianCalls]() mutable -> std::array<double, 2u> {
      return asianCalls(end); }, 10));
  
  auto summary = vol::stats::summary(
      paired_sample.begin(), 
      paired_sample.end(), 
      geoPrice);

  std::cout << " count = " << std::get<0>(summary)
             << ", mean = " << std::get<1>(summary)
             << ", variance = "<< std::get<2>(summary) << std::endl; 
  return 0;
}

