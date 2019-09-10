#ifndef _NELDER_MEAD_H_
#define _NELDER_MEAD_H_

#include <vector>
#include <Eigen/Dense>

/**
 * Optimization utilities
 */
namespace optimization {

/**
 * Class that implements Nelder-Mead algorithm for multidimensional
 * unconstrained optimization
 */
class NelderMead {
 public:
  /**
   * @constructor Default constructor
   */
  NelderMead() = default;

  /**
   * @constructor Construct with input function tolerance
   * @param[in] function_tolerance
   */
  NelderMead(double function_tolerance);  

  /**
   * @destructor Virtual destructor
   */
  virtual ~NelderMead() {};

  /**
   * Delete copy constructor
   */
  NelderMead(const NelderMead&) = delete;

  /**
   * Delete assignment operator
   */
  NelderMead& operator=(const NelderMead&) = delete;
  
 private:
  double function_tol_; /**< Function tolerance for convergence */
  unsigned int num_evals_; /**< Number of function evaluations */
  unsigned int num_dimensions_; /**< Number of dimensions */
  double func_min_; /**< Objective function minimum */
  Eigen::VectorXd func_vals_; /**< Function values at vertices */
  Eigen::MatrixXd simplex_; /**< Current simplex */
};
}  // namespace numeric_utils

#endif  // _NELDER_MEAD_H_
