#ifndef _VLACHOS_ET_AL_H_
#define _VLACHOS_ET_AL_H_

#include <string>
#include "stochastic_model.h"

namespace stochastic {
/**
 * Stochastic model for generating scenario specific ground
 * motion time histories. This is based on the paper:
 * Vlachos C., Papakonstantinou K.G., & Deodatis G. (2018).
 * Predictive model for site specific simulation of ground motions based on
 * earthquake scenarios. Earthquake Engineering & Structural Dynamics, 47(1),
 * 195-218.
 */
class VlachosEtAl : public StochasticModel {
 public:
  /**
   * @constructor Delete default constructor
   */
  VlachosEtAl() = delete;

  /**
   * @destructor Virtual destructor
   */
  virtual ~VlachosEtAl() {};

  /**
   * Get the name of the stochastic model
   * @return Model name as a string
   */
  std::string model_name() const override { return model_name_ };

  /**
   * Generate ground motion time histories based on input parameters
   */
  void generate() override;

  /**
   * Get the generated ground motion time histories in JSON wrapper
   * class format
   * @return Desires time histories in JSON wrapper class format
   */
  utilities::JsonWrapper wrapped_json_results() const override;

  /**
   * Get the generated ground motion time histories in string format
   * @return Desired time histories in string format
   */
  std::string json_results() const override;

 private:
  std::string model_name_; /**< Name of stochastic model */
  double moment_magnitude_; /**< Moment magnitude for scenario */
  double rupture_dist_; /**< Closest-to-site rupture distance in kilometers */
  double vs30_; /**< Soil shear wave velocity averaged over top 30 meters in
                   meters per second */
  unsigned int num_spectra_; /**< Number of evolutionary power spectra that
                                should be generated */
  unsigned int num_sims_; /**< Number of simulated ground motion time histories
                             that should be generated per evolutionary power
                             spectrum */
  double time_step_; /**< Temporal discretization */
  double freq_step_; /**< Frequency discretization */
  double soft_site_; /**< Factor for soft soil site */
  double med_site_; /**< Factor for medium soil site */
  double hard_site_; /**< Factor for hard soil site */
};
}  // namespace stochastic

#endif  // _VLACHOS_ET_AL_H_
