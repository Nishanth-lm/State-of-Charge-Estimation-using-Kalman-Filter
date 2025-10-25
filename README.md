# Battery State of Charge (SoC) Estimation using EKF & UKF

This project implements Extended Kalman Filter (EKF) and Unscented Kalman Filter (UKF) algorithms to estimate the State of Charge (SoC) of a Li-ion battery using measured voltage and current data.  
It includes both C and Python implementations for simulation, estimation, and visualization.

---

## Overview

Accurate estimation of the State of Charge (SoC) is essential for reliable and safe Battery Management Systems (BMS).  
This project compares two nonlinear estimation techniques — EKF and UKF— applied to a Li-ion battery model that captures the nonlinear voltage–SoC relationship.

---

## Features

-  Extended Kalman Filter (EKF) implementation in C  
-  Unscented Kalman Filter (UKF) implementation in C  
-  Nonlinear Li-ion battery voltage model  
-  Python script for post-processing and visualization  
-  CSV-based I/O for easy integration  
-  Modular and extendable structure  
