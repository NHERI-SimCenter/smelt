#include <cmath>
#include <complex>
#include <ctime>
#include <string>
// Boost random generator
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
// Eigen dense matrices
#include <Eigen/Dense>

#include "function_dispatcher.h"
#include "json_object.h"
#include "wittig_sinha.h"

stochastic::WittigSinha::WittigSinha(const std::string& exposure_category,
                                     double gust_speed, double height,
                                     unsigned int num_floors, double total_time)
    : StochasticModel(),
      exposure_category_{exposure_category_},
      gust_speed_{gust_speed},
      bldg_height_{height},
      num_floors_{num_floors},
      seed_value_{static_cast<int>(std::time(nullptr))},
      local_x_{std::vector<double>(1, 1.0)},
      local_y_{std::vector<double>(1, 1.0)},
      freq_cutoff_{5.0},
      time_step_{1.0 / (2.0 * freq_cutoff_)} {
  model_name_ = "WittigSinha";
  num_times_ = std::ceil(total_time / time_step_) % 2 == 0
                   ? std::ceil(total_time / time_step_)
                   : std::ceil(total_time / time_step_) + 1;

  // Calculate range of frequencies based on cutoff frequency
  num_freqs_ = num_times_ / 2;
  frequencies_.resize(num_freqs_);

  for (unsigned int i = 0; i < frequencies_.size(); ++i) {
    frequencies_[i] = i * freq_cutoff_ / num_freqs_;
  }
  
  // Calculate heights of each floor
  heights_ = std::vector<double>(0.0, num_floors_);  
  heights_[0] = bldg_height_ / num_floors_;

  for (unsigned int i = 1; i < heights_.size(); ++i) {
    heights_[i] = heights_[i - 1] + bldg_height_ / num_floors_;
  }

  // Calculate velocity profile
  friction_velocity_ =
      Dispatcher<double, const std::string&, const std::vector<double>&, double,
                 double, std::vector<double>&>::instance()
          ->dispatch("ExposureCategoryVel", exposure_category, heights_, 0.4,
                     gust_speed, wind_velocities_);
}

stochastic::WittigSinha::WittigSinha(const std::string& exposure_category,
                                     double gust_speed, double height,
                                     unsigned int num_floors, double total_time,
                                     int seed_value)
    : WittigSinha(exposure_category, gust_speed, height, num_floors,
                  total_time),
      seed_value_{seed_value}
{}

stochastic::WittigSinha::WittigSinha(const std::string& exposure_category,
                                     double gust_speed,
                                     const std::vector<double>& heights,
                                     const std::vector<double>& x_locations,
                                     const std::vector<double>& y_locations,
                                     double total_time)
    : StochasticModel(),
      exposure_category_{exposure_category_},
      gust_speed_{gust_speed},
      seed_value_{static_cast<int>(std::time(nullptr))},
      heights_{heights},
      local_x_{x_locations},
      local_y_{y_locations},
      freq_cutoff_{5.0},
      time_step_{1.0 / (2.0 * freq_cutoff_)}
{
  model_name_ = "WittigSinha";
  num_times_ = std::ceil(total_time / time_step_) % 2 == 0
                   ? std::ceil(total_time / time_step_)
                   : std::ceil(total_time / time_step_) + 1;

  // Calculate range of frequencies based on cutoff frequency
  num_freqs_ = num_times_ / 2;
  frequencies_.resize(num_freqs_);

  for (unsigned int i = 0; i < frequencies_.size(); ++i) {
    frequencies_[i] = i * freq_cutoff_ / num_freqs_;
  }

  // Calculate velocity profile
  friction_velocity_ =
      Dispatcher<double, const std::string&, const std::vector<double>&, double,
                 double, std::vector<double>&>::instance()
          ->dispatch("ExposureCategoryVel", exposure_category, heights_, 0.4,
                     gust_speed, wind_velocities_);  
}

stochastic::WittigSinha::WittigSinha(const std::string& exposure_category,
                                     double gust_speed,
                                     const std::vector<double>& heights,
                                     const std::vector<double>& x_locations,
                                     const std::vector<double>& y_locations,
                                     double total_time, int seed_value)
  : WittigSinha(exposure_category, gust_speed, heights, x_locations, y_locations, total_time),
    seed_value_{seed_value}
{}

