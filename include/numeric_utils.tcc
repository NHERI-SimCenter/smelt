// Boost distribution types
#include <boost/math/distributions/normal.hpp>
// Boost random generator
#include <boost/random/variate_generator.hpp>
// Eigen dense matrix
#include <Eigen/Dense>

namespace numeric_utils {

template <typename Generator, template <typename> typename Distribution,
          typename RealType>
RandomGenerator<Generator, Distribution<RealType> >::RandomGenerator(int seed)
    : seed_{seed},
      generator_{Generator},
      distribution_{Distribution<RealType>},
      var_generator_(generator_(seed_), distribution_()) {}

template <typename Generator, template <typename> typename Distribution,
          typename RealType>
double RandomGenerator<Generator, Distribution<RealType> >::generate(
    double mean, double std_dev) const {
  return std_dev * std_dev * var_generator_() + mean;
}

template <typename Generator, template <typename> typename Distribution,
          typename RealType>
Eigen::VectorXd RandomGenerator<Generator, Distribution<RealType> >::generate(
    double mean, double std_dev, unsigned int cases) const {
  Eigen::VectorXd random_vals = Eigen::VectorXd::Zero(cases);

  for (auto& realization : random_vals) {
    realization = std_dev * std_dev * var_generator_() + mean;
  }

  return random_vals;
}

template <typename Generator, template <typename> typename Distribution,
          typename RealType>
Eigen::MatrixXd RandomGenerator<Generator, Distribution<RealType> >::generate(
    const Eigen::VectorXd& means, const Eigen::MatrixXd& cov,
    unsigned int cases = 1) const {
  auto lower_cholesky = cov.llt().matrixL();
  auto random_vars = Eigen::MatrixXd::Zero(cov.rows(), cases);
  
  for (unsigned int i = 0; i < random_vars.cols(); ++i) {
    for (unsigned int j = 0; j < random_vars.rows(); ++j) {
      random_vars(j, i) = gen();
    }
  }

  return lower_cholesky * random_vars + means;
}
}  // namespace numeric_utils
