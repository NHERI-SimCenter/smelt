#include <string>
#include "wittig_sinha.h"

stochastic::WittigSinha::WittigSinha(const std::string& exposure_category,
                                     double gust_speed, double drag_coeff,
                                     double height, double section_width,
                                     unsigned int num_floors, double total_time)
    : StochasticModel(),
      exposure_category_{exposure_category_},
      gust_speed_{gust_speed},
      drag_coeff_{drag_coeff},
      freq_cutoff_{5.0},
      time_step_{1.0 / (2.0 * freq_cutoff_)}
      bldg_height_{height},
      section_width_{section_width},
      num_floors_{num_floors},
      seed_value_{std::numeric_limits<int>::infinity()}
{
  model_name_ = "WittigSinha";
  num_times_ = std::ceil(total_time / time_step_) % 2 == 0
                   ? std::ceil(total_time / time_step_)
                   : std::ceil(total_time / time_step_) + 1;

  // CONTINUE HERE
  
  heights_ = std::vector<double>(0.0, num_floors_);

  // Calculate heights of each floor
  heights_[0] = bldg_height_ / num_floors_;
  for (unsigned int i = 1; i < heights_.size(); ++i) {
    heights_[i] = heights_[i - 1] + bldg_height_ / num_floors_;
  }

  
}
