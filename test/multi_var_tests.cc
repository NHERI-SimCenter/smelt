#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <catch/catch.hpp>
#include <Eigen/Dense>
#include "numeric_utils.h"

TEST_CASE("Test generation of multivariate random numbers", "[MultiVariate]") {

  SECTION("Generate normally distributed random numbers for single random "
          "variable") {
    unsigned int seed = 100;
    double mean = 1.789;
    double std_dev = 0.0123;
    Eigen::VectorXd means(1);
    means(0) = mean;
    Eigen::MatrixXd std_devs(1,1);
    std_devs(0, 0) = std_dev;

    // auto random_generator = numeric_utils::RandomGenerator(seed);
  }
}
