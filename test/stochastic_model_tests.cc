#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <catch2/catch.hpp>
#include <Eigen/Dense>
#include <nlohmann/json.hpp>
#include "dabaghi_der_kiureghian.h"
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

  SECTION("Test time history generation for all building floors") {
    auto time_histories = test_wittig_sinha.generate("Test");
    REQUIRE(time_histories.get_library_json()["Events"][0]["timeSeries"].size() == num_floors);
    REQUIRE(time_histories.get_library_json()["Events"][0]["pattern"].size() == num_floors);
    
    auto time_histories_json = test_wittig_sinha.generate("Test", "./test_wind.json", false);
    REQUIRE(time_histories_json);
  }

  SECTION("Test different constructors") {
    REQUIRE_NOTHROW(Factory<stochastic::StochasticModel, std::string, double,
                            double, unsigned int, double>::instance()
                        ->create("WittigSinhaDiscreteFreqWind",
                                 std::move(exposure_category),
                                 std::move(gust_speed), std::move(height),
                                 std::move(num_floors), std::move(total_time)));

    REQUIRE_NOTHROW(Factory<stochastic::StochasticModel, std::string, double,
                            double, unsigned int, double, int>::instance()
                        ->create("WittigSinhaDiscreteFreqWind", std::move("D"),
                                 std::move(30.0), std::move(123.0),
                                 std::move(8), std::move(200.0),
                                 std::move(100)));

    REQUIRE_NOTHROW(
        Factory<stochastic::StochasticModel, std::string, double,
                const std::vector<double> &, const std::vector<double> &,
                const std::vector<double> &, double>::instance()
            ->create("WittigSinhaDiscreteFreqWind", std::move("D"),
                     std::move(30.0), std::move(std::vector<double>(1, 0.0)),
                     std::move(std::vector<double>(1, 0.0)),
                     std::move(std::vector<double>{10.0, 23.0, 50.0}),
                     std::move(200.0)));

    REQUIRE_NOTHROW(
        Factory<stochastic::StochasticModel, std::string, double,
                const std::vector<double> &, const std::vector<double> &,
                const std::vector<double> &, double, int>::instance()
            ->create("WittigSinhaDiscreteFreqWind", std::move("D"),
                     std::move(30.0), std::move(std::vector<double>(1, 0.0)),
                     std::move(std::vector<double>(1, 0.0)),
                     std::move(std::vector<double>{10.0, 23.0, 50.0}),
                     std::move(200.0), std::move(25)));
  }

  SECTION("Test that when seed provided subsequent runs are identical") {
    auto run1 = Factory<stochastic::StochasticModel, std::string, double,
                        double, unsigned int, double, int>::instance()
                    ->create("WittigSinhaDiscreteFreqWind", std::move("D"),
                             std::move(30.0), std::move(123.0), std::move(8),
                             std::move(200.0), std::move(100));
    auto run1_history = run1->generate("Run1");

    auto generateTest =
        Factory<stochastic::StochasticModel, std::string, double, double,
                unsigned int, double>::instance()
            ->create("WittigSinhaDiscreteFreqWind", std::move("D"),
                     std::move(30.0), std::move(5.08), std::move(3),
                     std::move(13107.2));
    auto genTest = generateTest->generate("Blah", "./another_wind_test.json", false);
    generateTest->generate("Blah", "./yet_another_wind_test.json", false);
    
    auto run2 = Factory<stochastic::StochasticModel, std::string, double,
                        double, unsigned int, double, int>::instance()
                    ->create("WittigSinhaDiscreteFreqWind", std::move("D"),
                             std::move(30.0), std::move(123.0), std::move(8),
                             std::move(200.0), std::move(100));
    auto run2_history = run2->generate("Run2");

    auto run1_data =
        run1_history.get_library_json()["Events"][0]["timeSeries"][7]["data"]
            .get<std::vector<double>>();
    auto run2_data =
        run2_history.get_library_json()["Events"][0]["timeSeries"][7]["data"]
            .get<std::vector<double>>();

    REQUIRE(run1_data.size() == run2_data.size());
    for (unsigned int i = 0; i < run1_data.size(); ++i) {
      REQUIRE(run1_data[i] - run2_data[i] + 1.0 == Approx(1.0).epsilon(0.01));
    }
  }

  SECTION(
      "Test that trying to generate time histories when x and y are vectors "
      "throws exception since this capability isn't currently implemented") {

    auto vector_case =
        Factory<stochastic::StochasticModel, std::string, double,
                const std::vector<double> &, const std::vector<double> &,
                const std::vector<double> &, double, int>::instance()
            ->create("WittigSinhaDiscreteFreqWind", std::move("D"),
                     std::move(30.0), std::move(std::vector<double>{1.0, 2.0}),
                     std::move(std::vector<double>(1, 0.0)),
                     std::move(std::vector<double>{10.0, 23.0, 50.0}),
                     std::move(200.0), std::move(25));

    REQUIRE_THROWS_AS(vector_case->generate("Test"), std::runtime_error);

    auto non_vector_case =
        Factory<stochastic::StochasticModel, std::string, double,
                const std::vector<double> &, const std::vector<double> &,
                const std::vector<double> &, double>::instance()
            ->create("WittigSinhaDiscreteFreqWind", std::move("D"),
                     std::move(30.0),
                     std::move(std::vector<double>{10.0, 23.0, 50.0}),
                     std::move(std::vector<double>(1, 0.0)),
                     std::move(std::vector<double>(1, 0.0)), std::move(200.0));

    auto non_vector_case_history = non_vector_case->generate("NonVectorCase");

    REQUIRE(non_vector_case_history.get_library_json()["Events"][0]["timeSeries"].size() == 3);
  }
}

