#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <cmath>
#include <string>
#include <vector>

namespace signal_processing {

/**
 * Abstract base class for window functions. Implemented using classes to allow
 * for easy extension without having to change implementation to account for
 * different function dispatch.
 */
class Window {
 public:
  /**
   * @constructor Default constructor
   */
  Window() = default;

  /**
   * @destructor Virtual destructor
   */
  virtual ~Window(){};

  /**
   * Delete copy constructor
   */
  Window(const Window&) = delete;

  /**
   * Delete assignment operator
   */
  Window& operator=(const Window&) = delete;

  /**
   * Get the name of the window
   * @return Window name as string
   */
  virtual std::string name() const = 0;

  /**
   * Get the window for the input window_length
   * @param[in] window_length Desired length of window
   * @return Vector filled with window function evaluations based on input
   * window length
   */
  virtual std::vector<double> get_window(unsigned int window_length) const = 0;
};
}  // namespace signal_processing

#endif  // _WINDOW_H_
