#include <functional>
#include <iostream>
#include <vector>
#include <catch/catch.hpp>
#include <Eigen/Dense>
#include "function_dispatcher.h"

/**
 * Check the dispatcher class implementation
 */
TEST_CASE("Check the dispatcher class implementation", "[Dispatcher]") {

  
  SECTION("Create dispatcher at compile time") {
    std::function<double(int)> add_small_number = [](int input) -> double {
      return static_cast<double>(input) + 0.01;
    };

    std::function<std::vector<double>(const std::vector<double> &, double)>
        multiply_vector = [](const std::vector<double> input,
                             double multiplier) -> std::vector<double> {
      std::vector<double> output(input.size());
      
      for (unsigned int i = 0; i < input.size(); ++i) {
	output[i] = input[i] * multiplier;
      };

      return output;
    };

    DispatchRegister<double, int> add_small_number_func("AddSmallNumber", add_small_number);
    DispatchRegister<std::vector<double>, std::vector<double>, double>
        vector_mull("MultiplyVecs", multiply_vector);

    // Create dispatchers
    REQUIRE(Dispatcher<double, int>::dispatcher()->check("AddSmallNumber") == true);
    REQUIRE(Dispatcher<std::vector<double>, std::vector<double>, double>::dispatcher()->check("MultiplyVecs"));
    
    // Check correct functions are called
    auto answer_1 =
        Dispatcher<double, int>::dispatcher()->dispatch("AddSmallNumber", 3);
    REQUIRE(answer_1 == Approx(3.0 + 0.01).epsilon(0.01));    
    
    std::vector<double> input_vector{0.0, 1.0, 2.0, 3.0};
    double multiplier = 2.0;
    auto answer_2 = Dispatcher<std::vector<double>, std::vector<double>,
                               double>::dispatcher()
                        ->dispatch("MultiplyVecs", input_vector, multiplier);

    for (unsigned int i = 0; i < input_vector.size(); ++i) {
      REQUIRE(input_vector[i] * multiplier ==
              Approx(answer_2[i]).epsilon(0.01));
    }
    
    // Create a non-existant element
    REQUIRE(
        Dispatcher<std::vector<double>, std::vector<double>, double>::dispatcher()
            ->check("NoFunc") == false);
    try {
      auto no_func = Dispatcher<std::vector<double>, std::vector<double>,
                                double>::dispatcher()
                         ->dispatch("NoFunc", std::vector<double>(), 1.0);
    } catch (std::exception &exception) {
      std::cout << "Dispatcher error: " << exception.what() << std::endl;
    }
  }
}
