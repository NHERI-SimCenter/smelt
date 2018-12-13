#include <string>
#include <vector>
#include <catch/catch.hpp>
#include <nlohmann_json/json.hpp>
#include "json_object.h"

TEST_CASE("Test JSON object wrapper", "[Helpers][Json]") { 
  SECTION("Test JSON object basic functionality") {
    utilities::JsonObject test_object;

    // Test that object is empty on construction
    REQUIRE(test_object.is_empty());
    // Should throw when deleting non-existent key
    REQUIRE_THROWS(test_object.delete_key("something"));

    // Add key and value
    test_object.add_value("Foo", 17);
    REQUIRE(test_object.get_value<int>("Foo") == 17);
    REQUIRE(!test_object.is_empty());

    // Delete key
    test_object.delete_key("Foo");
    REQUIRE(test_object.is_empty());
    REQUIRE_THROWS(test_object.get_value<int>("Foo"));

    // Add container as value
    std::vector<double> double_vec = {0.0, 1.0, 2.0};
    test_object.add_value("Vector", double_vec);
    test_object.add_value("String", "I'm a string");
    REQUIRE_THROWS(test_object.add_value("String", "Yet another string"));
    REQUIRE_THROWS(test_object.add_value("Vector", double_vec));
    REQUIRE(test_object.get_size() == 2);

    // Add JsonObject as value
    utilities::JsonObject insert_object;
    insert_object.add_value("Bar", true);
    test_object.add_value("JSON Object", insert_object);
    auto get_object = test_object.get_value<utilities::JsonObject>("JSON Object");
    REQUIRE(get_object.get_value<bool>("Bar"));
    REQUIRE(get_object != test_object);
    REQUIRE(get_object == insert_object);

    // Delete all entries
    test_object.clear();
    REQUIRE(test_object.is_empty());
  }
}
