open_system('BatterySOCEstimation')

set_param(find_system('BatterySOCEstimation','FindAll', 'on','type','annotation','Tag','ModelFeatures'),'Interpreter','off')

%% Simulation Results
%
% The plot below shows the real and estimated battery state-of-charge.
%


Soc_plot;

%% Results from Real-Time Simulation
%%
%
% This has been tested on a Speedgoat Performance real-time target 
% machine with an Intel(R) 3.5 GHz i7 multi-core CPU. This model can run 

% in real time with a step size of 50 microseconds.
