
------------------Scenario Specific Ground Motion Simulator v1.0------------------

README for Scenario Specific Ground Motion Simulator software.

This software implements the described in references [1,2,3]. 

All comments and questions regarding this package should be addressed to:

Kostas Papakonstantinou <kpapakon@psu.edu>

-----------------------------------------------------------------------------------

Table of contents of this README

1. Package contents
2. Installation
3. Usage instructions
4. References
5. Acknowledgements

===================================================================================

1. PACKAGE CONTENTS

Installer_web.exe
README.txt (this file)
Source code lite version:
Scenario_Specific_Simulation.m
Sxx_sim.m

===================================================================================

2. INSTALLATION

Run the Installer_web.exe file. NOTE: You will need administrator rights to run it.

The software requires that version 9.3 (R2017b) of the MATLAB Runtime is installed. 
If the MATLAB Runtime is not installed in your system, it will prompt you to download 
it freely from the web and install it. 

Alternatively, download and install the MATLAB Runtime for R2017b manually from the 
following link on the MathWorks website: 
http://www.mathworks.com/products/compiler/mcr/index.html

Continue with the installation and choose a folder such as the default or, for example, 
C:\Ground_Motion_Simulator 
If you choose the desktop shortcut option, the output results will be stored on the 
desktop. If you do not want this, do not include a desktop shortcut at the installation 
phase and create one manually aftewards from the compiled executable in the "application" 
folder inside the installation folder you chose. In this case, results will be stored 
in that "application" folder, as will also happen if you run the compiled executable 
directly. 

The source code lite version can be run in Matlab without installation or compilation.     

===================================================================================

3. USAGE INSTRUCTIONS

Main input:
Richter scale magnitude (Mw) 
Closest-to-site rupture distance (Rrup, unit - km)
Shear-wave Velocity (Vs30) averaged over top 30m of soil (unit - m/s)
Number of evolutionary power spectra that user wants to generate
Number of simulated ground motions that user wants to generate per evolutionary power spectrum 

There is also an option to calculate and save acceleration response spectra for all 
simulated ground motions, and an option to save the on screen displayed figures, which 
are related to the first simulated ground motion in each run. 

Seed:
Option to use a user-specified seed or a randomly specified one. Information about 
the previously or the currently used seed is also displayed. This is based on the 
produced scurr.mat file in the chosen installation folder, so DO NOT ERASE that file.    

File type and simulation:
User can choose to save output results in different formats. Mat output files are used 
by Matlab. Same results can be also reported in text or excel file format.   

If present, multiple CPU cores can be utilized in the simulation process. If this option 
is selected, the number of parallel workers can be defined. This choice is computationally 
beneficial for a large number of ground motions. RESULTS CANNOT BE REPLICATED with this 
option however, even if the same seed is specified.    

Output (See also notes in the Installation section):
All results are stored in C:\<CHOSEN FOLDER NAME>\application\Output. Results from each 
software run are stored in individual folders named based on the simulation date and time, 
as C:\<CHOSEN FOLDER NAME>\application\Output\Output_DayMonthYearTime. Separate mat files 
are generated for ground motion simulation information, acceleration, velocity and 
displacement time histories, and response spectra (if that option is selected). The 
generated excel file saves the same results in separate sheets. With the text files 
option, the same results are reported but different files are now generated for each 
simulated ground motion. If the option to save the on screen displayed figures is selected, 
these figures are stored as matlab files .fig in the subdirectory named Figures.   

General notes:
(i)   Total number of ground motions = (number of power spectra) x (number of ground motions per power spectrum). 
      The generated ground motions are numbered based on the relevant power spectrum, i.e. Spec1_Sim1, Spec1_Sim2, Spec2_Sim1, Spec2_Sim2 etc. 
(ii)  Time interval dt for all ground motions is fixed at 0.01sec
(iii) As mentioned, results cannot be replicated if the parallel processing option is used

Source code comments:
The source code lite version only produces the relevant acceleration time histories as 
described in detail in [1]. 
The main function is the Scenario_Specific_Simulation.m and usage instructions are provided 
there. Different fixed parameters, such as the dt time step, the d_om frequency time step 
and others, can be modified in the .m files accordingly. 

===================================================================================

4. REFERENCES

[1] Vlachos C., Papakonstantinou K.G., & Deodatis G. (2018). Predictive model for site 
specific simulation of ground motions based on earthquake scenarios. Earthquake Engineering 
& Structural Dynamics, 47(1), 195-218.

[2] Vlachos C., Papakonstantinou K.G., & Deodatis G. (2016). A multi-modal analytical 
non-stationary spectral model for characterization and stochastic simulation of earthquake 
ground motions. Soil Dynamics and Earthquake Engineering, 80, 177-191.

[3] Vlachos C., Papakonstantinou K.G., & Deodatis G. (2018). Structural applications of a 
predictive stochastic ground motion model: Assessment and use. ASCE-ASME Journal of Risk 
and Uncertainty in Engineering Systems, Part A: Civil Engineering, 4(2), 04018006:1-12.

===================================================================================
5. ACKNOWLEDGEMENTS

This software has been developed by Mr. Yogesh Rathod and Dr. Kostas Papakonstantinou. 


$Id: README, v1.0 2018/02/19 11:08:24 kgpapakon Exp $