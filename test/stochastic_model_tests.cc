#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <catch/catch.hpp>
#include <Eigen/Dense>
#include <nlohmann_json/json.hpp>
#include "factory.h"
#include "vlachos_et_al.h"
#include "wittig_sinha.h"

TEST_CASE("Test Vlachos et al. (2018) implementation", "[Stochastic][Seismic]") {

  double moment_magnitude = 6.5, rupture_dist = 30.0, vs30 = 500.0,
         orientation = 30.0;
  unsigned int num_spectra = 2, num_sims = 2;
 
  stochastic::VlachosEtAl test_model(moment_magnitude, rupture_dist, vs30, orientation, num_spectra, num_sims);

  SECTION("Test modal frequency identification") {
    std::vector<double> params = {2.0, 3.0, 4.0};
    std::vector<double> energy = {0.5, 2.5};
    auto modal_frequencies = test_model.modal_frequencies(params, energy);

    REQUIRE(modal_frequencies[0] == 4.0);
    REQUIRE(modal_frequencies[1] == -36.0);
  }

  SECTION("Test energy accumulation computation") {
    std::vector<double> params = {2.0, 0.5};
    std::vector<double> times = {0.0, 0.5, 1.0};

    auto accumulated_energy = test_model.energy_accumulation(params, times);

    std::vector<double> expected_value = {0.0, 0.556667904, 1.0};
    REQUIRE(accumulated_energy[0] - expected_value[0] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(accumulated_energy[1] - expected_value[1] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(accumulated_energy[2] - expected_value[2] + 1.0 ==
            Approx(1.0).epsilon(0.01));
  }

  SECTION("Test modal participation factor") {
    std::vector<double> params = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    std::vector<double> energy = {0.5, 1.5};

    auto participation_factors = test_model.modal_participation_factor(params, energy);
    
    std::vector<double> expected_value = {112.236819763036, 1109.33809695776};
    REQUIRE(participation_factors[0] - expected_value[0] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(participation_factors[1] - expected_value[1] + 1.0 ==
            Approx(1.0).epsilon(0.01));    
  }

  SECTION("Test amplitude modulating factor") {
    std::vector<double> params = {2.0, 0.5};
    std::vector<double> times = {0.0, 1.0, 2.0};
    double duration = 2.0, total_energy = 4.0;

    auto modulating_factors = test_model.amplitude_modulating_function(
        duration, total_energy, params, times);
    std::vector<double> expected_value = {1.414213562373095, 0.756590125372443};

    REQUIRE(std::isnan(modulating_factors[0]));
    REQUIRE(modulating_factors[1] - expected_value[0] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(modulating_factors[2] - expected_value[1] + 1.0 ==
            Approx(1.0).epsilon(0.01));
  }

  SECTION("Test acceleration rotation") {
    std::vector<double> acceleration = {1.0};
    std::vector<double> x_accels(acceleration.size());
    std::vector<double> y_accels(acceleration.size());

    test_model.rotate_acceleration(acceleration, x_accels, y_accels, false);

    REQUIRE(x_accels[0] == Approx(std::sqrt(3.0) / (100.0 *2.0)).epsilon(0.01));
    REQUIRE(y_accels[0] == Approx(1.0/(100.0 * 2.0)).epsilon(0.01));

    stochastic::VlachosEtAl test_model_2(6.5, 30.0, 500.0, 315.0, 1, 1);
    test_model_2.rotate_acceleration(acceleration, x_accels, y_accels, false);

    REQUIRE(x_accels[0] == Approx(1.0/(100.0 * std::sqrt(2.0))).epsilon(0.01));
    REQUIRE(y_accels[0] == Approx(-1.0/(100.0 * std::sqrt(2.0))).epsilon(0.01));

    test_model_2.rotate_acceleration(acceleration, x_accels, y_accels, true);

    REQUIRE(x_accels[0] ==
            Approx(1.0 / (100.0 * 9.81 * std::sqrt(2.0))).epsilon(0.01));
    REQUIRE(y_accels[0] ==
            Approx(-1.0 / (100.0 * 9.81 * std::sqrt(2.0))).epsilon(0.01));
  }

  SECTION("Test K-T model") {
    std::vector<double> params = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    std::vector<double> frequencies = {1.0, 2.0};
    std::vector<double> hp_butter_energy = {0.5, 0.75};

    auto power_spectrum = test_model.kt_2(params, frequencies, hp_butter_energy);
    std::vector<double> expected_value = {5.63257127563669, 8.39842328684051};

    for (unsigned int i = 0; i < power_spectrum.size(); ++i) {
      REQUIRE(power_spectrum[i] == Approx(expected_value[i]).epsilon(0.01));
    }
  }

  SECTION("Test time history generation") {  
    auto test_model_factory =
        Factory<stochastic::StochasticModel, double, double, double, double,
                unsigned int, unsigned int>::instance()
            ->create("VlachosSiteSpecificEQ", std::move(moment_magnitude),
                     std::move(rupture_dist), std::move(vs30),
                     std::move(orientation), std::move(num_spectra),
                     std::move(num_sims));

    auto success = test_model_factory->generate("TestHistory", "./test.json", false);
    REQUIRE(success);

    // stochastic::VlachosEtAl coalinga_model(6.36, 43.6, 441.4, 0.0, 0.01, 0.2, 1, 1);
    // auto results = coalinga_model.generate("TestHistory", "./coalinga.json");
  }
  
  SECTION("Test time history generation") {
    int seed = 10;    
    auto test_model_factory1 =
        Factory<stochastic::StochasticModel, double, double, double, double,
                unsigned int, unsigned int, int>::instance()
            ->create("VlachosSiteSpecificEQ", std::move(moment_magnitude),
                     std::move(rupture_dist), std::move(vs30),
                     std::move(orientation), std::move(num_spectra),
                     std::move(num_sims), std::move(seed));
      
    auto test_model_factory2 =
        Factory<stochastic::StochasticModel, double, double, double, double,
                unsigned int, unsigned int, int>::instance()
            ->create("VlachosSiteSpecificEQ", std::move(moment_magnitude),
                     std::move(rupture_dist), std::move(vs30),
                     std::move(orientation), std::move(num_spectra),
                     std::move(num_sims), std::move(seed));

    auto result1 = test_model_factory1->generate("TestHistory");
    auto result2 = test_model_factory2->generate("TestHistory");
    auto json1 = result1.get_library_json();
    auto json2 = result2.get_library_json();

    REQUIRE(json1["Events"][0]["timeSeries"][0]["data"] ==
            json2["Events"][0]["timeSeries"][0]["data"]);
  }  
}

TEST_CASE("Test Wittig & Sinha (1975) implementation", "[Stochastic][Wind]") {
  std::string exposure_category = "A";
  double gust_speed = 30.0;
  double height = 200.0;
  unsigned int num_floors = 20;
  double total_time = 600;

  stochastic::WittigSinha test_wittig_sinha(exposure_category, gust_speed,
                                            height, num_floors, total_time);

  auto test_model_factory1 =
      Factory<stochastic::StochasticModel, std::string, double, double,
              unsigned int, double>::instance()
          ->create("WittigSinhaDiscreteFreqWind", std::move(exposure_category),
                   std::move(gust_speed), std::move(height),
                   std::move(num_floors), std::move(total_time));

  SECTION("Test cross spectral density matrix computation") {

    Eigen::MatrixXd expected_density_matrix(num_floors, num_floors);
    expected_density_matrix <<
      0.959283437907896, 1.140660608173711, 1.246474365873345, 1.317351209251509, 1.368026478757019, 1.405563636959474, 1.433922626982650, 1.455551976511244, 1.472071539554080, 1.484607470874768, 1.493973058020677, 1.500773605712803, 1.505470777477411,  1.508423886207301, 1.509917392972469, 1.510179798584229, 1.509396969414855, 1.507721753485774, 1.505281058306567, 1.502181151843766,
      1.140660608173711, 1.404332537453387, 1.533431953981596, 1.621118361356531, 1.684030179854216, 1.730816271052571, 1.766326296809765, 1.793559836103599, 1.814502068942717, 1.830533754148074, 1.842652548192438, 1.851601377680545, 1.857947196481228,  1.862131533333016, 1.864504160703294, 1.865346229170771, 1.864886589253295, 1.863313571980112, 1.860783661885641, 1.857427994274683,
      1.246474365873345, 1.533431953981596, 1.724381332617859, 1.821316982869174, 1.892230726933503, 1.945079362122620, 1.985290872899425, 2.016222558864558, 2.040096521895055, 2.058458514685225, 2.072425606729346, 2.082829825345476, 2.090306274903157,  2.095349691488831, 2.098352112771816, 2.099628761957606, 2.099436310130595, 2.097986058095904, 2.095453641635196, 2.091986302644697,
      1.317351209251509, 1.621118361356531, 1.821316982869174, 1.975264045493186, 2.050205410913884, 2.107604182567155, 2.151347381876264, 2.185060560639033, 2.211143278381126, 2.231264749927427, 2.246631326574126, 2.258141608078218, 2.266481588761922,  2.272185717018468, 2.275677563306525, 2.277297763026081, 2.277323731016889, 2.275983891404253, 2.273468154464480, 2.269935765942838,
      1.368026478757019, 1.684030179854216, 1.892230726933503, 2.050205410913884, 2.180694577343107, 2.239559878229213, 2.286133585538716, 2.322076276312510, 2.349930042620272, 2.371463159423016, 2.387953548339291, 2.400353132569224, 2.409388639833794,  2.415626285360819, 2.419514849570127, 2.421415274590276, 2.421621544296497, 2.420375754795262, 2.417879209868303, 2.414300733553258,
      1.405563636959474, 1.730816271052571, 1.945079362122620, 2.107604182567155, 2.239559878229213, 2.353533272604917, 2.400112974379595, 2.437913148554039, 2.467241778796656, 2.489950052166998, 2.507375737197111, 2.520515308302347, 2.530129499616276,  2.536811028695948, 2.541029694829150, 2.543163361222094, 2.543519811061008, 2.542352521513453, 2.539872276549266, 2.536255866779533,
      1.433922626982650, 1.766326296809765, 1.985290872899425, 2.151347381876264, 2.286133585538716, 2.400112974379595, 2.501707815008127, 2.538595088454987, 2.569184010543029, 2.592895478152776, 2.611118760780845, 2.624888637988367, 2.634995059488140,  2.642053494939402, 2.646551776080158, 2.648882273952180, 2.649364596167844, 2.648261966881760, 2.645793284993862, 2.642142157175058,
      1.455551976511244, 1.793559836103599, 2.016222558864558, 2.185060560639033, 2.322076276312510, 2.437913148554039, 2.538595088454987, 2.630514848403267, 2.659570698809377, 2.684154454636061, 2.703070127683097, 2.717386129378088, 2.727918040052022,  2.735301274711294, 2.740039461994805, 2.742537673884466, 2.743125863368880, 2.742075777449158, 2.739613406894088, 2.735928312023415,
      1.472071539554080, 1.814502068942717, 2.040096521895055, 2.211143278381126, 2.349930042620272, 2.467241778796656, 2.569184010543029, 2.659570698809377, 2.743695921088902, 2.766305230049162, 2.785830228382558, 2.800625638404431, 2.811529889995672,  2.819196165899417, 2.824142156969015, 2.826784233823230, 2.827461546742052, 2.826453415157028, 2.823992127224201, 2.820272527021312,
      1.484607470874768, 1.830533754148074, 2.058458514685225, 2.231264749927427, 2.371463159423016, 2.489950052166998, 2.592895478152776, 2.684154454636061, 2.766305230049162, 2.844004755293195, 2.861227823602539, 2.876448453481178, 2.887681701982461,  2.895596783040082, 2.900724066252887, 2.903490102357626, 2.904242323047215, 2.903266861870061, 2.900801670356545, 2.897046341648879,
      1.493973058020677, 1.842652548192438, 2.072425606729346, 2.246631326574126, 2.387953548339291, 2.507375737197111, 2.611118760780845, 2.703070127683097, 2.785830228382558, 2.861227823602539, 2.933533039247586, 2.946200478019308, 2.957726700426974,  2.965862003354543, 2.971148307612918, 2.974021432378284, 2.974836344163625, 2.973885372403328, 2.971411615034331, 2.967618978208675,
      1.500773605712803, 1.851601377680545, 2.082829825345476, 2.258141608078218, 2.400353132569224, 2.520515308302347, 2.624888637988367, 2.717386129378088, 2.800625638404431, 2.876448453481178, 2.946200478019308, 3.013909956512797, 3.022684839310162,  3.031016143626015, 3.036442496281942, 3.039408228644414, 3.040275222686663, 3.039341490804034, 3.036854848378839, 3.033023152702185,
      1.505470777477411, 1.857947196481228, 2.090306274903157, 2.266481588761922, 2.409388639833794, 2.530129499616276, 2.634995059488140, 2.727918040052022, 2.811529889995672, 2.887681701982461, 2.957726700426974, 3.022684839310162, 3.086430678251573,  3.091850773983988, 3.097400821511584, 3.100446597200950, 3.101356377156777, 3.100433424960624, 3.097929912310625, 3.094057342955011,
      1.508423886207301, 1.862131533333016, 2.095349691488831, 2.272185717018468, 2.415626285360819, 2.536811028695948, 2.642053494939402, 2.735301274711294, 2.819196165899417, 2.895596783040082, 2.965862003354543, 3.031016143626015, 3.091850773983988,  3.152142518084847, 3.154649880288111, 3.157764697040440, 3.158709051837987, 3.157791091512073, 3.155267036316336, 3.151351776777269,
      1.509917392972469, 1.864504160703294, 2.098352112771816, 2.275677563306525, 2.419514849570127, 2.541029694829150, 2.646551776080158, 2.740039461994805, 2.824142156969015, 2.900724066252887, 2.971148307612918, 3.036442496281942, 3.097400821511584,  3.154649880288111, 3.211904649886118, 3.211866890929356, 3.212838517036374, 3.211920336804234, 3.209372354009234, 3.205412622311969,
      1.510179798584229, 1.865346229170771, 2.099628761957606, 2.277297763026081, 2.421415274590276, 2.543163361222094, 2.648882273952180, 2.742537673884466, 2.826784233823230, 2.903490102357626, 2.974021432378284, 3.039408228644414, 3.100446597200950,  3.157764697040440, 3.211866890929356, 3.266430668486989, 3.264156598178981, 3.263233482663591, 3.260658448260092, 3.256652517405485,
      1.509396969414855, 1.864886589253295, 2.099436310130595, 2.277323731016889, 2.421621544296497, 2.543519811061008, 2.649364596167844, 2.743125863368880, 2.827461546742052, 2.904242323047215, 2.974836344163625, 3.040275222686663, 3.101356377156777,  3.158709051837987, 3.212838517036374, 3.264156598178981, 3.316319648745609, 3.312070993132474, 3.309466019058792, 3.305412227842854,
      1.507721753485774, 1.863313571980112, 2.097986058095904, 2.275983891404253, 2.420375754795262, 2.542352521513453, 2.648261966881760, 2.742075777449158, 2.826453415157028, 2.903266861870061, 2.973885372403328, 3.039341490804034, 3.100433424960624,  3.157791091512073, 3.211920336804234, 3.263233482663591, 3.312070993132474, 3.362079277792734, 3.356079609451986, 3.351976367355088,
      1.505281058306567, 1.860783661885641, 2.095453641635196, 2.273468154464480, 2.417879209868303, 2.539872276549266, 2.645793284993862, 2.739613406894088, 2.823992127224201, 2.900801670356545, 2.971411615034331, 3.036854848378839, 3.097929912310625,  3.155267036316336, 3.209372354009234, 3.260658448260092, 3.309466019058792, 3.356079609451986, 3.404143391929569, 3.396585036957838,
      1.502181151843766, 1.857427994274683, 2.091986302644697, 2.269935765942838, 2.414300733553258, 2.536255866779533, 2.642142157175058, 2.735928312023415, 2.820272527021312, 2.897046341648879, 2.967618978208675, 3.033023152702185, 3.094057342955011,  3.151351776777269, 3.205412622311969, 3.256652517405485, 3.305412227842854, 3.351976367355088, 3.396585036957838, 3.442885480558219;
    
    expected_density_matrix = expected_density_matrix * 1000.0;
   
    double frequency = 0.001666666666667;
    auto cross_spec_density_matrix = test_wittig_sinha.cross_spectral_density(frequency);

    REQUIRE(cross_spec_density_matrix.rows() == expected_density_matrix.rows());
    REQUIRE(cross_spec_density_matrix.cols() == expected_density_matrix.cols());

    for (unsigned int i = 0; i < cross_spec_density_matrix.rows(); ++i) {
      for (unsigned int j = 0; j < cross_spec_density_matrix.cols(); ++j) {
        REQUIRE(cross_spec_density_matrix(i, j) - expected_density_matrix(i, j) + 1.0 ==
            Approx(1.0).epsilon(0.01));	
      }
    }
  }
}
