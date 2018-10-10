// Boost random generator
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
// Eigen dense matrices
#include <Eigen/Dense>

#include "factory.h"
#include "normal_multivar.h"

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
  Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> lower_cholesky =
      cov.llt().matrixL();
  Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> random_vars =
      Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Zero(cov.rows(),
                                                                  cases);

  // Generate random numbers based on distribution and generator type for
  // requested number of cases
  for (unsigned int i = 0; i < random_vars.cols(); ++i) {
    for (unsigned int j = 0; j < random_vars.rows(); ++j) {
      random_vars(j, i) = distribution_(generator_);
    }
  }

  // Transform from unit normal distribution based on covariance and mean values
  for (unsigned int i = 0; i < random_vars.cols(); ++i) {
    random_vars.col(i) = lower_cholesky * random_vars.col(i) + means;
  }
  
  return random_vars;
}

std::string NormalMultiVar::name() const {
  return "NormalMultiVar";
}
}  // namespace numeric_utils
