#include <cmath>
#include <iostream>
#include <vector>
#include <catch2/catch.hpp>
#include <Eigen/Dense>
#include "numeric_utils.h"

TEST_CASE("Test correlation to covariance functionality", "[Helpers]") {
  SECTION("Correlation is diagonal matrix with values of 1.0 along diagonal") {
    Eigen::MatrixXd test_corr = Eigen::MatrixXd::Zero(3, 3);
    test_corr(0, 0) = 1.0;
    test_corr(1, 1) = 1.0;
    test_corr(2, 2) = 1.0;
    Eigen::MatrixXd std_dev = Eigen::VectorXd::Ones(3);

    std_dev << 2.0, 3.0, 4.0;

    auto cov = numeric_utils::corr_to_cov(test_corr, std_dev);

    Eigen::MatrixXd expected_matrix = Eigen::MatrixXd::Zero(3, 3);
    expected_matrix(0, 0) = 4.0;
    expected_matrix(1, 1) = 9.0;
    expected_matrix(2, 2) = 16.0;

    REQUIRE(cov(0, 0) == expected_matrix(0, 0));
    REQUIRE(cov(1, 1) == expected_matrix(1, 1));
    REQUIRE(cov(2, 2) == expected_matrix(2, 2));
    REQUIRE(expected_matrix.lpNorm<2>() == Approx(cov.lpNorm<2>()).epsilon(0.01));
  }
}

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

  SECTION("STL vector with unit spacing") {
    std::vector<double> input_vector{1, 4, 9, 16, 25};

    auto integral = numeric_utils::trapazoid_rule(input_vector, 1.0);
    REQUIRE(integral == 42);
  }

  SECTION("STL vector with non-unit spacing") {
    std::vector<double> input_vector(101, 0.0);

    double accumulator = 0.0;
    for (unsigned int i = 1; i < input_vector.size(); ++i) {
      accumulator += M_PI / 100.0;
      input_vector[i] = std::sin(accumulator);
    }

    auto integral = numeric_utils::trapazoid_rule(input_vector, M_PI / 100.0);
    REQUIRE(integral == Approx(1.9998).epsilon(0.01));
  }

  SECTION("Eigen vector with unit spacing") {
    Eigen::VectorXd input_vector(5);
    input_vector << 1, 4, 9, 16, 25;

    auto integral = numeric_utils::trapazoid_rule(input_vector, 1.0);
    REQUIRE(integral == 42);
  }

  SECTION("Eigen vector with non-unit spacing") {
    Eigen::VectorXd input_vector = Eigen::VectorXd::Zero(101);

    double accumulator = 0.0;
    for (unsigned int i = 1; i < input_vector.size(); ++i) {
      accumulator += M_PI / 100.0;
      input_vector[i] = std::sin(accumulator);
    }

    auto integral = numeric_utils::trapazoid_rule(input_vector, M_PI / 100.0);
    REQUIRE(integral == Approx(1.9998).epsilon(0.01));
  }    
}

TEST_CASE("Test 1-D inverse Fast Fourier Transform", "[Helpers][FFT]") {
  SECTION("Calculate real portion of one-dimesional inverse FFT") {
    std::vector<std::complex<double>> input_vector = {
        {15.0, 0.0},
        {-2.5, 3.440954801177933},
        {-2.5, 0.812299240582266},
        {-2.5, -0.812299240582266},
        {-2.5, -3.440954801177933}};

    std::vector<double> output_vector(4);
    auto status = numeric_utils::inverse_fft(input_vector, output_vector);

    REQUIRE(status);
    REQUIRE(output_vector[0] == Approx(1.0).epsilon(0.01));
    REQUIRE(output_vector[1] == Approx(2.0).epsilon(0.01));
    REQUIRE(output_vector[2] == Approx(3.0).epsilon(0.01));
    REQUIRE(output_vector[3] == Approx(4.0).epsilon(0.01));
    REQUIRE(output_vector[4] == Approx(5.0).epsilon(0.01));
  }

  SECTION("Calculate real portion of one-dimesional inverse FFT") {
    Eigen::VectorXcd input_vector(5);
    input_vector << std::complex<double>(15.0, 0.0),
      std::complex<double>(-2.5, 3.440954801177933),
      std::complex<double>(-2.5, 0.812299240582266),
      std::complex<double>(-2.5, -0.812299240582266),
      std::complex<double>(-2.5, -3.440954801177933);

    Eigen::VectorXd output_vector;
    auto status = numeric_utils::inverse_fft(input_vector, output_vector);

    REQUIRE(status);
    REQUIRE(output_vector[0] == Approx(1.0).epsilon(0.01));
    REQUIRE(output_vector[1] == Approx(2.0).epsilon(0.01));
    REQUIRE(output_vector[2] == Approx(3.0).epsilon(0.01));
    REQUIRE(output_vector[3] == Approx(4.0).epsilon(0.01));
    REQUIRE(output_vector[4] == Approx(5.0).epsilon(0.01));
  }
}

