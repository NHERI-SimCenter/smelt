#include <cmath>
#include <iostream>
#include <vector>
#include <catch/catch.hpp>
#include "numeric_utils.h"

TEST_CASE("Test one dimensional convolution", "[Helpers][Convolution]") { 
  SECTION("One dimensional convolution of vectors with length 1") {
    std::vector<double> input_x{1.0};
    std::vector<double> input_y{2.0};
    std::vector<double> response;

    bool status;
    try {
      status = numeric_utils::convolve_1d(input_x, input_y, response);
    } catch (std::exception &exception) {
      std::cout << "Convolution error: " << exception.what() << std::endl;      
      FAIL("One dimensional convolution function throws exception, check where "
           "exception is being generated interally");
    }

    REQUIRE(status);
    REQUIRE(response.size() == 1);
    REQUIRE(response[0] == Approx(2.0).epsilon(0.01));
  }

  SECTION("One dimensional convolution of vectors with length 2 and 3") {
    std::vector<double> input_x{3.0, 4.0, 5.0};
    std::vector<double> input_y{2.0, 1.0};
    std::vector<double> response(1);

    bool status;
    try {
      status = numeric_utils::convolve_1d(input_x, input_y, response);
    } catch (std::exception &exception) {
      std::cout << "Convolution error: " << exception.what() << std::endl;      
      FAIL("One dimensional convolution function throws exception, check where "
           "exception is being generated interally");
    }

    REQUIRE(status);
    REQUIRE(response.size() == input_x.size() + input_y.size() - 1);
    REQUIRE(response[0] == Approx(6.0).epsilon(0.01));
    REQUIRE(response[1] == Approx(11.0).epsilon(0.01));
    REQUIRE(response[2] == Approx(14.0).epsilon(0.01));
    REQUIRE(response[3] == Approx(5.0).epsilon(0.01));
  }  
}

TEST_CASE("Test trapazoid rule", "[Helpers][Trapazoid]") {

  SECTION("Input vector with unit spacing") {
    std::vector<double> input_vector{1, 4, 9, 16, 25};

    auto integral = numeric_utils::trapazoid_rule(input_vector, 1.0);
    REQUIRE(integral == 42);
  }

  SECTION("Input vector with non-unit spacing") {
    std::vector<double> input_vector(101, 0.0);

    double accumulator = 0.0;
    for (unsigned int i = 1; i < input_vector.size(); ++i) {
      accumulator += M_PI / 100.0;
      input_vector[i] = std::sin(accumulator);
    }

    auto integral = numeric_utils::trapazoid_rule(input_vector, M_PI / 100.0);
    REQUIRE(integral == Approx(1.9998).epsilon(0.01));
  }  
}
