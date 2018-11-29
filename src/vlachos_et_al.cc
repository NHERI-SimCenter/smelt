#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>
#include <Eigen/Dense>
#include "factory.h"
#include "lognormal_dist.h"
#include "normal_dist.h"
#include "normal_multivar.h"
#include "numeric_utils.h"
#include "vlachos_et_al.h"

stochastic::VlachosEtAl::VlachosEtAl(double moment_magnitude,
                                     double rupture_distance, double vs30,
                                     double time_step, double freq_step,
                                     unsigned int num_spectra;
                                     unsigned int num_sims)
  : StochasticModel(),
    moment_magnitude_{moment_magnitude / 6.0},
    rupture_dist_{(rupture_distance + 5.0) / 30.0},
    vs30_{vs30 / 450.0},
    time_step_{time_step},
    freq_step_{freq_step},
    num_spectra_{num_spectra},
    num_sims_{num_sims},
    model_parameters_(18)
{
  // Factors for site condition based on Vs30
  double site_soft = 0.0, site_medium = 0.0, site_hard = 0.0; 
  if (vs30 <= 300.0) {
    site_soft = 1.0;
  } else if (vs30 <= 450.0) {
    site_medium = 1.0;
  } else {
    site_hard = 1.0;
  }
  
  // Estimated conditional mean values
  Eigen::VectorXd conditional_means(7);
  // clang-format off
  conditional_means <<
      1.0, moment_magnitude_, std::log(rupture_dist_),
      moment_magnitude_ * std::log(rupture_dist_), site_soft * std::log(vs30_),
      site_medium * std::log(vs30_), site_hard * std::log(vs30_);
  // clang-format on

  // Restricted Maximum Likelihood method regression coefficients and variance
  // components of the normal model parameters (Table 3 on page 13)
  Eigen::MatrixXd beta(18, 7);
  // clang-format off
  beta <<
    -1.1417, 1.0917, 1.9125, -0.9696, 0.0971, 0.3476, -0.6740,
    1.8052,-1.8381, -3.5874, 3.7895, 0.3236, 0.5497, 0.2876,
    1.8969,-1.8819, -2.0818, 1.9000, -0.3520, -0.6959, -0.0025,
    1.6627,-1.6922, -1.2509, 1.1880, -0.5170, -1.0157, -0.1041,
    3.8703,-3.4745, -0.0816, 0.0166, 0.4904, 0.8697, 0.3179,
    1.1043,-1.1852, -1.0068, 0.9388, -0.5603, -0.8855, -0.3174,
    1.1935,-1.2922, -0.7028, 0.6975, -0.6629, -1.1075, -0.4542,
    1.7895,-1.5014, -0.0300, -0.1306, 0.4526, 0.7132, 0.1522,
    -3.6404, 3.3189, -0.5316, 0.3874, -0.3757, -0.8334, 0.1006,
    -2.2742, 2.1454, 0.6315, -0.6620, 0.1093, -0.1028, -0.0479,
    0.6930, -0.6202, 1.8037, -1.6064, 0.0727, -0.1498, -0.0722,
    1.3003, -1.2004, -1.2210, 1.0623, -0.0252, 0.1885, 0.0069,
    0.4604, -0.4087, -0.5057, 0.4486, 0.1073, -0.0219, -0.1352,
    2.2304, -2.0398, -0.1364, 0.1910, 0.2425, 0.1801, 0.3233,
    2.3806, -2.2011, -0.3256, 0.2226, -0.0221, 0.0970, 0.0762,
    0.2057, -0.1714, 0.3385, -0.2229, 0.0802, 0.2649, 0.0396,
    -7.6011, 6.8507, -2.3609, 0.9201, -0.7508, -0.7903, -0.6204,
    -6.3472, 5.8241, 3.2994, -2.8774, -0.1411, -0.5298, -0.0203;
  // clang-format on

  // Variance of model parameters (Table 3 on page 13)
  Eigen::VectorXd variance(18);
  // clang-format off
  variance <<
      0.90, 0.80, 0.78, 0.74, 0.66, 0.73, 0.72, 0.70, 0.69,
      0.78, 0.90, 0.90, 0.90, 0.90, 0.80, 0.90, 0.35, 0.80;
  // clang-format on
  
  // Estimated correlation matrix (Table A1 on page 24)
  Eigen::MatrixXd correlation_matrix(18, 18);
  // clang-format off
  correlation_matrix <<
    1.0000, 0.0382, -0.0912, -0.0701, -0.0214, -0.0849, -0.0545, -0.0185, 0.0270, -0.0122, 0.0059, -0.0344, -0.0342, 0.0409, -0.0137, -0.0168, -0.0990, -0.6701,
    0.0382, 1.0000, -0.1159, -0.1856, 0.0681, -0.2018, -0.2765, -0.0304, -0.1719, -0.1157, -0.0347, -0.0277, -0.0189, 0.0357, 0.0657, -0.0070, 0.3690, -0.0510,
    -0.0912, -0.1159, 1.0000, 0.9467, 0.4123, 0.4815, 0.4240, 0.2120, 0.1070, -0.1898, 0.0506, -0.0661, -0.0380, 0.0260, 0.0506, -0.0317, -0.0278, 0.0245,
    -0.0701, -0.1856, 0.9467, 1.0000, 0.4075, 0.4891, 0.4940, 0.2285, 0.2009, -0.1709, 0.0365, -0.0579, -0.0999, 0.0467, 0.0410, 0.0027, -0.0966, 0.0631,
    -0.0214, 0.0681, 0.4123, 0.4075, 1.0000, 0.1772, 0.1337, 0.7315, -0.0066, -0.2787, 0.0703, -0.0541, -0.0453, 0.1597, 0.0792, 0.0220, 0.0606, -0.0844,
    -0.0849, -0.2018, 0.4815, 0.4891, 0.1772, 1.0000, 0.9448, 0.3749, 0.1682, -0.0831, 0.0124, -0.1236, -0.0346, -0.0054, 0.0877, -0.0197, -0.0867, 0.0281,
    -0.0545, -0.2765, 0.4240, 0.4940, 0.1337, 0.9448, 1.0000, 0.3530, 0.2305, -0.0546, -0.0223, -0.0782, -0.0872, 0.0074, 0.0999, 0.0066, -0.1358, 0.0626,
    -0.0185, -0.0304, 0.2120, 0.2285, 0.7315, 0.3749, 0.3530, 1.0000, 0.1939, -0.0617, -0.0017, -0.0942, -0.0332, 0.0813, 0.0810, -0.0032, -0.0870, -0.0599,
    0.0270, -0.1719, 0.1070, 0.2009, -0.0066, 0.1682, 0.2305, 0.1939, 1.0000, -0.1851, -0.2073, -0.0756, -0.1637, -0.0865, 0.0699, -0.0485, -0.2153, 0.0320,
    -0.0122, -0.1157, -0.1898, -0.1709, -0.2787, -0.0831, -0.0546, -0.0617, -0.1851, 1.0000, 0.2139, 0.0769, 0.1391, 0.0769, -0.1838, 0.0377, -0.1615, 0.1000,
    0.0059, -0.0347, 0.0506, 0.0365, 0.0703, 0.0124, -0.0223, -0.0017, -0.2073, 0.2139, 1.0000, -0.1102, -0.0530, 0.0791, 0.0012, 0.0090, -0.0236, 0.0037,
    -0.0344, -0.0277, -0.0661, -0.0579, -0.0541, -0.1236, -0.0782, -0.0942, -0.0756, 0.0769, -0.1102, 1.0000, -0.2562, -0.0406, 0.3154, 0.0065, -0.0093, -0.0354,
    -0.0342, -0.0189, -0.0380, -0.0999, -0.0453, -0.0346, -0.0872, -0.0332, -0.1637, 0.1391, -0.0530, -0.2562, 1.0000, -0.1836, -0.1624, -0.5646, 0.0216, 0.0243,
    0.0409, 0.0357, 0.0260, 0.0467, 0.1597, -0.0054, 0.0074, 0.0813, -0.0865, 0.0769, 0.0791, -0.0406, -0.1836, 1.0000, 0.1624, 0.1989, 0.0549, -0.0411,
    -0.0137, 0.0657, 0.0506, 0.0410, 0.0792, 0.0877, 0.0999, 0.0810, 0.0699, -0.1838, 0.0012, 0.3154, -0.1624, 0.1624, 1.0000, 0.1552, 0.0844, -0.0637,
    -0.0168, -0.0070, -0.0317, 0.0027, 0.0220, -0.0197, 0.0066, -0.0032, -0.0485, 0.0377, 0.0090, 0.0065, -0.5646, 0.1989, 0.1552, 1.0000, 0.0058, 0.0503,
    -0.0990, 0.3690, -0.0278, -0.0966, 0.0606, -0.0867, -0.1358, -0.0870, -0.2153, -0.1615, -0.0236, -0.0093, 0.0216, 0.0549, 0.0844, 0.0058, 1.0000, -0.0930,
    -0.6701, -0.0510, 0.0245, 0.0631, -0.0844, 0.0281, 0.0626, -0.0599, 0.0320, 0.1000, 0.0037, -0.0354, 0.0243, -0.0411, -0.0637, 0.0503, -0.0930, 1.0000;
  // clang-format on

  // Mean of transformed normal model parameters (described by Eq. 25 on page 12)
  means_ = conditional_means * beta.transpose();

  // Convert the standard deviation and correlation to covariance
  covariance_ = numeric_utils::corr_to_cov(correlation_matrix, variance.sqrt());

  // Generate realizations of model parameters
  int seed = 100;
  sample_generator_ =
      Factory<numeric_utils::RandomGenerator, int>::instance()->create(
          "MultivariateNormal", std::move(seed));
  sample_generator_->generate(parameter_realizations_, means_, covariance_,
                              num_spectra_);

  // Create distributions for model parameters
  model_parameters_[0] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "LognormalDist", std::move(-1.735), std::move(0.523));
  model_parameters_[1] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "LognormalDist", std::move(1.009), std::move(0.422));
  model_parameters_[2] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "NormalDist", std::move(0.249), std::move(1.759));
  model_parameters_[3] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "NormalDist", std::move(0.768), std::move(1.958));  
  model_parameters_[4] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "LognormalDist", std::move(2.568), std::move(0.557));
  model_parameters_[5] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "NormalDist", std::move(0.034), std::move(1.471));
  model_parameters_[6] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "NormalDist", std::move(0.441), std::move(1.733));
  model_parameters_[7] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "LognormalDist", std::move(3.356), std::move(0.473));    
  model_parameters_[8] =
    Factory<stochastic::Distribution, double, double>::instance()->create(
          "BetaDist", std::move(2.516), std::move(9.714));
  model_parameters_[9] =
    Factory<stochastic::Distribution, double, double>::instance()->create(
          "BetaDist", std::move(3.582), std::move(15.209));
  model_parameters_[10] =
    Factory<stochastic::Distribution, double, double>::instance()->create(
          "LognormalDist", std::move(0.746), std::move(0.404));
  model_parameters_[11] =
      Factory<stochastic::Distribution, double, double, double>::instance()
          ->create("StudentstDist", std::move(0.205), std::move(0.232),
                   std::move(7.250));
  model_parameters_[12] =
        Factory<stochastic::Distribution, double, double>::instance()->create(
            "InverseGaussianDist", std::move(0.499), std::move(0.213));
  model_parameters_[13] =
        Factory<stochastic::Distribution, double, double>::instance()->create(
            "LognormalDist", std::move(0.702), std::move(0.435));
  model_parameters_[14] =
      Factory<stochastic::Distribution, double, double, double>::instance()
          ->create("StudentstDist", std::move(0.792), std::move(0.157),
                   std::move(4.223));
  model_parameters_[15] =
        Factory<stochastic::Distribution, double, double>::instance()->create(
            "InverseGaussianDist", std::move(0.350), std::move(0.170));
  model_parameters_[16] =
    Factory<stochastic::Distribution, double, double>::instance()->create(
          "LognormalDist", std::move(9.470), std::move(1.317));
  model_parameters_[17] =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "LognormalDist", std::move(3.658), std::move(0.375));

  // Standard normal distribution with mean at 0.0 and standard deviation of 1.0
  auto std_normal_dist =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "NormalDist", std::move(0.0), std::move(1.0));

  physical_parameters_.resize(parameter_realizations_.rows(),
                              parameter_realizations_.cols());

  // Transform sample normal model parameters to physical space
  for (unsigned int i = 0; i < model_parameters_.size(); ++i) {
    for (unsigned int j = 0; j < parameter_realizations_.rows(); ++j) {
      physical_parameters(j, i) =
          model_parameters_[i]->inv_cumulative_dist_func(
              std_normal_dist->cumulative_dist_func(
                  std::vector<double>{parameter_realizations_(j, i)}));
    }
  }
}