utilities::JsonObject stochastic::WittigSinha::generate(const std::string& event_name, bool units) {
  // Initialize wind velocity vectors
  std::vector<std::vector<std::vector<double>>> wind_vels(
      local_x_.size(),
      std::vector<double>(local_y_.size(),
                          std::vector<double>(heights_.size(), 0.0)));

  // Construct random number generator for standard normal distribution
  auto generator = boost::random::mt19937(static_cast<unsigned int>(seed_value_));
  boost::random::normal_distribution<double> distribution();
  boost::random::variate_generator<boost::random::mt19937&,
                                   boost::random::normal_distribution<>>
      distribution_gen(generator, distribution);

  // Generate white noise consisting of complex numbers
  Eigen::MatrixXcd white_noise(num_freqs_, heights_.size());

  for (unsigned int i = 0; i < white_noise.rows(); ++i) {
    for (unsigned int j = 0; j < white_noise.cols(); ++j) {
      white_noise(i, j) = std::complex<double>(
          distribution_gen() * std::sqrt(0.5),
          distribution_gen() * std::sqrt(std::complex<double>(-0.5)).imag());
    }
  }

  // Iterator over all frequencies and generate complex random numbers
  // for discrete time series simulation
  Eigen::MatrixXd cross_spectral_density(heights_.size(), heights_.size());
  Eigen::MatrixXd complex_random(num_freqs_, heights_.size());

  for (unsigned int i = 0; i < frequencies_.size(); ++i) {
    // Calculate cross-spectral density matrix for current frequency
    cross_spectral_density = cross_spectral_density(frequencies_[i]);

    // Find lower Cholesky factorization of cross-spectral density
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> lower_cholesky;

    try {
      auto llt = cross_spectral_density.llt();
      lower_cholesky = llt.matrixL();

      if (llt.info() == Eigen::NumericalIssue) {
        throw std::runtime_error(
            "\nERROR: In stochastic::WittigSinha::generate method: Cross-Spectral Density "
            "matrix is not positive semi-definite\n");
      }
    } catch (const std::exception& e) {
      std::cerr << "\nERROR: In time history generation: " << e.what()
                << std::endl;
    }


    // CONTINUE HERE--CHECK MATRIX MATH IN COMPARISON TO PAPER AND MATLAB CODE. IS IT REALLY
    // NECESSARY TO TRANSPOSE A BUNCH OF RANDOM NUMBER TO USE OTHER RANDOM NUMBERS?
    
    // This is Equation 5(a) from Wittig & Sinha (1975)
    complex_random.row(i) =
        (num_freqs_ * std::sqrt(2.0 * freq_cutoff_ / num_freqs_) *
         lower_cholesky * white_noise.col(i))
            .transpose();
  }
}

Eigen::MatrixXd stochastic::WittigSinha::cross_spectral_density(double frequency) const {
  // Coefficient for coherence function
  double coherence_coeff = 10.0;
  Eigen::MatrixXd cross_spectral_density(heights_.size(), heights_.size());

  for (unsigned int i = 0; i < cross_spectral_density.rows(); ++i) {
    cross_spectral_density(i, i) =
        200.0 * friction_velocity_ * friction_velocity_ * heights_[i] /
        (wind_velocities_[i] *
         std::pow(1.0 + 50.0 * frequency * heights_[i] / wind_velocities_[i],
                  5.0 / 3.0));
  }

  for (unsigned int i = 0; i < cross_spectral_density.rows(); ++i) {
    for (unsigned int j = 0; j < cross_spectral_density.cols(); ++j) {
      cross_spectral_density(i, j) =
          std::sqrt(cross_spectral_density(i, i) *
                    cross_spectral_density(j, j)) *
          std::exp(-coherence_coeff * frequency *
                   std::abs(heights_(i) - heights_(j)) /
                   (0.5 * (wind_velocities_[i] + wind_velocities_[j]))) *
          0.999;
    }
  }

  return cross_spectral_density.transpose() + cross_spectral_density -
         cross_spectral_density.diagonal().asDiagonal();
}
