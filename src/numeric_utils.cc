#include <complex>
#include <stdexcept>
#include <Eigen/Dense>
#include <mkl.h>
#include <mkl_dfti.h>
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

bool inverse_fft(std::vector<std::complex<double>> input_vector,
                 std::vector<double>& output_vector) {
  output_vector.resize(input_vector.size());

  // Create task descriptor and MKL status
  DFTI_DESCRIPTOR_HANDLE fft_descriptor;
  MKL_LONG fft_status;

  // Allocate the descriptor data structure and initializes it with default
  // configuration values
  fft_status = DftiCreateDescriptor(&fft_descriptor, DFTI_DOUBLE, DFTI_REAL, 1,
                                input_vector.size());
  if (fft_status != DFTI_NO_ERROR) {
    throw std::runtime_error(
        "\nERROR: in numeric_utils::inverse_fft: Error in descriptor creation\n");
    return false;
  }
  
  // Set configuration value to not do inplace transformation
  fft_status = DftiSetValue(fft_descriptor, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
  if (fft_status != DFTI_NO_ERROR) {
    throw std::runtime_error(
        "\nERROR: in numeric_utils::inverse_fft: Error in setting configuration\n");
    return false;
  }

  // Set the backward scale factor to be 1 divided by the size of the input vector
  // to make the backward tranform the inverse of the forward transform
  fft_status = DftiSetValue(fft_descriptor, DFTI_BACKWARD_SCALE,
                            static_cast<double>(1.0 / input_vector.size()));
  if (fft_status != DFTI_NO_ERROR) {
    throw std::runtime_error(
        "\nERROR: in numeric_utils::inverse_fft: Error in setting backward "
        "scale factor\n");
    return false;
  }  

  // Perform all initialization for the actual FFT computation
  fft_status = DftiCommitDescriptor(fft_descriptor);
  if (fft_status != DFTI_NO_ERROR) {
    throw std::runtime_error(
        "\nERROR: in numeric_utils::inverse_fft: Error in committing descriptor\n");
    return false;
  }
  
  // Compute the backward FFT
  fft_status = DftiComputeBackward(fft_descriptor, input_vector.data(),
                                   output_vector.data());
  if (fft_status != DFTI_NO_ERROR) {
    throw std::runtime_error(
        "\nERROR: in numeric_utils::inverse_fft: Error in computing backward FFT\n");
    return false;
  }
  
  // Free the memory allocated for descriptor
  fft_status = DftiFreeDescriptor(&fft_descriptor);
  if (fft_status != DFTI_NO_ERROR) {
    throw std::runtime_error(
        "\nERROR: in numeric_utils::inverse_fft: Error in freeing FFT descriptor\n");
    return false;
  }  

  return true;
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
