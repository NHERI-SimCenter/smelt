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

/**< Template specialization for case when input value in JsonObject */
template <>
bool utilities::JsonObject::add_value(const std::string& key,
                                      const utilities::JsonObject& value) {
  bool status = true;
  
  try {
    add_value<nlohmann::json>(key, value.get_library_json());
  } catch (const std::exception& e) {
    std::cerr << e.what();
    status = false;
    throw;    
  }

  return status;
}

/**< Get value at input key */
template <typename Tparam>
Tparam utilities::JsonObject::get_value(const std::string& key) const {
  return json_object_.at(key);
};

/**< Template specialization for case when return value is JsonObject */
template <>
utilities::JsonObject utilities::JsonObject::get_value(
    const std::string& key) const {
  /* nlohmann::json json_object_value = json_object_.at(key); */

  return JsonObject(json_object_.at(key));
}
