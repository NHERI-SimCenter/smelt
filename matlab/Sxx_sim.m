function   fi_pool  =  Sxx_sim(PP,N_sim,dt,d_om,PD,Mean,Sigma)  


%Copyright (c) <2018> <COPYRIGHT HOLDER> All rights reserved.
% More information is in the README file.


%--------------------------------------------------------------------------

% Calculate model parameters
  PP      =  P_check(PP,PD,Mean,Sigma)                                   ; 

%-------------------------------------------------------------------------- 
 
  IA      = PP(17)                                                       ;

  TD      = PP(18)                                                       ;

  % Corner frequency
  om_cut  = 220                                                          ;
  
%-------------------------------------------------------------------------- 
  
  omega   =  0:d_om:om_cut                                               ;

  time    =  0:dt:TD                                                     ;
  
  time(1) =  10^(-6)                                                     ;
  
%--------------------------------------------------------------------------

  time_ndm      =  time/time(end)                                        ;

  energy_pars   =  PP( 1:2 )                                             ; 
  
  energy        =  energy_sim(energy_pars,time_ndm)                      ;

%--------------------------------------------------------------------------

  omega_1_pars  =  PP( 3:5 )                                             ; 

  omega_1       =  omega_sim(omega_1_pars,energy)                        ;

%-----------------------------

  omega_2_pars  =  PP( 6:8 )                                             ; 

  omega_2       =  omega_sim(omega_2_pars,energy)                        ;
                
%--------------------------------------------------------------------------

  zeta_1  =  PP( 9 )                                                     ;   
  
  zeta_2  =  PP( 10 )                                                    ;
  
%--------------------------------------------------------------------------

  R_pars   =  PP( 11:16 )                                                ;
  
  R        =  R_sim(R_pars,energy)                                       ;

  S_2      =  10.^R                                                      ;
  
%--------------------------------------------------------------------------

  z_2      =  z_2_sim(TD,IA,energy_pars,time_ndm)                        ;                            

%-------------------------------------------------------------------------- 

  N    =  4                                                              ;

  fc   =  0.20                                                           ;
  
  wc   =  2*pi*fc                                                        ;

  HP2  = (omega/wc).^(2*N)./( 1+(omega/wc).^(2*N) )                      ;

%--------------------------------------------------------------------------

  Sxx     =  zeros( length(time),length(omega) )                         ;
  
 for i = 1:length(time)
   
     Sxx(i,:) = KT_2( [ omega_1(i) , zeta_1 , 1 ,      ...
                        omega_2(i) , zeta_2 , S_2(i) ] , omega , HP2 )   ;

     Sxx(i,:) = Sxx(i,:)/( 2*d_om*trapz( Sxx(i,:) ) )                    ;

     Sxx(i,:) = z_2(i).*Sxx(i,:)                                         ;

 end
 
%--------------------------------------------------------------------------

  [b,a]     =  butter(N,fc/(1/dt/2),'high')                              ; 

  time_len  =  1.5*(N/2)/fc                                              ;

  time_len  =  roundn(time_len,0)/dt + 1                                 ;
 
  [h,~ ]    =  impz(b,a,time_len)                                        ;
                    
%--------------------------------------------------------------------------

  fi_pool =  zeros(N_sim,(length(time)+length(h)-1))                     ;
  
  for k = 1:N_sim
      fi           = SRM(d_om,dt,Sxx)                                    ;
         
      fi_pool(k,:) = filt_BW(fi,dt,h)                                    ;
  end
  
%--------------------------------------------------------------------------

end

%--------------------------------------------------------------------------
%--------------------------------------------------------------------------

% This function calculates the values the parameters, using Eq-8
% from page 6 to check validity.
function   PP  =  P_check(PP,PD,Mean,Sigma)

%--------------------------------------------------------------------------

  energy     =  0:0.05:1                                                 ;
%-----------------------------

  om_1_pars  =  PP( 3:5 )                                                ; 

  om_1       =  omega_sim(om_1_pars,energy)                              ;

%-----------------------------

  om_2_pars  =  PP( 6:8 )                                                ; 

  om_2       =  omega_sim(om_2_pars,energy)                              ;
                
%-----------------------------

  M1         =  PP( 12 )                                                 ;
  
  M2         =  PP( 15 )                                                 ;
  
%--------------------------------------------------------------------------  
  
