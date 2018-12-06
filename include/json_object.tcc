#include <iostream>

/**< Add value to key */
template <typename Tparam>
bool utilities::JsonObject::add_value(const std::string& key,
                                      const Tparam& value) {
  bool status = true;
    
  try {
    if (json_object_.find(key) != json_object_.end()) {
      json_object_.at(key) = {json_object_.at(key), value};
      } else {
      json_object_.emplace(key, value);      
    }
  } catch (const std::exception& e) {
    std::cerr << e.what();
    status = false;
    throw;
  }

  return status;
};

/**< Get value at input key */
template <typename Tparam>
Tparam utilities::JsonObject::get_value(const std::string& key) const {
  return json_object_.at(key);
};
