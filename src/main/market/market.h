#pragma once
#include <range/v3/all.hpp>

#include "stats.hpp"


namespace vol {
  enum class option {CALL, PUT};
  namespace market::vanilla {

    double black(option o, double r, double f, double t, double v, double k);

    double delta(option o, double r, double f, double t, double v, double k);

    double gamma(option o, double r, double f, double t, double v, double k);

    double vega(option o, double r, double f, double t, double v, double k);

    double theta(option o, double r, double f, double t, double v, double k);

    double rho(option o, double r, double f, double t, double v, double k);

    inline auto payoff(option o, double k) {
      return [o, k](double s) {
        switch(o) {
          case option::CALL:
            return std::max(s - k, 0.);
            break;
          case option::PUT:
            return std::max(k - s, 0.);
            break;
          default:
            return std::numeric_limits<double>::quiet_NaN();
        }
      };
    }
  }

  namespace market::asian {
    template<typename process>
    double asianing(process p, double& start, double end, double dt) {
      /*FIXME - hack.  can't figure out how to get iota 
      * and concepts to work
      * as desired.
      */ 
      size_t count = (end - start) / dt;
      return accumulate(ranges::views::iota(0u, count) 
          | [start, end, dt](size_t i) {return i * dt + start;} 
          | p, 0);
    }
  }
}

