#include <cmath>
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
  auto random_generator = Factory<numeric_utils::RandomGenerator, int>::instance()
    ->create("MultivariateNormal", std::move(seed));
  Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> random_numbers;  
  
  SECTION("Generate normally distributed random numbers for single random "
          "variable") {
    Eigen::VectorXd means(1);
    Eigen::MatrixXd cov(1,1);    
    means(0) = 1.789;
    cov(0, 0) = 0.0123;
    
    // Single random variable
    random_generator->generate(random_numbers, means, cov, 100);
    
    double average = 0.0;    
    for (unsigned int i = 0; i < random_numbers.size(); ++i) {
      average = average + random_numbers(i);
    }
    average = average / random_numbers.size();
    REQUIRE(average == Approx(means(0)).epsilon(0.01));
  }

  SECTION("Generate normally distributed random numbers for multiple "
          "uncorrelated random variables", "[RandomNumbers]") {
    Eigen::VectorXd means(4);
    Eigen::MatrixXd cov = Eigen::MatrixXd::Zero(4,4);
    means << 1.789, 0.01, -10012.7, 702;
    cov(0, 0) = 0.0123 * 0.0123;
    cov(1, 1) = 0.00005;
    cov(2, 2) = 50.0;
    cov(3, 3) = 25;

    // Random variable vector
    random_generator->generate(random_numbers, means, cov, 1000);

    std::vector<double> averages(means.size());
    for (unsigned int i = 0; i < random_numbers.cols(); ++i) {
      for (unsigned int j = 0; j < means.size(); ++j) {
	averages[j] = averages[j] + random_numbers(j, i);
      }
    }

    REQUIRE(averages[0] / random_numbers.cols() == Approx(means(0)).epsilon(0.01));
    REQUIRE(averages[1] / random_numbers.cols() == Approx(means(1)).epsilon(0.02));
    REQUIRE(averages[2] / random_numbers.cols() == Approx(means(2)).epsilon(0.01));
    REQUIRE(averages[3] / random_numbers.cols() == Approx(means(3)).epsilon(0.01));
  }

  SECTION("Generate normally distributed random numbers for correlated random "
          "variable", "[RandomNumbers]") {
    Eigen::VectorXd means(3);
    Eigen::MatrixXd cov = Eigen::MatrixXd::Zero(3, 3);
    means << 64.0, 300.0, 60.0;

    // Try bad COV matrix
    // clang-format off
    cov << 1.0, 1.0, 0.0, 
           1.0, 1.0, 1.0,
           0.0, 1.0, 1.0;    
    // clang-format on

    bool success = random_generator->generate(random_numbers, means, cov, 250000);
    REQUIRE(success == false);

    // Try good COV matrix
    // clang-format off   
    cov << 504.0, 360.0, 180.0, 
           360.0, 360.0, 0.0,
           180.0, 0.0, 720.0;
    // clang-format on
    success = random_generator->generate(random_numbers, means, cov, 250000);
    REQUIRE(success == true);    
    
    Eigen::VectorXd averages = Eigen::VectorXd::Zero(means.size());
    for (unsigned int i = 0; i < means.size(); ++i) {
      for (unsigned int j = 0; j < random_numbers.cols(); ++j) {
	averages(i) = averages(i) + random_numbers(i, j);
      }
    }

    averages = averages / random_numbers.cols();

    REQUIRE(averages(0) == Approx(means(0)).epsilon(0.01));
    REQUIRE(averages(1) == Approx(means(1)).epsilon(0.01));
    REQUIRE(averages(2) == Approx(means(2)).epsilon(0.01));

    // Compute covariance matrix from random values
    Eigen::MatrixXd deviation_scores =
        Eigen::MatrixXd::Zero(random_numbers.rows(), random_numbers.cols());

    for (unsigned int i = 0; i < random_numbers.cols(); ++i) {
      deviation_scores.col(i) = random_numbers.col(i) - averages;
    }

    Eigen::MatrixXd calculated_cov =
        (deviation_scores * deviation_scores.transpose()) / random_numbers.cols();

    REQUIRE(cov.lpNorm<2>() == Approx(calculated_cov.lpNorm<2>()).epsilon(0.01));
  }
}
