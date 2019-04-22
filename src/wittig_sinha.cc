#include <cmath>
#include <ctime>
#include <string>
#include "function_dispatcher.h"
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
