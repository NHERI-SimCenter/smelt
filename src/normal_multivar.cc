// Boost random generator
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
// Eigen dense matrices
#include <Eigen/Dense>

#include "factory.h"
#include "normal_multivar.h"

// Register method with factory
static Register<numeric_utils::RandomGenerator, numeric_utils::NormalMultiVar,
                int>
    normal_multivar("MultivariateNormal");

namespace numeric_utils {

NormalMultiVar::NormalMultiVar()
  : RandomGenerator()
{
  generator_ = boost::random::mt19937(seed_);
  distribution_ = boost::random::normal_distribution<double>();
}

NormalMultiVar::NormalMultiVar(int seed)
  : RandomGenerator()
{
  seed_ = seed;
  generator_ = boost::random::mt19937(seed_);
  distribution_ = boost::random::normal_distribution<double>();
}

Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> NormalMultiVar::generate(
    const Eigen::VectorXd& means, const Eigen::MatrixXd& cov,
    unsigned int cases) {
  auto lower_cholesky = cov.llt().matrixL();
  Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> random_vars =
      Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Zero(cov.rows(),
                                                                  cases);

  for (unsigned int i = 0; i < random_vars.cols(); ++i) {
    for (unsigned int j = 0; j < random_vars.rows(); ++j) {
      random_vars(j, i) = distribution_(generator_);
    }
  }

  return lower_cholesky * random_vars + means;
}
}  // namespace numeric_utils
