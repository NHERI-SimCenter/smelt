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
 * Templated class for random number generation
 * @tparam Generator Type of random number generator
 * @tparam Distribution Distribution type to use for random number generation
 */ 
template <typename Generator, template <typename> typename Distribution,
          typename RealType = double>
class RandomGenerator {
 public:

 /**
  * @constructor Default constructor that uses default value for seed values
  */
 RandomGenerator() = default;

 /**
  * @constructor Constructor that takes seed value as argument
  * @param[in] seed Value to use as seed for random number generator
  */
 RandomGenerator(int seed);

 /**
  * @destructor Default destructor
  */
 ~RandomGenerator() = default;

 /**
  * Delete copy constructor
  */
 RandomGenerator(const RandomGenerator&) = delete;

 /**
  * Delete assignment operator
  */
 RandomGenerator& operator=(const RandomGenerator&) = delete;

 /**
  * Get single variable random realization
  * @param[in] mean Mean of distribution
  * @param[in] std_dev Standard deviation of distribution
  * @return Random variable realization
  */
 double generate(double mean, double std_dev) const;

 /**
  * Get single variable random realizations
  * @param[in] mean Mean of distribution
  * @param[in] std_dev Standard deviation of distribution
  * @param[in] cases Number of cases to generate
  * @return Random variable realizations
  */
 Eigen::VectorXd generate(double mean, double std_dev,
                          unsigned int cases) const;

 /**
  * Get multivariate random realization
  * @param[in] means Vector of mean values for random variables
  * @param[in] cov Covariance matrix of for random variables
  * @param[in] cases Number of cases to generate
  * @return Matrix of variable realizations
  */
 Eigen::MatrixXd generate(const Eigen::VectorXd& means,
                          const Eigen::MatrixXd& cov,
                          unsigned int cases = 1) const;

private:
 int seed_ = static_cast<int>(std::time(nullptr)); /**< Seed value to use in random number generator */
 Generator generator_; /**< Random number generator type */
 Distribution distribution_; /**< Distribution type to use for random number generation */
 boost::variate_generator<Generator, Distribution<RealType> >
     var_generator_; /**< Random realization generator based on generator and
                        distribution */
};
}  // namespace numeric_utils

#include "numeric_utils.tcc"

#endif  // _NUMERIC_UTILS_H_
