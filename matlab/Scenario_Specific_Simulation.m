function    GM_pool  =  Scenario_Specific_Simulation(M,R,V,N_spec,N_sim)

%Main function for scenario specific ground motion simulations. 

%Input:
%Richter scale magnitude (M) 
%Closest-to-site rupture distance (R, unit - km)
%Shear-wave Velocity (V) averaged over top 30m of soil (unit - m/s)
%Number of evolutionary power spectra that user wants to generate (N_spec)
%Number of simulated ground motions that user wants to generate per evolutionary power spectrum (N_sim)

%Output: GM_pool cell with dimensions (1,total number of ground motions = (N_spec) x (N_sim)) containing the acceleration time histories. 
%The generated ground motions are numbered sequentially based on the relevant power spectrum, i.e. Spec1_Sim1,Spec1_Sim2,Spec2_Sim1,Spec2_Sim2 etc. 

%Based on the paper: 
%Vlachos C., Papakonstantinou K.G., & Deodatis G. (2018). 
%Predictive model for site specific simulation of ground motions based on earthquake scenarios. 
%Earthquake Engineering & Structural Dynamics, 47(1), 195-218. 

%{
Copyright (c) <2018> <COPYRIGHT HOLDER> All rights reserved.
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
%}

%More information is in the README file.


%--------------------------------------------------------------------------

 dt     =   0.01                                                         ;
 
 d_om   =   0.20                                                         ;

%-------------------------------------------------------------------------- 

% Sets seed for random number generator based on current time
 rng('shuffle')                                                          ;
 
%--------------------------------------------------------------------------

% Factors for site condition based on Vs
 S_S = 0                                                                 ;

 S_M = 0                                                                 ;
 
 S_H = 0                                                                 ;
 
%---------------------- 
 
 if     V   <= 300
        S_S  = 1                                                         ;              
 elseif V   <= 450
        S_M  = 1                                                         ;
 else
        S_H  = 1                                                         ;
 end       

%--------------------------------------------------------------------------
 
% Normalize input parameters
 M  =  M/6                                                               ;
 
 R  = (R+5)/30                                                           ;
 
 V  =  V/450                                                             ;

%--------------------------------------------------------------------------

% Coefficients of linear regression, given on page 12, Eq-25.
 XX = [ 1 , M , log(R) , M*log(R) , S_S*log(V) , S_M*log(V) , S_H*log(V) ]  ;

%--------------------------------------------------------------------------
% Restricted maximum likelihood method regression coefficients of
% normal model parameters--see page 13, Table 3
 Beta = [ ...
-1.1417	1.0917	1.9125	-0.9696	0.0971	0.3476	-0.6740
1.8052	-1.8381	-3.5874	3.7895	0.3236	0.5497	0.2876
1.8969	-1.8819	-2.0818	1.9000	-0.3520	-0.6959	-0.0025
1.6627	-1.6922	-1.2509	1.1880	-0.5170	-1.0157	-0.1041
3.8703	-3.4745	-0.0816	0.0166	0.4904	0.8697	0.3179
1.1043	-1.1852	-1.0068	0.9388	-0.5603	-0.8855	-0.3174
1.1935	-1.2922	-0.7028	0.6975	-0.6629	-1.1075	-0.4542
1.7895	-1.5014	-0.0300	-0.1306	0.4526	0.7132	0.1522
-3.6404	3.3189	-0.5316	0.3874	-0.3757	-0.8334	0.1006
-2.2742	2.1454	0.6315	-0.6620	0.1093	-0.1028	-0.0479
0.6930	-0.6202	1.8037	-1.6064	0.0727	-0.1498	-0.0722
1.3003	-1.2004	-1.2210	1.0623	-0.0252	0.1885	0.0069
0.4604	-0.4087	-0.5057	0.4486	0.1073	-0.0219	-0.1352
2.2304	-2.0398	-0.1364	0.1910	0.2425	0.1801	0.3233
2.3806	-2.2011	-0.3256	0.2226	-0.0221	0.0970	0.0762
0.2057	-0.1714	0.3385	-0.2229	0.0802	0.2649	0.0396      
-7.6011	6.8507 	-2.3609	0.9201 	-0.7508	-0.7903	-0.6204
-6.3472	5.8241	3.2994	-2.8774	-0.1411	-0.5298	-0.0203 ]                ;

%--------------------------------------------------------------------------

