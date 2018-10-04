#include <utility>

// Boost distribution types
#include <boost/math/distributions/arcsine.hpp>
#include <boost/math/distributions/bernoulli.hpp>
#include <boost/math/distributions/beta.hpp>
#include <boost/math/distributions/binomial.hpp>
#include <boost/math/distributions/cauchy.hpp>
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/complement.hpp>
#include <boost/math/distributions/detail>
#include <boost/math/distributions/exponential.hpp>
#include <boost/math/distributions/extreme_value.hpp>
#include <boost/math/distributions/find_location.hpp>
#include <boost/math/distributions/find_scale.hpp>
#include <boost/math/distributions/fisher_f.hpp>
#include <boost/math/distributions/fwd.hpp>
#include <boost/math/distributions/gamma.hpp>
#include <boost/math/distributions/geometric.hpp>
#include <boost/math/distributions/hyperexponential.hpp>
#include <boost/math/distributions/hypergeometric.hpp>
#include <boost/math/distributions/inverse_chi_squared.hpp>
#include <boost/math/distributions/inverse_gamma.hpp>
#include <boost/math/distributions/inverse_gaussian.hpp>
#include <boost/math/distributions/laplace.hpp>
#include <boost/math/distributions/logistic.hpp>
#include <boost/math/distributions/lognormal.hpp>
#include <boost/math/distributions/negative_binomial.hpp>
#include <boost/math/distributions/non_central_beta.hpp>
#include <boost/math/distributions/non_central_chi_squared.hpp>
#include <boost/math/distributions/non_central_f.hpp>
#include <boost/math/distributions/non_central_t.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/pareto.hpp>
#include <boost/math/distributions/poisson.hpp>
#include <boost/math/distributions/rayleigh.hpp>
#include <boost/math/distributions/skew_normal.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions/text.txt>
#include <boost/math/distributions/triangular.hpp>
#include <boost/math/distributions/uniform.hpp>
#include <boost/math/distributions/weibull.hpp>
// Boost random generator
#include <boost/random/variate_generator.hpp>
// Eigen dense matrix
#include <Eigen/Dense>

namespace numeric_utils {

template <T generator, T distribution>
Eigen::Matrix<double, Dynamic, Dynamic> multivariate_random(
    const Eigen::VectorXd& means, const Eigen::MatrixXd& cov, int seed,
    unsigned int cases) {
  // Construct random number generator based on template arguments
  auto gen(generator(seed), distribution());
  auto lower_cholesky = cov.llt().matrixL();

  Eigen::Matrix<double, cov.rows(), cases> random_vars;

  for (unsigned int i = 0; i < random_vars.cols(); ++i) {
    for (unsigned int j = 0; j < random_vars.rows(); ++j) {
      random_vars(j, i) = gen();
    }
  }

  return lower_cholesky * random_vars;
}
}  // namespace numeric_utils
