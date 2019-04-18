#include <string>
#include "wittig_sinha.h"

stochastic::WittigSinha::WittigSinha(const std::string& exposure_category,
                                     double gust_speed, double drag_coeff,
                                     double height, double section_width,
                                     unsigned int num_floors)
    : StochasticModel(),
      exposure_category_{exposure_category_},
      gust_speed_{gust_speed},
      drag_coeff_{drag_coeff},
      height_{height},
      section_width_{section_width},
      num_floors_{num_floors},
      seed_value_{std::numeric_limits<int>::infinity()}      
{
  model_name_ = "WittigSinha";
  
}