% Iterate to find values of alpha, beta and Q from Eq-8 on page 6.
  while  min( om_2 > om_1 ) == 0  ||  ( M1 > M2 )
      
      %-----------------------------
      
         Z  = mvnrnd(Mean,Sigma,1)                                       ;
         
         PP = zeros(size(Z))                                             ;
       
       %-----------------------------

       % Transform sample parameters to physical space
         for k = 1:18
             PP(k) = icdf( PD{k} , normcdf( Z(k) ) )                     ;
         end    
  
       %-----------------------------

         om_1_pars  =  PP( 3:5 )                                         ; 

         om_1       =  omega_sim(om_1_pars,energy)                       ;

       %-----------------------------

         om_2_pars  =  PP( 6:8 )                                         ; 

         om_2       =  omega_sim(om_2_pars,energy)                       ;
                
       %-----------------------------

         M1         =  PP( 12 )                                          ;
  
         M2         =  PP( 15 )                                          ;
         
       %-----------------------------
       
  end 
  
%--------------------------------------------------------------------------

end

% This function calculates non-dimensional cumulative energy from
% Eq-5 on page 6.
function   y   =  energy_sim(x,time)

%--------------------------------------------------------------------------

 y = exp( -( time./x(1) ).^( -x(2) ) ) ./ exp( -( 1./x(1) ).^( -x(2) ) ) ;

%--------------------------------------------------------------------------

end

% This function calculates Eq-8 on page 6. Captures frequency
% non-stationarities of the ground motion, identifying dominant
% modal frequencies.
function   y   =  omega_sim(x,energy)

%--------------------------------------------------------------------------

 y = x(3) * ( 0.5+energy ).^( x(1) ).*( 1.5-energy ).^( x(2) )           ;         
                                        
%--------------------------------------------------------------------------

end

function   y   =  R_sim(x,energy)

%--------------------------------------------------------------------------

 y  =  x(1) .* exp( -((energy-x(2))./x(3)).^2 ) +   ...
       x(4) .* exp( -((energy-x(5))./x(6)).^2 ) - 2                      ;

%--------------------------------------------------------------------------

end

function   y   =  z_2_sim(TD,IA,energy_pars,time_ndm)

%--------------------------------------------------------------------------

 g  =  energy_pars(1)                                                    ;

 d  =  energy_pars(2)                                                    ;   
   
 y   =  IA * (d/g)/TD * exp( (1/g)^(-d) - (time_ndm/g).^(-d) ) .* ...
                                          (time_ndm/g).^(-1-d)           ;
                                      
%--------------------------------------------------------------------------                                       

end

function   y   =  KT_2(x,omega,HP2)

%--------------------------------------------------------------------------

 y  =  x(3) * ( 1+4*x(2)^2*(omega/x(1)).^2 ) ./ ( (1-(omega/x(1)).^2).^2+4*x(2)^2*(omega/x(1)).^2 ) + ...
                                                                                                      ...
       x(6) * ( 1+4*x(5)^2*(omega/x(4)).^2 ) ./ ( (1-(omega/x(4)).^2).^2+4*x(5)^2*(omega/x(4)).^2 )       ;

 y  =  HP2.*y                                                                                             ;                
 
%-------------------------------------------------------------------------------------------------------------
       
end

function   fi  =  SRM(d_om,dt,Sxx)

     % -------------------------------------------------------
     %  COMPUTE GROUND ACCELERATION SAMPLE TIME HISTORY fi(t)
     % -------------------------------------------------------

        [ NT , NW ] = size(Sxx)       ;
 
          fi        = zeros(1,NT)     ;
         
     % ------------------------------------     
         
       omega =  ( 0:1:(NW-1) )*d_om   ;
    
       time  =  ( 0:1:(NT-1) )*dt     ;    
       
     % -------------------------------------
           
         phi_n =  rand(1,NW)*(2*pi)   ;
         
       
       for n  = 1:NW 
 
           wn  =  omega(n)                                               ;     

           phi =  phi_n(n)                                               ; 
   
           fi  =  fi + sqrt( Sxx(:,n)' ).*cos( wn*time + phi )           ;

       end

       fi  =  2*sqrt(d_om).*fi                                           ;

%--------------------------------------------------------------------------

end

function   fi  =  filt_BW(fi,dt,h)

%--------------------------------------------------------------------------

   t_hann_2  =  1                                                        ;   
 
   NT        =  length(fi)                                               ;        

   Window    =  ones(1,NT)                                               ;
    
   N_hann    =  t_hann_2/dt + 1                                          ; 

   N_2_hann  = (N_hann - 1)/2                                            ;
    
   W_hann    =  hann(N_hann)'                                            ;
     
%--------------------------------------------------------------------------

   Window( 1:N_2_hann )            =  W_hann(1:N_2_hann)                 ;                    

   Window( (end-N_2_hann+1):end )  =  fliplr( W_hann(1:N_2_hann) )       ;

%--------------------------------------------------------------------------

   fi  =  Window.*( fi - mean(fi) )                                      ;
 
   fi  =  conv(h',fi)                                                    ;   

%--------------------------------------------------------------------------

end

%--------------------------------------------------------------------------
