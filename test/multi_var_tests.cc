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
  }
}
