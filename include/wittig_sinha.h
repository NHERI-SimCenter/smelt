#ifndef _STOCHASTIC_MODEL_H_
#define _STOCHASTIC_MODEL_H_

#include <string>
#include "json_object.h"

namespace stochastic {

/**
 * Stochastic model for generating wind loads using discrete frequency
 * functions with Cholesky decomposition and FFTs, as described in
 * Wittig & Sinha (1975), "Simulation of multicorrelated random
 * processes using the FFT algorithm"
 */
class WittigSinha : public StochasticModel {
 public:
  /**
   * @constructor Default constructor
   */
  WittigSinha() = default;

  /**
   * @constructor Construct wind load generator based on model input parameters
   * @param[in] exposure_category Exposure category based on ASCE-7
   * @param[in] gust_speed Gust speed of wind
   * @param[in] drag_coeff Drag coefficient for structure
   * @param[in] height Building height
   * @param[in] section_width Width of section
   * @param[in] num_floors Number of floors in building
   */
  WittigSinha(const std::string& exposure_category, double gust_speed,
              double drag_coeff, double height, double section_width,
              unsigned int num_floors);

  /**
   * @constructor Construct wind load generator based on model input parameters with
   * specified seed value
   * @param[in] exposure_category Exposure category based on ASCE-7
   * @param[in] gust_speed Gust speed of wind
   * @param[in] drag_coeff Drag coefficient for structure
   * @param[in] height Building height
   * @param[in] section_width Width of section
   * @param[in] num_floors Number of floors in building
   * @param[in] seed_value Value to seed random variables with to ensure
   *                       repeatability
   */
  WittigSinha(const std::string& exposure_category, double gust_speed,
              double drag_coeff, double height, double section_width,
              unsigned int num_floors, int seed_value);

  /**
   * @destructor Virtual destructor
   */
  virtual ~WittigSinha() {};
  
  /**
   * Delete copy constructor
   */
  WittigSinha(const WittigSinha&) = delete;

  /**
   * Delete assignment operator
   */
  WittigSinha& operator=(const WittigSinha&) = delete;

  /**
   * Generate loading based on stochastic model and store
   * outputs as JSON object
   * @param[in] event_name Name to assign to event
   * @param[in] g_units Indicates that time histories should be returned in
   *                    units of g. Defaults to false where time histories
   *                    are returned in units of m/s^2
   * @return JsonObject containing loading time histories
   */
  virtual utilities::JsonObject generate(const std::string& event_name,
                                         bool g_units = false) = 0;

  /**
   * Generate loading based on stochastic model and write
   * results to file in JSON format
   * @param[in] event_name Name to assign to event
   * @param[in, out] output_location Location to write outputs to
   * @param[in] g_units Indicates that time histories should be returned in
   *                    units of g. Defaults to false where time histories
   *                    are returned in units of m/s^2
   * @return Returns true if successful, false otherwise
   */
  virtual bool generate(const std::string& event_name,
                        const std::string& output_location,
                        bool g_units = false) = 0;

 private:
  std::string exposure_category_; /**< Exposure category for building based on ASCE-7 */
  double gust_speed_; /**< Gust speed for wind */
  double drag_coeff_; /**< Drag coefficient for building */
  double height_; /**< Height of building */
  double section_width_; /**< Width of section */
  unsigned int num_floors_; /**< Number of floors */
  int seed_value_; /**< Integer to seed random distributions with */  
};
}  // namespace stochastic

#endif  // _STOCHASTIC_MODEL_H_
