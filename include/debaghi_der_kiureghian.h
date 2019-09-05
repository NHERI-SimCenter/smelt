#ifndef _DABAGHI_DER_KIUREGHIAN_H_
#define _DABAGHI_DER_KIUREGHIAN_H_

#include <memory>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "distribution.h"
#include "json_object.h"
#include "numeric_utils.h"
#include "stochastic_model.h"

namespace stochastic {
/** @enum stochastic::FaultType
 *  @brief is a strongly typed enum class representing the type of faulting
 */  
enum class FaultType {
  StrikeSlip, /**< Strike-slip fault */
  ReverseAndRevObliq /**< Reverse or reverse-oblique fault */
};

/** @enum stochastic::SimulationType
 *  @brief is a strongly typed enum class representing pulse-like proportion
 *  of ground motion
 */  
enum class SimulationType {
  PulseAndNoPulse, /**< pulse-like and non-pulse-like motions in proportion predicted by Shahi and Baker(2014) */
  Pulse, /**< only pulse-like */
  NoPulse /**< only non-pulse-like */
};

/**
 * Stochastic model for simulating near-fault ground motions. Based on the following
 * references:
 *   1. Dabaghi and Der Kiureghian (2014 PEER report) "Stochastic Modeling and Simulation of Near-Fault Ground Motions for Performance-Based Earthquake Engineering"
 *   2. Dabaghi and Der Kiureghian (2017 EESD) "Stochastic model for simulation of NF GMs"
 *   3. Dabaghi and Der Kiureghian (2018 EESD) "Simulation of orthogonal horizontal components of near-fault ground motion for specified EQ source and site characteristics"
 */
class DabaghiDerKiureghian : public StochasticModel {
 public:
  /**
   * @constructor Default constructor
   */
  DabaghiDerKiureghian() = default;

  /**
   * @constructor Construct near-fault ground motion model based on input
   * parameters
   * @param[in] faulting Type of faulting
   * @param[in] simulation_type Pulse-type of simulation
   * @param[in] moment_magnitude Moment magnitude of earthquake
   * @param[in] depth_to_rupt Depth to the top of the rupture plane in
   *               kilometers
   * @param[in] rupture_distance Closest distance from the site to the fault
   *               rupture in kilometers
   * @param[in] vs30 Soil shear wave velocity averaged over top 30 meters in
   *               meters per second
   * @param[in] s_or_d Directivity parameter s or d (km)--input the larger of
   *               the two
   * @param[in] theta_or_phi Directivity ange parameter theta or phi
   *               (degrees)--input corresponding value to s or d
   * @param[in] num_sims Number of simulated ground motion time histories that
   *               should be generated (number of different model parameter realizations)
   * @param[in] num_params Number of realizations of non-stationary, modulated, filtered
   *               white noise per set of model parameters
   * @param[in] truncate Boolean indicating whether to truncate and baseline correct
   *               synthetic motion
   */
  DabaghiDerKiureghian(FaultType faulting, SimulationType simulation_type,
                       double moment_magnitude, double depth_to_rupt,
                       double rupture_distance, double vs30, double s_or_d,
                       double theta_or_phi, unsigned int num_sims,
                       unsigned int num_params, bool truncate);

  /**
   * @constructor Construct near-fault ground motion model based on input
   * parameters
   * @param[in] faulting Type of faulting
   * @param[in] simulation_type Pulse-type of simulation
   * @param[in] moment_magnitude Moment magnitude of earthquake
   * @param[in] depth_to_rupt Depth to the top of the rupture plane in
   *               kilometers
   * @param[in] rupture_distance Closest distance from the site to the fault
   *               rupture in kilometers
   * @param[in] vs30 Soil shear wave velocity averaged over top 30 meters in
   *               meters per second
   * @param[in] s_or_d Directivity parameter s or d (km)--input the larger of
   *               the two
   * @param[in] theta_or_phi Directivity ange parameter theta or phi
   *               (degrees)--input corresponding value to s or d
   * @param[in] num_sims Number of simulated ground motion time histories that
   *               should be generated (number of different model parameter realizations)
   * @param[in] num_params Number of realizations of non-stationary, modulated, filtered
   *               white noise per set of model parameters
   * @param[in] truncate Boolean indicating whether to truncate and baseline correct
   *               synthetic motion
   * @param[in] seed_value Value to seed random variables with to ensure
   *               repeatability
   */
  DabaghiDerKiureghian(FaultType faulting, SimulationType simulation_type,
                       double moment_magnitude, double depth_to_rupt,
                       double rupture_distance, double vs30, double s_or_d,
                       double theta_or_phi, unsigned int num_sims,
                       unsigned int num_params, bool truncate, int seed_value);

