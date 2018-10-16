#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <vector>
#include <kfr/dsp/window.hpp>

namespace signal_processing {

enum class window_type : int {
  NONE = 0,
  HANN = 1
}; /**< Enum class for filter types */

template <class T = NONE>
std::vector<double> window(const std::vector<double>& data);

template std::vector<double> window<NONE>(const std::vector<double>& data) {
  return std::vector<double>(data.size(), 1.0);
};

template std::vector<double> window<HANN>(const std::vector<double>& data) {
  // Get Hann window based on data size
  kfr::univector<fbase, 64> output = kfr::window_hann(data.size());
  // Assign window to std::vector
  std::vector<double> hann;
  hann.assign(output.get_data(), output.get_data() + output.get_size());
  return hann;
};
}  // namespace signal_processing

#endif  // _WINDOW_HPP_
