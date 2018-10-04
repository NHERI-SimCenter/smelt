#include <Eigen/Dense>

#include "numeric_utils.h"

namespace numeric_utils {
Eigen::MatrixXd corr_to_cov(const Eigen::MatrixXd& corr,
                            const Eigen::MatrixXd& std_dev) {
  Eigen::MatrixXd cov_matrix = Eigen::MatrixXd::Zero(corr.rows(), corr.cols());

  for (unsigned int i = 0; i < cov_matrix.rows(); ++i) {
    for (unsigned int j = 0; j < cov_matrix.cols(); ++j) {
      cov_matrix(i, j) = corr(i, j) * std_dev(i) * std_dev(j);
    }
  }

  return cov_matrix;
}
}  // namespace numeric_utils
