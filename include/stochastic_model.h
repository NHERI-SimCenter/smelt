#ifndef _STOCHASTIC_MODEL_H_
#define _STOCHASTIC_MODEL_H_

#include <string>

#include "??????JSON_WRAPPER.h"

namespace stochastic {

/**
 * Abstract base class for stochastic models
 */
class StochasticModel {
 public:
  /**
   * @constructor Default constructor
   */
  StochasticModel() = default;

  /**
   * @constructor Constructor taking model arguments
   * @tparam Args Parameter pack containing arguments for stochastic model
   */
  template <typename... Targs>
  StochasticModel(Targs&&... args);

  /**
   * @destructor Virtual destructor
   */
  virtual ~StochasticModel();

  /**
   * Delete copy constructor
   */
  StochasticModel(const StochasticModel&) = delete;
  
  /**
   * Delete assignment operator
   */
  StochasticModel& operator=(const StochasticModel&) = delete;

  /**
   * Get the name of the stochastic model
   * @return Model name as a string
   */
  std::string model_name() const;

  /**
   * Generate loading based on stochastic model
   */
  virtual void generate() = 0;

  /**
   * Get the results of the stochastic load generation
   * @return Desired stochastic results in JSON format
   */
  ? ? ? JSON results() const;

 protected:
  std::string model_name_; /**< Name of stochastic model */
}
}  // namespace stochastic

#include "stochastic_model.tcc"

#endif  // _STOCHASTIC_MODEL_H_
