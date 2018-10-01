#include <string>

#include "?????JSON_WRAPPER.h"
#include "stochastic_model.h"

stochastic::StochasticModel::~StochasticModel(){};

std::string stochastic::StochasticModel::model_name() const {
  return model_name_;
};

? ? ? JSON stochastic::StochasticModel::results() const {
  return JSON_FORMATTED_RESULTS;
}