  /**
   * @destructor Virtual destructor
   */
  virtual ~DabaghiDerKiureghian() {};

  /**
   * Delete copy constructor
   */
  DabaghiDerKiureghian(const DabaghiDerKiureghian&) = delete;

  /**
   * Delete assignment operator
   */
  DabaghiDerKiureghian& operator=(const DabaghiDerKiureghian&) = delete;

  /**
   * Generate ground motion time histories based on input parameters
   * and store outputs as JSON object. Throws exception if errors
   * are encountered during time history generation.
   * @param[in] event_name Name to assign to event
   * @param[in] units Indicates that time histories should be returned in
   *                  units of g. Defaults to false where time histories
   *                  are returned in units of m/s^2
   * @return JsonObject containing time histories
   */
  utilities::JsonObject generate(const std::string& event_name,
                                 bool units = false) override;

  /**
   * Generate ground motion time histories based on input parameters
   * and write results to file in JSON format. Throws exception if
   * errors are encountered during time history generation.
   * @param[in] event_name Name to assign to event
   * @param[in, out] output_location Location to write outputs to
   * @param[in] units Indicates that time histories should be returned in
   *                  units of g. Defaults to false where time histories
   *                  are returned in units of m/s^2
   * @return Returns true if successful, false otherwise
   */
  bool generate(const std::string& event_name,
                const std::string& output_location,
                bool units = false) override;

  /**
   * Generates proportion of motions that should be pulse-like based on total
   * number of simulations and probability of those motions containing a pulse
   * following pulse probability model developed by Shahi & Baker (2014)
   * @param[in] num_sims Total number of simulations that should be generated
   */
  unsigned int simulate_pulse_type(unsigned num_sims) const;

 private:
  DabaghiDerKiureghian(FaultType faulting, SimulationType simulation_type,
                       double moment_magnitude, double depth_to_rupt,
                       double rupture_distance, double vs30, double s_or_d,
                       double theta_or_phi, unsigned int num_sims,
                       unsigned int num_params, bool truncate, int seed_value);

  FaultType faulting_; /**< Enum for type of faulting for scenario */
  SimulationType sim_type_; /**< Enum for pulse-like nature of ground motion */
  double moment_magnitude_; /**< Moment magnitude for scenario */
  double depth_to_rupt_; /**< Depth to the top of the rupture plane (km) */
  double rupture_dist_; /**< Closest-to-site rupture distance in kilometers */
  double vs30_; /**< Soil shear wave velocity averaged over top 30 meters in
                   meters per second */
  double s_or_d_; /**< Directivity parameter s or d (km) */
  double theta_or_phi_; /**< Directivity angle parameter theta or phi */
  bool truncate_; /**< Indicates whether to truncate and baseline correct motion */
  unsigned int num_sims_pulse_; /**< Number of pulse-like simulated ground
                             motion time histories that should be generated */
  unsigned int num_sims_no_pulse_; /**< Number of no-pulse-like simulated ground
                             motion time histories that should be generated */
  unsigned int num_params_; /**< Number of realizations of model parameters */
  int seed_value_; /**< Integer to seed random distributions with */
  double time_step_; /**< Temporal discretization. Set to 0.005 seconds */
  
  
  /* double orientation_; /\**< Counter-clockwise angle away from global x-axis *\/ */
  /* double time_step_; /\**< Temporal discretization. Set to 0.01 seconds *\/ */
  /* double freq_step_; /\**< Frequency discretization. Set to 0.2 Hz *\/ */
  /* double cutoff_freq_; /\**< Cutoff frequency *\/ */
  /* unsigned int num_spectra_; /\**< Number of evolutionary power spectra that */
  /*                               should be generated *\/ */
  /* unsigned int num_sims_; /\**< Number of simulated ground motion time histories */
  /*                            that should be generated per evolutionary power */
  /*                            spectrum *\/ */
  /* int seed_value_; /\**< Integer to seed random distributions with *\/ */
  /* Eigen::VectorXd means_; /\**< Mean values of model parameters *\/ */
  /* Eigen::MatrixXd covariance_; /\**< Covariance matrix for model parameters *\/ */
  /* std::vector<std::shared_ptr<stochastic::Distribution>> */
  /*     model_parameters_; /\**< Distrubutions for 18-parameter model *\/ */
  /* Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> */
  /*     parameter_realizations_; /\**< Random realizations of normal model parameters *\/ */
  /* Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> */
  /*     physical_parameters_; /\**< Normal parameters transformed to */
  /*                              physical space *\/ */
  /* std::shared_ptr<numeric_utils::RandomGenerator> */
  /*     sample_generator_; /\**< Multivariate normal random number generator *\/ */
};
}  // namespace stochastic

#endif  // _DABAGHI_DER_KIUREGHIAN_H_
