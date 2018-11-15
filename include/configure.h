#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "beta_dist.h"
#include "factory.h"
#include "filter.h"
#include "function_dispatcher.h"
#include "inv_gauss_dist.h"
#include "lognormal_dist.h"
#include "numeric_utils.h"
#include "normal_dist.h"
#include "normal_multivar.h"
#include "students_t_dist.h"
#include "window.h"

/**
 * Namespace for configuration and settings
 */
namespace config {

/**
 * This function registers classes with the factory methods so that clients
 * using the library will have access to them
 */
void initialize() {
  // RANDOM VARIABLE GENERATION
  // Register multivariate normal distribution random number generator
  static Register<numeric_utils::RandomGenerator, numeric_utils::NormalMultiVar,
                  int>
      normal_multivar("MultivariateNormal");

  // DISTRIBUTION TYPES
  // Register normal distribution
  static Register<stochastic::Distribution, stochastic::NormalDistribution,
                  double, double>
      normal_dist("NormalDist");
  // Register lognormal distribution
  static Register<stochastic::Distribution, stochastic::LognormalDistribution,
                  double, double>
      lognormal_dist("LognormalDist");
  // Register inverse Gaussian distribution
  static Register<stochastic::Distribution, stochastic::InverseGaussianDistribution,
                  double, double>
      inv_gauss_dist("InverseGaussianDist");  
  // Register beta distribution
  static Register<stochastic::Distribution, stochastic::BetaDistribution,
                  double, double>
      beta_dist("BetaDist");
  // Register Student's t distribution
  static Register<stochastic::Distribution, stochastic::StudentstDistribution,
                  double, double, double>
      student_t_dist("StudentstDist");

  // WINDOW FUNCTIONS
  // Register Hann window
  static DispatchRegister<std::vector<double>, unsigned int> hann_window_function(
      "HannWindow", signal_processing::hann_window);

  /* // FILTER FUNCTIONS */
  /* static DispatchRegister<std::vector<std::vector<double>>(unsigned int, double) */
}
}  // namespace config

#endif  // _CONFIGURE_H_
