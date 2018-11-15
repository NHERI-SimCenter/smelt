#ifndef _FILTER_H_
#define _FILTER_H_

#include <stdexcept>
#include <vector>
#include <intel_ipp/include/ipps.h>

/**
 * Signal processing functionality
 */
namespace signal_processing {

/**
 * Function for calculating the coefficients of the highpass Butterworth filter
 * @param[in] filter_order Order of the Butterworth filter
 * @param[in] cuttoff_freq Normalized cutoff frequency
 * @return Returns a vector containing two vectors where the first vector
 *         contains and numerator coefficients and the second vector contains
 *         the denominator coefficients.
 */
std::function<std::vector<std::vector<double>>(unsigned int, double)>
    hp_butterworth =
        [](int filter_order,
           double cutoff_freq) -> std::vector<std::vector<double>> {
  // Allocated memory for coefficients
  Ipp64f taps[2 * (filter_order + 1)];
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
  Ipp8u * internal_calcs = ippsMalloc_8u(internal_buffer_size);
  status = ippsIIRGenHighpass_64f(cutoff_freq / 2.0, 0, filter_order, taps,
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

  ippsFree(internal_calcs);
  return std::vector<std::vector<double>>{numerator, denominator};
};
}  // namespace signal_processing

#endif  // _FILTER_H_
