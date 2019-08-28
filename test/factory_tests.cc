#include <iostream>
#include <catch2/catch.hpp>
#include <Eigen/Dense>
#include "factory.h"
#include "normal_multivar.h"
#include "numeric_utils.h"

class BaseClass {
public:
  // Default constructor
  BaseClass() = default;
  // Get class name
  virtual std::string name() const = 0;
};

class DerivedClass : public BaseClass {
public:
  // Default constructor
  DerivedClass() = default;
  // Get class name
  virtual std::string name() const { return "DerivedClass"; };
};

class AnotherDerivedClass : public BaseClass {
public:
  // Default constructor
  AnotherDerivedClass() = default;
  // Get class name
  virtual std::string name() const { return "AnotherDerivedClass"; };
};

// Register classes
static Register<BaseClass, DerivedClass> derived("DerivedClass");
static Register<BaseClass, AnotherDerivedClass> another("AnotherDerivedClass");

namespace numeric_utils {
class TestingClass : public RandomGenerator {
public:
  TestingClass() : RandomGenerator() {};

  TestingClass(int seed) : RandomGenerator(){};

  virtual ~TestingClass(){};

  bool generate(
      Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &random_numbers,
      const Eigen::VectorXd &means, const Eigen::MatrixXd &cov,
      unsigned int cases = 1) override {
    return true;
  };

  std::string name() const override {
    return "TestingClass";
  };  
};  
}


/**
 * Check the factory class implementation
 */
TEST_CASE("Check the factory class implementation", "[Factory]") {
  // Seed value for repeatability
  int seed = 100;
  
  SECTION("Create factory at compile time") {
    // Create derived class
    REQUIRE(Factory<BaseClass>::instance()->check("DerivedClass") == true);
    auto derived_class = Factory<BaseClass>::instance()->create("DerivedClass");
    REQUIRE(derived_class->name() == "DerivedClass");

    // Create another derived class
    REQUIRE(Factory<BaseClass>::instance()->check("AnotherDerivedClass") ==
            true);
    auto another_derived_class =
        Factory<BaseClass>::instance()->create("AnotherDerivedClass");
    REQUIRE(another_derived_class->name() == "AnotherDerivedClass");

    // Create a non-existant element
    REQUIRE(Factory<BaseClass>::instance()->check("Garbage") == false);
    try {
      auto garbage = Factory<BaseClass>::instance()->create("Garbage");
    } catch (std::exception &exception) {
      std::cout << "Factory error: " << exception.what() << std::endl;
    }
  }

  SECTION("Extend library when linked") {
    // Register class with factory
    Register<numeric_utils::RandomGenerator, numeric_utils::TestingClass, int>
        testing_multivar("TestingClass");

    // Create newly registered derived class
    REQUIRE(Factory<numeric_utils::RandomGenerator, int>::instance()->check("TestingClass") == true);
    auto testing_class = Factory<numeric_utils::RandomGenerator, int>::instance()
      ->create("TestingClass", std::move(seed));
    REQUIRE(testing_class->name() == "TestingClass");    
  }
}