utilities::JsonWrapper stochastic::VlachosEtAl::generate() {
  // Calculate sample non-stationary power spectrum following Eq-18 on page 8 as
  // described in Section 5.
  // UPDATE DATA TYPE HERE AFTER ADDING POWER_SPECTRUM FUNCTION
  std::vector<std::vector<double>> accelerations(num_spectra_,
                                                 std::vector<double>());

  for (unsigned int i = 0; i < num_spectra_; ++i) {
    accelerations[i] =
        power_spectrum(physical_parameters.row(i));
  }

  // CONTINUE HERE AFTER ADDING POWER_SPECTRUM FUNCS
}

std::vector<std::vector<double>> stochastic::VlachosEtAl::simulate_time_histories() const {
  // STILL NEEDS TO BE IMPLEMENTED
}

Eigen::VectorXd stochastic::VlachosEtAl::identify_parameters(
    const Eigen::VectorXd& initial_params) const {
  // Initialize non-dimensional cumulative energy
  std::vector<double> energy(static_cast<unsigned int>(1.0 / 0.05) + 1, 0.0);

  for (unsigned int i = 1; i < energy.size(); ++i) {
    energy[i] = energy[i - 1] + 0.05;
  }

  // Initialze mode 1 parameters and frequencies
  std::vector<double> mode_1_params = {initial_params(2),
				       initial_params(3),
				       initial_params(4)};
  auto mode_1_freqs = modal_frequencies(mode_1_params, energy);

  // Initialize mode 2 parameters and frequencies
  std::vector<double> mode_2_params = {initial_params(5),
				       initial_params(6),
				       initial_params(7)};
  auto mode_2_freqs = modal_frequencies(mode_2_params, energy);

  double mode_1_mean = initial_params(11), mode_2_mean = initial_params(14);

  // Standard normal distribution with mean at 0.0 and standard deviation of 1.0
  auto std_normal_dist =
      Factory<stochastic::Distribution, double, double>::instance()->create(
          "NormalDist", std::move(0.0), std::move(1.0));  
  bool freq_comparison = true;
  Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> realizations(
      initial_params.size(), 1);
  Eigen::VectorXd transformed_realizations(initial_params.size());

  // Iterate until suitable parameter values have been identified
  while (freq_comparison || (mode_1_mean > mode_2_mean)) {
    
    // Generate realizations of parameters
    sample_generator_->generate(realizations, means_, covariance_, 1);
    
    // Transform parameter realizations to physical space
    for (unsigned int i = 0; i < initial_params.size(); ++i) {
      transformed_realizations(i) =
          model_parameters_[i]->inv_cumulative_dist_func(
              std_normal_dist->cumulative_dist_func(
                  std::vector<double>{realizations(i, 0)}));
    }

    // Calculate dominant modal frequencies
    mode_1_params = {transformed_realizations(2), transformed_realizations(3),
                     transformed_realizations(4)};
    mode_1_freqs = modal_frequencies(mode_1_params, energy);

    mode_2_params = {transformed_realizations(5), transformed_realizations(6),
                     transformed_realizations(7)};
    mode_2_freqs = modal_frequencies(mode_2_params, energy);

    mode_1_mean = transformed_realizations(11);
    mode_2_mean = transformed_realizations(14);

    // Check if any mode 1 dominant frequencies across all non-dimensional energy values
    // are greater than the corresponding values for mode 2
    freq_comparison = false;
    for (unsigned int i = 0; i < mode_1_freqs.size(); ++i) {
      if (mode_1_freqs[i] > mode_2_freqs[i]) {
	freq_comparison = true;
	break;
      }
    }
  }

  return transformed_realizations;
}

