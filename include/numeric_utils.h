#ifndef _NUMERIC_UTILS_H_
#define _NUMERIC_UTILS_H_

#include <ctime>
#include <Eigen/Dense>

/**
 * Numeric utility functions not tied to any particular class
 */
namespace numeric_utils {

/**
 * Convert input correlation matrix and standard deviation to covariance matrix
 * @param[in] corr Input correlation matrix
 * @param[in] std_dev Standard deviation vector
 * @return Covariance matrix with same dimensions as input correlation matrix
 */
Eigen::MatrixXd corr_to_cov(const Eigen::MatrixXd& corr,
                            const Eigen::VectorXd& std_dev);

/**
 * Generate multivariate random numbers for different distribution types
 * @param[in] means Input vector of mean values for random variables
 * @param[in] cov Input covariance matrix for random variables
 * @param[in] seed Seed value to use in random generator
 * @param[in] cases Number of cases to generate
 * @tparam generator Random number generator to use
 * @tparam distribution Distribution type to use for random number generation
 * @tparam Targs Parameter pack containing inputs for distribution
 * @return Matrix of random variables chosen from multivariate distribution
 */
template <T generator, T distribution>
Eigen::Matrix<double, Dynamic, Dynamic> multivariate_random(
    const Eigen::VectorXd& means, const Eigen::MatrixXd& cov,
    int seed = static_cast<int>(std::time(nullptr)), unsigned int cases = 1);
}  // namespace numeric_utils
}

#include "numeric_utils.tcc"

#endif  // _NUMERIC_UTILS_H_