% Variance of model parameters--see page 13, Table 3, last
% column. Note that entry orders are different order than in Table 3
 Var = [ 0.90 , 0.80 , 0.78 , 0.74 , 0.66 , 0.73 , 0.72 , 0.70 , 0.69 ...
         0.78 , 0.90 , 0.90 , 0.90 , 0.90 , 0.80 , 0.90 , 0.35 , 0.80  ] ;
     
%--------------------------------------------------------------------------

% Estimated correlation matrix from page 24, Table A1. Again,
% notice ordering in different compared to paper.
 Corr = [ ...
1.0000	0.0382	-0.0912	-0.0701	-0.0214	-0.0849	-0.0545	-0.0185	0.0270	-0.0122	0.0059	-0.0344	-0.0342	0.0409	-0.0137	-0.0168	-0.0990	-0.6701
0.0382	1.0000	-0.1159	-0.1856	0.0681	-0.2018	-0.2765	-0.0304	-0.1719	-0.1157	-0.0347	-0.0277	-0.0189	0.0357	0.0657	-0.0070	0.3690	-0.0510
-0.0912	-0.1159	1.0000	0.9467	0.4123	0.4815	0.4240	0.2120	0.1070	-0.1898	0.0506	-0.0661	-0.0380	0.0260	0.0506	-0.0317	-0.0278	0.0245
-0.0701	-0.1856	0.9467	1.0000	0.4075	0.4891	0.4940	0.2285	0.2009	-0.1709	0.0365	-0.0579	-0.0999	0.0467	0.0410	0.0027	-0.0966	0.0631
-0.0214	0.0681	0.4123	0.4075	1.0000	0.1772	0.1337	0.7315	-0.0066	-0.2787	0.0703	-0.0541	-0.0453	0.1597	0.0792	0.0220	0.0606	-0.0844
-0.0849	-0.2018	0.4815	0.4891	0.1772	1.0000	0.9448	0.3749	0.1682	-0.0831	0.0124	-0.1236	-0.0346	-0.0054	0.0877	-0.0197	-0.0867	0.0281
-0.0545	-0.2765	0.4240	0.4940	0.1337	0.9448	1.0000	0.3530	0.2305	-0.0546	-0.0223	-0.0782	-0.0872	0.0074	0.0999	0.0066	-0.1358	0.0626
-0.0185	-0.0304	0.2120	0.2285	0.7315	0.3749	0.3530	1.0000	0.1939	-0.0617	-0.0017	-0.0942	-0.0332	0.0813	0.0810	-0.0032	-0.0870	-0.0599
0.0270	-0.1719	0.1070	0.2009	-0.0066	0.1682	0.2305	0.1939	1.0000	-0.1851	-0.2073	-0.0756	-0.1637	-0.0865	0.0699	-0.0485	-0.2153	0.0320
-0.0122	-0.1157	-0.1898	-0.1709	-0.2787	-0.0831	-0.0546	-0.0617	-0.1851	1.0000	0.2139	0.0769	0.1391	0.0769	-0.1838	0.0377	-0.1615	0.1000
0.0059	-0.0347	0.0506	0.0365	0.0703	0.0124	-0.0223	-0.0017	-0.2073	0.2139	1.0000	-0.1102	-0.0530	0.0791	0.0012	0.0090	-0.0236	0.0037
-0.0344	-0.0277	-0.0661	-0.0579	-0.0541	-0.1236	-0.0782	-0.0942	-0.0756	0.0769	-0.1102	1.0000	-0.2562	-0.0406	0.3154	0.0065	-0.0093	-0.0354
-0.0342	-0.0189	-0.0380	-0.0999	-0.0453	-0.0346	-0.0872	-0.0332	-0.1637	0.1391	-0.0530	-0.2562	1.0000	-0.1836	-0.1624	-0.5646	0.0216	0.0243
0.0409	0.0357	0.0260	0.0467	0.1597	-0.0054	0.0074	0.0813	-0.0865	0.0769	0.0791	-0.0406	-0.1836	1.0000	0.1624	0.1989	0.0549	-0.0411
-0.0137	0.0657	0.0506	0.0410	0.0792	0.0877	0.0999	0.0810	0.0699	-0.1838	0.0012	0.3154	-0.1624	0.1624	1.0000	0.1552	0.0844	-0.0637
-0.0168	-0.0070	-0.0317	0.0027	0.0220	-0.0197	0.0066	-0.0032	-0.0485	0.0377	0.0090	0.0065	-0.5646	0.1989	0.1552	1.0000	0.0058	0.0503
-0.0990	0.3690	-0.0278	-0.0966	0.0606	-0.0867	-0.1358	-0.0870	-0.2153	-0.1615	-0.0236	-0.0093	0.0216	0.0549	0.0844	0.0058	1.0000	-0.0930
-0.6701	-0.0510	0.0245	0.0631	-0.0844	0.0281	0.0626	-0.0599	0.0320	0.1000	0.0037	-0.0354	0.0243	-0.0411	-0.0637	0.0503	-0.0930	1.0000 ] ;        

