#ifndef _JSON_OBJECT_H_
#define _JSON_OBJECT_H_

// JSON for Modern C++ single-include header
#include <nlohmann_json/json.hpp>

namespace utilities {
// Alias for JSON type
using json = nlohmann::json;
  
/**
 * Wrapper class for JSON implementation
 */
class JsonObject {
 public:
  /**
   * @constructor Construct JSON wrapper
   */
  JsonObject() = default;

  /**
   * @destructor Virtual destructor
   */
  virtual ~JsonObject() {};

  /**
   * Add key to JSON object
   * @param[in] key Key to add to JSON object
   * @return Returns true if successful, false otherwise
   */
  bool add_key(const std::string& key);

  /**
   * Add value to key in JSON object. If key doesn't exist,
   * it will automatically added.
   * @tparam Tparam Template parameter for value
   * @param[in] key Key to add values to
   * @param[in] value Value to add to key
   * @return Returns true if successful, false otherwise
   */
  template <typename Tparam>
  bool add_value(const std::string& key, const Tparam& value);

  /**
   * Delete key from JSON object
   * @param[in] key Key to remove from JSON object
   * @return Returns true if successful, false otherwise
   */
  bool delete_key(const std::string& key);

  /**
   * Write prettified JSON object to file
   * @param[in] output_location Location to write JSON object to
   * @return Returns true if successful, false otherwise
   */
  bool write_to_file(const std::string& output_location) const;

 protected:
  json json_object_; /**< Json object for this class */  
};
}  // namespace utilities

#include "json_object.tcc"

#endif  // _JSON_OBJECT_H_
