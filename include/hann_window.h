#ifndef _HANN_WINDOW_H_
#define _HANN_WINDOW_H_

#include <cmath>
#include <string>
#include <vector>
#include "window.h"

namespace signal_processing {

/**
 * Hann window
 */
class HannWindow : public Window {
 public:
  /**
   * @constructor Default constructor
   */
  HannWindow();

  /**
   * @destructor Virtual destructor
   */
  virtual ~HannWindow(){};

  /**
   * Delete copy constructor
   */
  HannWindow(const HannWindow&) = delete;

  /**
   * Delete assignment operator
   */
  HannWindow& operator=(const HannWindow&) = delete;

  /**
   * Get the name of the window
   * @return Window name as string
   */
  std::string name() const override { return "HannWindow"; };

  /**
   * Get the window for the input window_length
   * @param[in] window_length Desired length of window
   * @return Vector filled with window function evaluations based on input
   * window length
   */
  std::vector<double> get_window(unsigned int window_length) const override;
};
}  // namespace signal_processing

#endif  // _HANN_WINDOW_H_