std::vector<double> stochastic::VlachosEtAl::modal_frequencies(
    const std::vector<double>& parameters,
    const std::vector<double>& energy) const {
  std::vector<double> frequencies(energy.size());

  for (unsigned int i = 0; i < energy.size(); ++i) {
    frequencies[i] = parameters[2] * std::pow(0.5 + energy[i], parameters[0]) *
                     std::pow(1.5 - energy[i], parameters[1]);
  }

  return frequencies;
}

std::vector<double> stochastic::VlachosEtAl::energy_accumulation(
    const std::vector<double>& parameters,
    const std::vector<double>& times) const {
  std::vector<double> accumulated_energy(times.size());

  for (unsigned int i = 0; i < times.size(); ++i) {
    accumulated_energy[i] =
        std::exp(-std::pow(times[i] / parameters[0]), -parameters[1]) /
        std::exp(-std::pow(1.0 / parameters[0], -parameters[1]));
  }

  return accumulated_energy;
}

std::vector<double> stochastic::VlachosEtAl::modal_participation_factor(
    const std::vector<double>& parameters,
    const std::vector<double>& energy) const {
  std::vector<double> participation_factor(energy.size());

  for (unsigned int i = 0; i < energy.size(); ++i) {
    participation_factor[i] =
        parameters[0] * std::exp(-std::pow(
                            (energy[i] - parameters[1]) / parameters[2], 2)) +
        parameters[3] * std::exp(-std::pow(
                            (energy[i] - parameters[4]) / parameters[5], 2)) -
        2.0;
  }
  
  return participation_factor;
}

