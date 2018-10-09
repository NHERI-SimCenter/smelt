#ifndef _NUMERIC_UTILS_H_
#define _NUMERIC_UTILS_H_

#include <ctime>
#include <utility>
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
 * Abstract base class for random number generators
 */
class RandomGenerator {
 public:
  /**
   * @constructor Default constructor
   */
  RandomGenerator() = default;

  /**
   * @destructor Virtual destructor
   */
  virtual ~RandomGenerator() {};

  /**
   * Delete copy constructor
   */
  RandomGenerator(const RandomGenerator&) = delete;

  /**
   * Delete assignment operator
   */
  RandomGenerator& operator=(const RandomGenerator&) = delete;

 /**
  * Get multivariate random realization
  * @param[in] means Vector of mean values for random variables
  * @param[in] cov Covariance matrix of for random variables
  * @param[in] cases Number of cases to generate
  * @return Matrix of variable realizations
  */
  virtual Eigen::MatrixXd generate(const Eigen::VectorXd& means,
                                   const Eigen::MatrixXd& cov,
                                   unsigned int cases = 1) = 0;

  /**
   * Get the class name
   * @return Class name
   */
   virtual std::string name() const = 0;
  
 protected:
  int seed_ = static_cast<int>(
      std::time(nullptr)); /**< Seed value to use in random number generator */
};
}  // namespace numeric_utils

#endif  // _NUMERIC_UTILS_H_
