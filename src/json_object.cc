#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
// JSON for Modern C++ single-include header
#include <nlohmann_json/json.hpp>
#include "json_object.h"

bool utilities::JsonObject::delete_key(const std::string& key) {
  bool status = true;
  
  try {
    json_object_.erase(key);    
  } catch (const std::exception& e) {
    std::cerr << e.what();
    status = false;
    throw;
  }

  return status;
}

bool utilities::JsonObject::write_to_file(
    const std::string& output_location) const {
  bool status = true;
  std::ofstream output_file;
  output_file.open(output_location);

  if (!output_file.is_open()) {
    status = false;
    throw std::runtime_error(
        "\nERROR: In utilities::JsonObject::write_to_file(): Could not open "
        "output location\n");
  }

  // Write prettyfied JSON to file
  output_file << std::setw(4) << json_object_ << std::endl;

  output_file.close();

  if (output_file.fail()) {
    status = false;
    throw std::runtime_error(
        "\nERROR: In utilities::JsonObject::write_to_file(): Error when "
        "closing output location\n");
  }

  return status;
}

void utilities::JsonObject::clear() {
  json_object_.clear();
}

bool utilities::JsonObject::is_empty() const {
  return json_object_.empty();
}

unsigned int utilities::JsonObject::get_size() const {
  return json_object_.size();
}

void utilities::JsonObject::add_as_value(const std::string& key,
                                         JsonObject& receiving_object) const {
  receiving_object.add_value(key, json_object_);
}