std::vector<double> stochastic::VlachosEtAl::amplitude_modulating_function(
    double duration, double total_energy, const std::vector<double>& parameters,
    const std::vector<double>& times) const {
  std::vector<double> func_vals(times.size());

  double term1 = total_energy * parameters[1] / (parameters[0] * duration);
  double term2 = std::pow(1.0 / parameters[0], -parameters[1]);

  for (unsigned int i = 0; i < times.size(); ++i) {
    func_vals[i] =
        term1 *
        std::exp(term2 - std::pow(times[i] / parameters[0], parameters[1])) *
        std::pow(times[i] / parameters[0], -1.0 - parameters[1]);
  }

  return func_vals;
}

Eigen::VectorXd stochastic::VlachosEtAl::kt_2(
    const std::vector<double>& parameters,
    const std::vector<double>& frequencies,
    const std::vector<double>& highpass_butter) const {
  Eigen::VectorXd power_spectrum(frequencies.size());
  double mode1 = 0, mode2 = 0;
  
  for (unsigned int i = 0; i < frequencies.size(); ++i) {
    mode1 = parameters[2] *
            (1.0 + 4.0 * parameters[1] * parameters[1] *
                       std::pow(frequencies[i] / parameters[0], 2)) /
            (std::pow(1 - std::pow(frequencies[i] / parameters[0], 2), 2) +
             4.0 * parameters[1] * parameters[1] *
                 std::pow(frequencies[i] / parameters[0], 2));
    mode2 = parameters[5] *
            (1.0 + 4.0 * parameters[4] * parameters[4] *
                       std::pow(frequencies[i] / parameters[3], 2)) /
            (std::pow(1 - std::pow(frequencies[i] / parameters[3], 2), 2) +
             4.0 * parameters[4] * parameters[4] *
                 std::pow(frequencies[i] / parameters[3], 2));

    power_spectrum[i] = highpass_butter[i] * (mode1 + mode2)
  }

  return power_spectrum;
}
