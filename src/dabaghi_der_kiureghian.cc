#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <ctime>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
// Boost random generator
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>
// Eigen dense matrices
#include <Eigen/Dense>

#include "factory.h"
#include "function_dispatcher.h"
#include "json_object.h"
#include "dabaghi_der_kiureghian.h"

stochastic::DabaghiDerKiureghian::DabaghiDerKiureghian(
    stochastic::FaultType faulting, stochastic::SimulationType simulation_type,
    double moment_magnitude, double depth_to_rupt, double rupture_distance,
    double vs30, double s_or_d, double theta_or_phi, unsigned int num_sims,
    unsigned int num_params, bool truncate)
    : StochasticModel(),
      faulting_{faulting},
      sim_type_{simulation_type},
      moment_magnitude_{moment_magnitude},
      depth_to_rupt_{depth_to_rupt},
      rupture_dist_{rupture_distance},
      vs30_{vs30},
      s_or_d_{s_or_d},
      theta_or_phi_{theta_or_phi},
      truncate_{truncate},
      num_params_{num_params},
      seed_value_{std::numeric_limits<int>::infinity()},
      time_step_{0.005}
{
  model_name_ = "DabaghiDerKiureghian";
  num_sims_pulse_ = simulate_pulse_type(num_sims);
  num_sims_no_pulse_ = num_sims - num_sims_pulse_;
}

stochastic::DabaghiDerKiureghian::DabaghiDerKiureghian(
    stochastic::FaultType faulting, stochastic::SimulationType simulation_type,
    double moment_magnitude, double depth_to_rupt, double rupture_distance,
    double vs30, double s_or_d, double theta_or_phi, unsigned int num_sims,
    unsigned int num_params, bool truncate, int seed_value)
    : StochasticModel(),
      faulting_{faulting},
      sim_type_{simulation_type},
      moment_magnitude_{moment_magnitude},
      depth_to_rupt_{depth_to_rupt},
      rupture_dist_{rupture_distance},
      vs30_{vs30},
      s_or_d_{s_or_d},
      theta_or_phi_{theta_or_phi},
      truncate_{truncate},
      num_params_{num_params},
      seed_value_{seed_value},
      time_step_{0.005}
{
  model_name_ = "DabaghiDerKiureghian";
  num_sims_pulse_ = simulate_pulse_type(num_sims);
  num_sims_no_pulse_ = num_sims - num_sims_pulse_;
}

unsigned int stochastic::DabaghiDerKiureghian::simulate_pulse_type(unsigned int num_sims) const {
  double pulse_probability = 0.0;

  // Calculate pulse probability for any type of pulse
  if (faulting == stochastic::StrikeSlip) {
    pulse_probability = 1.0 / (1.0 + std::exp(0.457 + 0.126 * rupture_dist_ -
                                              0.244 * std::sqrt(s_or_d_) +
                                              0.013 * theta_or_phi_));
  } else {
    pulse_probability = 1.0 / (1.0 + std::exp(0.304 + 0.072 * rupture_dist_ -
                                              0.208 * std::sqrt(s_or_d_) +
                                              0.021 * theta_or_phi_));
  }

  // Create random generator for uniform distribution between 0.0 and 1.0
  auto generator =
    seed_value_ != std::numeric_limits<int>::infinity()
    ? boost::random::mt19937(static_cast<unsigned int>(seed_value_))
    : boost::random::mt19937(static_cast<unsigned int>(std::time(nullptr)));
  
  boost::random::uniform_real_distribution<> distribution(0.0, 1.0);
  boost::random::variate_generator<boost::random::mt19937&,
                                   boost::random::uniform_real_distribution<>>
      pulse_gen(generator, distribution);

  unsigned int number_of_pulses = 0;
  
  for (unsigned int i = 0; i < num_sims_pulse_ + num_sims_no_pulse_; ++i) {
    if (pulse_gen() < pulse_probability) {
      number_of_pulses++;
    }
  }

  return number_of_pulses;
}
