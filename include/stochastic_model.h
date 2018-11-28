#ifndef _STOCHASTIC_MODEL_H_
#define _STOCHASTIC_MODEL_H_

#include <string>
#include "json_wrapper.h"

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
  std::string model_name() const { return model_name_; };

  /**
   * Generate loading based on stochastic model
   * @return Json object containing acceleration time histories
   */
  utilities::JsonWrapper void generate() = 0;
}
}  // namespace stochastic

#include "stochastic_model.tcc"

#endif  // _STOCHASTIC_MODEL_H_
