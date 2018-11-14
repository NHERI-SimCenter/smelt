#ifndef _FILTER_H_
#define _FILTER_H_

#include <stdexcept>
#include <vector>
#include <ipps.h>

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
        [](unsigned int filter_order,
           double cutoff_freq) -> std::vector<std::vector<double>> {
  // Allocated memory for coefficients
  Ipp64f taps[2 * (filter_order + 1)];

  IppStatus status = ippStsNoErr;

  // Divide by 2 to make cutoff frequency match the definition given in MATLAB

  status = ippsIIRGenHighpass(cutoff_freq / 2.0, 0, filter_order, taps,
                              ippButterworth);

  // Check if filter computation succeeded
  if (status != ippStsNoErr) {
    throw std::runtime_error(
        "\nERROR: in signal_processing::hp_butterworth: Error in coefficient "
        "calculations\n");
  }

  std::vector<double> numerator(filter_order + 1);
  std::vector<double> denominator(filter_order + 1);

  for (unsigned int i = 0; i < filter_order + 1; ++i) {
    numerator[i] = taps[i];
    denominator[i] = taps[i + filter_order + 1];
  }

  return std::vector<std::vector<double>{numerator, denominator};
};
}  // namespace signal_processing

#endif  // _FILTER_H_
