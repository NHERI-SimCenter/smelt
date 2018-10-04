#include <string>

#include "?????JSON_WRAPPER.h"
#include "distribution.h"

stochastic::Distribution::~Distribution(){};

std::string stochastic::Distribution::model_name() const {
  return model_name_;
};