%--------------------------------------------------------------------------

% Standard normal parameter (mean)
 Mean   = XX*Beta'                                                       ;

 % Convert standard deviation and correlation to covariance
 % MATLAB function
 Sigma  = corr2cov( sqrt(Var) , Corr )                                   ; 
 
%--------------------------------------------------------------------------

% Random realizations (Sample model parameters)
% MATLAB function
 Z_sim  = mvnrnd(Mean,Sigma,N_spec)                                      ;
 
%--------------------------------------------------------------------------

% Different distributions for model parameters
% LOTS of MATLAB functions...

 PD{1}  = makedist('Lognormal',-1.735,0.523)                             ;
 
 PD{2}  = makedist('Lognormal', 1.009,0.422)                             ;
 
%---------------------------------------------- 

 PD{3}  = makedist('Normal',    0.249,1.759)                             ;
 
 PD{4}  = makedist('Normal',    0.768,1.958)                             ;
 
 PD{5}  = makedist('Lognormal', 2.568,0.557)                             ;
 
 PD{6}  = makedist('Normal',    0.034,1.471)                             ;
 
 PD{7}  = makedist('Normal',    0.441,1.733)                             ;
 
 PD{8}  = makedist('Lognormal', 3.356,0.473)                             ;

%----------------------------------------------

 PD{9}  = makedist('Beta',      2.516,9.174)                             ;
 
 PD{10} = makedist('Beta',     3.582,15.209)                             ;
 
%----------------------------------------------

 PD{11}  = makedist('Lognormal',      0.746,0.404)                       ;
 
 PD{12}  = makedist('tLocationScale', 0.205,0.232,7.250)                 ;
 
 PD{13}  = makedist('InverseGaussian',0.499,0.213)                       ;
 
 PD{14}  = makedist('Lognormal',      0.702,0.435)                       ;
 
 PD{15}  = makedist('tLocationScale', 0.792,0.157,4.223)                 ;
 
 PD{16}  = makedist('InverseGaussian',0.350,0.170)                       ;

%----------------------------------------------
 
 PD{17}  = makedist('Lognormal',9.470,1.317)                             ;
 
 PD{18}  = makedist('Lognormal',3.658,0.375)                             ;

%--------------------------------------------------------------------------

 P_sim = zeros(size(Z_sim))                                              ;

 % Transform sample normal model parameters back to physical space
 for k = 1:18
     P_sim(:,k) = icdf( PD{k} , normcdf(Z_sim(:,k)) )                    ;       
 end    

%--------------------------------------------------------------------------

% cell is essentially a tuple in MATLAB
 accel_pool = cell(1,N_spec)                                             ;

 % Calculate sample non-stationary power spectrum following Eq-18
 % on page 8. Method described in Section 5.
 for k = 1:N_spec
     PP            = P_sim(k,:)                                          ;
     accel_pool{k} = Sxx_sim(PP,N_sim,dt,d_om,PD,Mean,Sigma)             ;
     disp(['Sxx ',num2str(k),' done '])                                  ;
 end     
 
 disp('Simulation done')                                                 ;
   
%--------------------------------------------------------------------------

 GM_pool = cell(1,(N_spec*N_sim))                                        ;
 
 if   N_sim == 1
    %------------------------
      GM_pool = accel_pool                                               ;
    %------------------------
 else
    %------------------------ 
      for k = 1:N_spec
          ind  = (k-1)*N_sim                                             ;
          pool =  accel_pool{k}                                          ; 
          for j = 1:N_sim
              GM_pool{ind+j} = pool(j,:)                                 ;
          end
      end
    %------------------------  
 end
 
%--------------------------------------------------------------------------
 
end

%--------------------------------------------------------------------------
