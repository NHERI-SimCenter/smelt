#include <stdexcept>
#include <Eigen/Dense>
#include <mkl.h>
#include <mkl_vsl.h>
#include "numeric_utils.h"

namespace numeric_utils {
Eigen::MatrixXd corr_to_cov(const Eigen::MatrixXd& corr,
                            const Eigen::VectorXd& std_dev) {
  Eigen::MatrixXd cov_matrix = Eigen::MatrixXd::Zero(corr.rows(), corr.cols());

  for (unsigned int i = 0; i < cov_matrix.rows(); ++i) {
    for (unsigned int j = 0; j < cov_matrix.cols(); ++j) {
      cov_matrix(i, j) = corr(i, j) * std_dev(i) * std_dev(j);
    }
  }

  return cov_matrix;
}
  
bool convolve_1d(const std::vector<double>& input_x,
                 const std::vector<double>& input_y,
                 std::vector<double>& response) {
  bool status = true;
  response.resize(input_x.size() + input_y.size() - 1);

  // Create convolution status and task pointer
  int conv_status;
  VSLConvTaskPtr conv_task;
  // Construct convolution task, with solution mode set to direct
  conv_status =
      vsldConvNewTask1D(&conv_task, VSL_CONV_MODE_DIRECT, input_x.size(),
                        input_y.size(), response.size());

  // Check if convolution construction was successful
  if (conv_status != VSL_STATUS_OK) {
    throw std::runtime_error(
        "\nERROR: in numeric_utils::convolve_1d: Error in convolution "
        "construction\n");
    status = false;
  }

  // Set convolution to start at first element in input_y
  vslConvSetStart(conv_task, 0);

  // Execute convolution
  conv_status = vsldConvExec1D(conv_task, input_x.data(), 1, input_y.data(), 1,
                               response.data(), 1);

  // Check if convolution exectution was successful
  if (conv_status != VSL_STATUS_OK) {
    throw std::runtime_error(
        "\nERROR: in numeric_utils::convolve_1d: Error in convolution "
        "execution\n");
    status = false;
  }

  // Delete convolution task
  vslConvDeleteTask(&conv_task);

  return status;
}

double trapazoid_rule(const std::vector<double>& input_vector, double spacing) {
  double result = (input_vector[0] + input_vector[input_vector.size() - 1]) / 2.0;

  for (unsigned int i = 1; i < input_vector.size() - 1; ++i) {
    result = result + input_vector[i];
  }

  return result * spacing;
}

double trapazoid_rule(const Eigen::VectorXd& input_vector, double spacing) {
  double result = (input_vector[0] + input_vector[input_vector.size() - 1]) / 2.0;

  for (unsigned int i = 1; i < input_vector.size() - 1; ++i) {
    result = result + input_vector[i];
  }

  return result * spacing;
}  
}  // namespace numeric_utils
