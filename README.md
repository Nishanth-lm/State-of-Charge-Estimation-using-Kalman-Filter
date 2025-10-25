The Extended Kalman filter is the nonlinear version of the Kalman filter which linearizes about an estimate of the current mean and covariance.

Extended Kalman Filter operates in two main stages:

Prediction (Time Update) – Estimating the system state and uncertainty at the next time step.
Correction (Measurement Update) – Refining the state estimate using new measurements.


State of charge using Extended Kalman filter :
Represents the available capacity of a battery as a percentage of its total capacity
System Overview :  Model consists of the following inputs:
                • Cell Voltage (V)            • Current (I)
                • Cell Temperature (T)        • Initial SOC

