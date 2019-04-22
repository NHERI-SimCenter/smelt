#include <string>
#include "function_dispatcher.h"
#include "wittig_sinha.h"

stochastic::WittigSinha::WittigSinha(const std::string& exposure_category,
                                     double gust_speed,
                                     double height, double section_width,
                                     unsigned int num_floors, double total_time)
    : StochasticModel(),
      exposure_category_{exposure_category_},
      gust_speed_{gust_speed},
      freq_cutoff_{5.0},
      time_step_{1.0 / (2.0 * freq_cutoff_)} bldg_height_{height},
      section_width_{section_width},
      num_floors_{num_floors},
      seed_value_{std::numeric_limits<int>::infinity()} {
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
