#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "factory.h"
#include "numeric_utils.h"
#include "normal_dist.h"
#include "normal_multivar.h"

/**
 * Namespace for configuration and settings
 */
namespace config {

/**
 * This function registers classes with the factory methods so that clients
 * using the library will have access to them
 */
void initialize() {
  // Register multivariate normal distribution random number generator
  static Register<numeric_utils::RandomGenerator, numeric_utils::NormalMultiVar,
                  int>
      normal_multivar("MultivariateNormal");
  // Register normal distribution
  static Register<stochastic::Distribution, stochastic::NormalDistribution,
                  double, double>
      normal_dist("NormalDist");
}
}  // namespace config

#endif  // _CONFIGURE_H_