TEST_CASE("Test Dabaghi & Der Kiureghian (2018) implementation", "[Stochastic][Seismic]") {
  stochastic::FaultType faulting = stochastic::FaultType::StrikeSlip;
  stochastic::SimulationType simulation_type =
      stochastic::SimulationType::PulseAndNoPulse;
  double moment_magnitude = 6.5, depth_to_rupt = 0.0, rupture_dist = 10.0,
         vs30 = 760.0, s_or_d = 26.0, theta_or_phi = 0.0;
  unsigned int num_sims = 2, num_realizations = 2;
  bool truncate = true;

  stochastic::DabaghiDerKiureghian test_model(
      faulting, simulation_type, moment_magnitude, depth_to_rupt, rupture_dist,
      vs30, s_or_d, theta_or_phi, num_sims, num_realizations, truncate);

  SECTION("Test transformed model parameters calculation and tranformation "
          "back to normal space") {
    auto pulse_params = test_model.compute_transformed_model_parameters(true);
    auto nopulse_params =
        test_model.compute_transformed_model_parameters(false);

    Eigen::VectorXd expected_pulse_params(19);
    expected_pulse_params << 3.7842, 0.5417, 0, 0, 1.6636, 4.8294, 2.0845,
        1.2670, 1.7060, 1.4614, -0.0761, 0.0938, 4.7012, 2.0450, 1.2181, 1.6696,
        1.6212, -0.1735, -0.1250;

    Eigen::VectorXd expected_nopulse_params(14);
    expected_nopulse_params << 4.5267, 2.1629, 1.0427, 1.5920, 1.7794, 0.0068,
        -0.0772, 3.9270, 2.3581, 0.9721, 1.5534, 1.9106, -0.2490, -0.0181;

    for (unsigned int i = 0; i < expected_pulse_params.size(); ++i) {
      REQUIRE(pulse_params(i) + 1.0 ==
              Approx(expected_pulse_params(i) + 1.0).epsilon(0.01));
    }

    for (unsigned int i = 0; i < expected_nopulse_params.size(); ++i) {
      REQUIRE(nopulse_params(i) + 1.0 ==
              Approx(expected_nopulse_params(i) + 1.0).epsilon(0.01));
    }

    test_model.transform_parameters_from_normal_space(true, pulse_params);
    test_model.transform_parameters_from_normal_space(false, nopulse_params);

    Eigen::VectorXd expected_trans_pulse(19);
    expected_trans_pulse << 44.0025, 1.7189, 2.2567, 1.0000, 5.2785, 125.1375,
        8.0403, 3.5502, 5.5070, 4.3121, -0.0602, 0.1555, 110.0749, 7.7290,
        3.3808, 5.3102, 5.0591, -0.0736, 0.1309;

    Eigen::VectorXd expected_trans_nopulse(14);
    expected_trans_nopulse << 92.4499, 8.6960, 2.8370, 4.9137, 5.9262, -0.0496,
        0.1360, 50.7563, 10.5706, 2.6435, 4.7276, 6.7573, -0.0847, 0.1424;

    for (unsigned int i = 0; i < expected_trans_pulse.size(); ++i) {
      REQUIRE(pulse_params(i) + 1.0 ==
              Approx(expected_trans_pulse(i) + 1.0).epsilon(0.01));
    }

    for (unsigned int i = 0; i < expected_trans_nopulse.size(); ++i) {
      REQUIRE(nopulse_params(i) + 1.0 ==
              Approx(expected_trans_nopulse(i) + 1.0).epsilon(0.01));
    }
  }

  SECTION("Test model parameter transformation from normal space") {

    auto pulse_params = test_model.compute_transformed_model_parameters(true);
    auto nopulse_params =
        test_model.compute_transformed_model_parameters(false);

    test_model.transform_parameters_from_normal_space(true, pulse_params);
    test_model.transform_parameters_from_normal_space(false, nopulse_params);
  }

  SECTION("Test model parameter simulation") {

    auto pulse_params = test_model.simulate_model_parameters(true, 20);
    auto nopulse_params = test_model.simulate_model_parameters(false, 18);

    REQUIRE(pulse_params.rows() == 20);
    REQUIRE(pulse_params.cols() == 19);
    REQUIRE(nopulse_params.rows() == 18);
    REQUIRE(nopulse_params.cols() == 14);
  }

  SECTION("Test backcalculation of modulating parameters") {
    // These values are based on those presented in Table 4 of Dabaghi & Der
    // Kiureghian (2017) - Stochastic model for simulation of near-fault ground
    // motions
    Eigen::VectorXd params(4);
    params << 12.0, 14.0, 3.9, 5.7;
    auto backcalced_params =
        test_model.backcalculate_modulating_params(params, 1.7);

    REQUIRE(backcalced_params(0) == Approx(2.1516).epsilon(0.01));
    REQUIRE(backcalced_params(1) == Approx(0.1082).epsilon(0.01));
    REQUIRE(backcalced_params(2) == Approx(6.5923).epsilon(0.01));
    REQUIRE(backcalced_params(3) == Approx(0.0375).epsilon(0.01));

    params << 9.0, 15.2, 3.9, 5.5;
    backcalced_params = test_model.backcalculate_modulating_params(params, 1.7);

    REQUIRE(backcalced_params(0) == Approx(4.4321).epsilon(0.01));
    REQUIRE(backcalced_params(1) == Approx(0.0970).epsilon(0.01));
    REQUIRE(backcalced_params(2) == Approx(5.7577).epsilon(0.01));
    REQUIRE(backcalced_params(3) == Approx(0.0324).epsilon(0.01));
  }

  SECTION("Test modulating function, time-to-intensity, linear filter, and "
          "frequency response filter calculations") {
    double start_time = 0.0;
    unsigned int num_steps = 5;
    Eigen::VectorXd params(4);
    params << 1.0, 2.0, 3.0, 4.0;
    Eigen::VectorXd filter_params(2);
    filter_params << 2.0, 3.0;

    auto mod_func = test_model.calc_modulating_func(num_steps, start_time, params);
    
    REQUIRE(mod_func[0] + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(mod_func[1] == Approx(0.0067).epsilon(0.01));
    REQUIRE(mod_func[2] == Approx(0.0133).epsilon(0.01));
    REQUIRE(mod_func[3] == Approx(0.02).epsilon(0.01));
    REQUIRE(mod_func[4] == Approx(0.0267).epsilon(0.01));

    double t01 = test_model.calc_time_to_intensity(mod_func, 1.0);
    double t30 = test_model.calc_time_to_intensity(mod_func, 30.0);
    double t99 = test_model.calc_time_to_intensity(mod_func, 99.0);

    REQUIRE(t01 == Approx(0.01).epsilon(0.01));
    REQUIRE(t30 == Approx(0.02).epsilon(0.01));
    REQUIRE(t99 == Approx(0.025).epsilon(0.01));

    auto frequency_filter =
        test_model.calc_linear_filter(num_steps, filter_params, t01, t30, t99);

    REQUIRE(frequency_filter[0] == Approx(12.3779).epsilon(0.01));
    REQUIRE(frequency_filter[1] == Approx(12.3779).epsilon(0.01));
    REQUIRE(frequency_filter[2] == Approx(12.3779).epsilon(0.01));
    REQUIRE(frequency_filter[3] == Approx(12.4721).epsilon(0.01));
    REQUIRE(frequency_filter[4] == Approx(12.5664).epsilon(0.01));

    Eigen::MatrixXd impulse_response = test_model.calc_impulse_response_filter(
        num_steps, frequency_filter, 0.5);

    Eigen::MatrixXd expected_response = Eigen::MatrixXd::Zero(num_steps, num_steps);
    expected_response <<    
      0,    1.0000,    0.8885,    0.7901,    0.7130,
      0,         0,    0.4589,    0.5446,    0.5534,
      0,         0,         0,    0.2813,    0.3814,
      0,         0,         0,         0,    0.2000,
      0,         0,         0,         0,         0;

    REQUIRE(impulse_response.lpNorm<2>() ==
            Approx(expected_response.lpNorm<2>()).epsilon(0.01));
  }

  SECTION("Test acceleration filter") {
    double freq_corner = std::pow(10, 1.4071 - 0.3452 * moment_magnitude);
    unsigned int filter_order = 4;
    Eigen::VectorXd accel(6);
    accel << 0.0, 1.0, -1.0, 2.0, -2.0, 3.0;

    auto filtered_accel = test_model.filter_acceleration(accel, freq_corner, filter_order);

    std::vector<double> expected_accel = {-0.5000, 0.5000,  -1.5000,
                                          1.5000,  -2.5000, 2.5000};


    REQUIRE(filtered_accel[0] == Approx(expected_accel[0]).epsilon(0.01));
    REQUIRE(filtered_accel[1] == Approx(expected_accel[1]).epsilon(0.01));
    REQUIRE(filtered_accel[2] == Approx(expected_accel[2]).epsilon(0.01));
    REQUIRE(filtered_accel[3] == Approx(expected_accel[3]).epsilon(0.01));
    REQUIRE(filtered_accel[4] == Approx(expected_accel[4]).epsilon(0.01));
    REQUIRE(filtered_accel[5] == Approx(expected_accel[5]).epsilon(0.01));    
  }

  SECTION("Test pulse acceleration calculation") {
    Eigen::VectorXd params(5);
    params << 2.0, 3.0, 4.0, 5.0, 6.0;
    unsigned int num_steps = 8;

    auto pulse_accel = test_model.calc_pulse_acceleration(num_steps, params);
    Eigen::VectorXd expected_accel(8);
    expected_accel << 0, -0.0699, -0.2095, -0.3491, -0.4884, -0.6274, -0.7659,
        -0.9040;
    expected_accel = expected_accel * 1.0e-5;

    REQUIRE(pulse_accel[0] + 1.0 ==
            Approx(expected_accel[0] + 1.0).epsilon(0.01));
    REQUIRE(pulse_accel[1] == Approx(expected_accel[1]).epsilon(0.01));
    REQUIRE(pulse_accel[2] == Approx(expected_accel[2]).epsilon(0.01));
    REQUIRE(pulse_accel[3] == Approx(expected_accel[3]).epsilon(0.01));
    REQUIRE(pulse_accel[4] == Approx(expected_accel[4]).epsilon(0.01));
    REQUIRE(pulse_accel[5] == Approx(expected_accel[5]).epsilon(0.01));
    REQUIRE(pulse_accel[6] == Approx(expected_accel[6]).epsilon(0.01));
    REQUIRE(pulse_accel[7] == Approx(expected_accel[7]).epsilon(0.01));
  }
  
  SECTION("Test JSON generation") {
    bool success = test_model.generate("BlahBlah", "./dabaghi_test.json", true);
  }
}
