/**< Add value to value to key */
template <typename Tparam>
void utilities::JsonObject::add_value(const std::string& key,
                                      const Tparam& value) {
  bool status = true;
  
  try {
    json_object_.emplace(key, value);    
  } catch (const std::exception& e) {
    std::cerr << e.what();
    status = false;
    throw;
  }

  return status;
}