TEST_CASE("Test 1-D Fast Fourier Transform", "[Helpers][FFT]") {
  SECTION("Calculate one-dimesional FFT") {
    std::vector<double> input_vector = {3.0, 1.0, 0.0, 0.0};

    std::vector<std::complex<double>> output_vector(4);
    auto status = numeric_utils::fft(input_vector, output_vector);

    REQUIRE(status);
    REQUIRE(real(output_vector[0]) == Approx(4.0).epsilon(0.01));
    REQUIRE(imag(output_vector[0]) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(real(output_vector[1]) == Approx(3.0).epsilon(0.01));
    REQUIRE(imag(output_vector[1]) == Approx(-1.0).epsilon(0.01));
    REQUIRE(real(output_vector[2]) == Approx(2.0).epsilon(0.01));
    REQUIRE(imag(output_vector[2]) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(real(output_vector[3]) == Approx(3.0).epsilon(0.01));
    REQUIRE(imag(output_vector[3]) == Approx(1.0).epsilon(0.01));
  }

  SECTION("Calculate one-dimesional FFT") {
    Eigen::VectorXd input_vector(4);
    input_vector << 3.0, 1.0, 0.0, 0.0;

    std::vector<std::complex<double>> output_vector(4);
    auto status = numeric_utils::fft(input_vector, output_vector);

    REQUIRE(status);
    REQUIRE(real(output_vector[0]) == Approx(4.0).epsilon(0.01));
    REQUIRE(imag(output_vector[0]) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(real(output_vector[1]) == Approx(3.0).epsilon(0.01));
    REQUIRE(imag(output_vector[1]) == Approx(-1.0).epsilon(0.01));
    REQUIRE(real(output_vector[2]) == Approx(2.0).epsilon(0.01));
    REQUIRE(imag(output_vector[2]) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(real(output_vector[3]) == Approx(3.0).epsilon(0.01));
    REQUIRE(imag(output_vector[3]) == Approx(1.0).epsilon(0.01));
  }

  SECTION("Calculate one-dimesional FFT") {
    Eigen::VectorXd input_vector(4);
    input_vector << 3.0, 1.0, 0.0, 0.0;

    Eigen::VectorXcd output_vector(4);
    auto status = numeric_utils::fft(input_vector, output_vector);

    REQUIRE(status);
    REQUIRE(real(output_vector(0)) == Approx(4.0).epsilon(0.01));
    REQUIRE(imag(output_vector(0)) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(real(output_vector(1)) == Approx(3.0).epsilon(0.01));
    REQUIRE(imag(output_vector(1)) == Approx(-1.0).epsilon(0.01));
    REQUIRE(real(output_vector(2)) == Approx(2.0).epsilon(0.01));
    REQUIRE(imag(output_vector(2)) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(real(output_vector(3)) == Approx(3.0).epsilon(0.01));
    REQUIRE(imag(output_vector(3)) == Approx(1.0).epsilon(0.01));
  }
}

TEST_CASE("Test polynomial curve fitting, derivatives, and evaluation",
          "[Helpers][Polynomial]") {
  SECTION("Fit polynomial with non-zero intercept--should be degree 0") {
    Eigen::VectorXd points(4);
    points << 1.0, 2.0, 3.0, 4.0;
    Eigen::VectorXd data(4);
    data << 4.0, 4.0, 4.0, 4.0;

    auto poly_coeffs = numeric_utils::polyfit_intercept(points, data, 4.0, 3);

    REQUIRE(poly_coeffs(0) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(poly_coeffs(1) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(poly_coeffs(2) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(poly_coeffs(3) == Approx(4.0).epsilon(0.01));
  }

  SECTION("Fit polynomial with non-zero intercept--should be degree 0") {
    Eigen::VectorXd points(4);
    points << 1.0, 2.0, 3.0, 4.0;
    Eigen::VectorXd data(4);
    data << 4.0, 4.0, 4.0, 4.0;

    auto poly_coeffs = numeric_utils::polyfit_intercept(points, data, 4.0, 2);

    REQUIRE(poly_coeffs(0) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(poly_coeffs(1) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(poly_coeffs(2) == Approx(4.0).epsilon(0.01));
  }

  SECTION("Fit polynomial with zero intercept--should be degree 3") {
    Eigen::VectorXd points(4);
    points << 1.0, 2.0, 3.0, 4.0;
    Eigen::VectorXd data(4);
    data << 1.0, 8.0, 27.0, 64.0;

    auto poly_coeffs = numeric_utils::polyfit_intercept(points, data, 0.0, 3);

    REQUIRE(poly_coeffs(0) == Approx(1.0).epsilon(0.01));
    REQUIRE(poly_coeffs(1) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(poly_coeffs(2) + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(poly_coeffs(3) + 1.0 == Approx(1.0).epsilon(0.01));
  }

  SECTION("Take derivative of polynomial") {
    Eigen::VectorXd coefficients(4);
    coefficients << 2.0, 2.0, 2.0, 2.0;

    auto derivs = numeric_utils::polynomial_derivative(coefficients);

    REQUIRE(coefficients.size() - 1 == derivs.size());
    REQUIRE(derivs(0) == Approx(6.0).epsilon(0.01));
    REQUIRE(derivs(1) == Approx(4.0).epsilon(0.01));
    REQUIRE(derivs(2) == Approx(2.0).epsilon(0.01));
  }

  SECTION("Evaluate polynomial") {
    Eigen::VectorXd coefficients(4);
    coefficients << 2.0, 2.0, 2.0, 2.0;

    Eigen::VectorXd points(4);
    points << 1.0, 2.0, 3.0, 4.0;

    auto evaluations = numeric_utils::evaluate_polynomial(coefficients, points);

    REQUIRE(points.size() == evaluations.size());
    REQUIRE(evaluations(0) == Approx(8.0).epsilon(0.01));
    REQUIRE(evaluations(1) == Approx(30.0).epsilon(0.01));
    REQUIRE(evaluations(2) == Approx(80.0).epsilon(0.01));
    REQUIRE(evaluations(3) == Approx(170.0).epsilon(0.01));
  }

  SECTION("Evaluate polynomial") {
    Eigen::VectorXd coefficients(4);
    coefficients << 2.0, 2.0, 2.0, 2.0;

    std::vector<double> points = {1.0, 2.0, 3.0, 4.0};

    auto evaluations = numeric_utils::evaluate_polynomial(coefficients, points);

    REQUIRE(points.size() == evaluations.size());
    REQUIRE(evaluations(0) == Approx(8.0).epsilon(0.01));
    REQUIRE(evaluations(1) == Approx(30.0).epsilon(0.01));
    REQUIRE(evaluations(2) == Approx(80.0).epsilon(0.01));
    REQUIRE(evaluations(3) == Approx(170.0).epsilon(0.01));
  }

  SECTION("Evaluate polynomial") {
    std::vector<double> coefficients = {2.0, 2.0, 2.0, 2.0};

    std::vector<double> points = {1.0, 2.0, 3.0, 4.0};

    auto evaluations = numeric_utils::evaluate_polynomial(coefficients, points);

    REQUIRE(points.size() == evaluations.size());
    REQUIRE(evaluations[0] == Approx(8.0).epsilon(0.01));
    REQUIRE(evaluations[1] == Approx(30.0).epsilon(0.01));
    REQUIRE(evaluations[2] == Approx(80.0).epsilon(0.01));
    REQUIRE(evaluations[3] == Approx(170.0).epsilon(0.01));
  }    
}
