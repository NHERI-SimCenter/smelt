#include <vector>
#include "hann_window.h"

signal_processing::HannWindow::HannWindow()
  : Window()
{}

std::vector<double> signal_processing::HannWindow::get_window(
    unsigned int window_length) const {

  std::vector<double> hann(window_length, 0.0);
  double number_of_points = static_cast<double>(window_length - 1);

  for (unsigned int i = 0; i < hann.size(); ++i) {
    hann[i] = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / number_of_points));
  }

  return hann;
}
