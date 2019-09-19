#include <cmath>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>
#include <ipps.h>

// Eigen dense matrices
#include <Eigen/Dense>

namespace signal_processing {

std::function<std::vector<std::vector<double>>(int, double)> hp_butterworth() {
  return [](int filter_order,
            double cutoff_freq) -> std::vector<std::vector<double>> {
    // Allocated memory for coefficients
    std::vector<Ipp64f> taps(2 * (filter_order + 1));
    IppStatus status = ippStsNoErr;
    int internal_buffer_size;

    // Calculate required buffer size for internal calculations
    status = ippsIIRGenGetBufferSize(filter_order, &internal_buffer_size);
    if (status != ippStsNoErr) {
      throw std::runtime_error(
          "\nERROR: in signal_processing::hp_butterworth: Error in buffer size "
          "calculations\n");
    }

    // Divide by 2 to make cutoff frequency match the definition given in MATLAB
    Ipp8u* internal_calcs = ippsMalloc_8u(internal_buffer_size);
    status =
        ippsIIRGenHighpass_64f(cutoff_freq / 2.0, 0, filter_order, taps.data(),
                               ippButterworth, internal_calcs);

    // Check if filter computation succeeded
    if (status != ippStsNoErr) {
      throw std::runtime_error(
          "\nERROR: in signal_processing::hp_butterworth: Error in coefficient "
          "calculations\n");
    }

    std::vector<double> numerator(filter_order + 1);
    std::vector<double> denominator(filter_order + 1);

    for (int i = 0; i < filter_order + 1; ++i) {
      numerator[i] = taps[i];
      denominator[i] = taps[i + filter_order + 1];
    }

    // Free memory associated with internal calcs
    ippsFree(internal_calcs);

    return std::vector<std::vector<double>>{numerator, denominator};
  };
}

std::function<std::vector<double>(std::vector<double>, std::vector<double>, int,
                                  int)>
    impulse_response() {
  return [](const std::vector<double>& numerator_coeffs,
            const std::vector<double>& denominator_coeffs, int order,
            int num_samples) -> std::vector<double> {
    if (numerator_coeffs.size() != denominator_coeffs.size()) {
      throw std::runtime_error(
          "\nERROR: in signal_processing::impulse_response: Inputs for "
          "numerator "
          "and denominator coefficients not same length\n");
    }

    IppStatus status = ippStsNoErr;
    int internal_buffer_size;
    IppsIIRState_64f* filter_state = nullptr;
    Ipp64f *samples = ippsMalloc_64f(num_samples),
           *impulse = ippsMalloc_64f(num_samples);
    std::vector<double> taps(numerator_coeffs.size() +
                             denominator_coeffs.size());

    // Set all values to zero except first one for impulse
    impulse[0] = 1.0;
    for (int i = 1; i < num_samples; ++i) {
      impulse[i] = 0.0;
    }

    // Put filter coefficients into single stack array
    for (unsigned int i = 0; i < numerator_coeffs.size(); ++i) {
      taps[i] = numerator_coeffs[i];
      taps[i + numerator_coeffs.size()] = denominator_coeffs[i];
    }

    // Get buffer size required for internal calcs
    status = ippsIIRGetStateSize_64f(order, &internal_buffer_size);
    if (status != ippStsNoErr) {
      throw std::runtime_error(
          "\nERROR: in signal_processing::impulse_response: Error in buffer "
          "size "
          "calculations\n");
    }

    // Allocate memory for internal calcs
    Ipp8u* internal_calcs = ippsMalloc_8u(internal_buffer_size);

    // Initialize filter state
    status = ippsIIRInit_64f(&filter_state, taps.data(), order, nullptr,
                             internal_calcs);
    if (status != ippStsNoErr) {
      throw std::runtime_error(
          "\nERROR: in signal_processing::impulse_response: Error in filter "
          "initialization\n");
    }

    // Apply filter to impulse
    status = ippsIIR_64f(impulse, samples, num_samples, filter_state);
    if (status != ippStsNoErr) {
      throw std::runtime_error(
          "\nERROR: in signal_processing::impulse_response: Error in filter "
          "application\n");
    }

    std::vector<double> sample_vec(num_samples);
    for (int i = 0; i < num_samples; ++i) {
      sample_vec[i] = samples[i];
    }

    // Free memory used for filtering
    ippsFree(samples);
    ippsFree(impulse);
    ippsFree(internal_calcs);

    return sample_vec;
  };
}

std::function<std::vector<double>(double, unsigned int, unsigned int)>
    acausal_highpass_filter() {
  return [](double freq_cutoff_norm, unsigned int order,
            unsigned int num_samples) -> std::vector<double> {
    // Initialize filter and frequencies
    Eigen::VectorXd filter(num_samples);
    std::vector<double> freq_steps(static_cast<unsigned int>(num_samples / 2) +
                                   1);
    double step_freq = freq_cutoff_norm / static_cast<double>(num_samples / 2);

    // Create vector of frequencies ranging from 0 to normalized cutoff
    // frequency
    for (unsigned int i = 0; i < freq_steps.size(); ++i) {
      freq_steps[i] = static_cast<double>(i) * step_freq;
    }

    // Calculate first half of filter coefficients
    for (unsigned int i = 0; i < freq_steps.size(); ++i) {
      filter(i) =
          std::sqrt(1.0 / (1.0 + std::pow(freq_cutoff_norm / freq_steps[i],
                                          2.0 * order)));
    }

    // Mirror coefficients
    Eigen::VectorXd highpass_filter(2 * filter.size() - 2);
    highpass_filter.head(filter.size()) = filter;
    highpass_filter.segment(filter.size(), filter.size() - 2) =
        filter.segment(1, filter.size() - 2).reverse();

    // Place filter coefficients in STL vector
    std::vector<double> filter_vector(highpass_filter.size());
    Eigen::VectorXd::Map(&filter_vector[0], highpass_filter.size()) =
        highpass_filter;

    return filter_vector;
  };
}
}  // namespace signal_processing
