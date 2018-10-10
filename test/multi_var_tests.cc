#include <catch/catch.hpp>
#include <Eigen/Dense>
#include "configure.h"
#include "factory.h"
#include "normal_multivar.h"
#include "numeric_utils.h"

TEST_CASE("Test generation of random numbers", "[RandomNumbers]") {
  // Initialize the factories
  config::initialize();
  // Seed value for repeatability
  int seed = 100;
  
  SECTION("Generate normally distributed random numbers for single random "
          "variable") {
    Eigen::VectorXd means(1);
    Eigen::MatrixXd cov(1,1);    
    means(0) = 1.789;
    cov(0, 0) = 0.0123;
    
    auto random_generator = Factory<numeric_utils::RandomGenerator, int>::instance()
      ->create("MultivariateNormal", std::move(seed));

    // Single random variable
    auto random_numbers = random_generator->generate(means, cov, 100);
    
    double average = 0.0;    
    for (unsigned int i = 0; i < random_numbers.size(); ++i) {
      average = average + random_numbers(i);
    }
    average = average / random_numbers.size();
    REQUIRE(average == Approx(means(0)).epsilon(0.01));
  }
}
