# Instrumentation - Homework 1

## Overview
This folder contains the solutions, datasets, and MATLAB implementations for Homework 1 of the Instrumentation course. The project explores foundational instrumentation principles, including control loop architecture, signal standards, sensor calibration, statistical evaluation of sensor performance, and system dynamics[cite: 36, 66, 71].

## Topics Covered

### Q1: Control Loop Architecture & 4-20mA Standards
*   **Sensor & Transmitter Roles:** Analysis of the sensor as the physical measurement interface and the transmitter as the signal converter[cite: 36, 66].
*   **Signal Conversion:** Calculating physical values from the standard 4-20mA industrial loop[cite: 36].
*   **Bias Error:** Evaluating the impact of sensor bias (a constant 0.2m offset) on the control loop's steady-state error[cite: 37].

### Q2: Polynomial Estimation (LS & RLS)
*   **Model Fitting:** Applying Least Squares (LS) and Recursive Least Squares (RLS) to estimate polynomial models of degrees 1, 3, and 9[cite: 38, 39, 72].
*   **Mathematical Derivation:** Proving the closed-form LS solution mathematically[cite: 42].
*   **Model Selection:** Evaluating polynomial degrees (1 through 10) by comparing Mean Squared Error (MSE) on training and testing datasets to balance underfitting and overfitting[cite: 44, 73].

### Q3: Sensor Performance Evaluation
*   **Statistical Analysis:** Comparing three industrial boiler pressure sensors based on inaccuracy, precision, repeatability, and reproducibility[cite: 46, 47, 48, 68].
*   **Sensitivity:** Calculating output change per unit input to determine the most reliable and sensitive sensor for the operating range[cite: 50, 69].

### Q4: Sensor Calibration, Hysteresis, & Drift
*   **Hysteresis & Dead Zone:** Plotting increasing and decreasing calibration data to identify hysteresis loops and calculating the maximum hysteresis error[cite: 51, 54, 74]. Locating the input range where the sensor produces no output change[cite: 52].
*   **Drift Calculation:** Evaluating Zero Drift and Sensitivity Drift across different operating temperatures[cite: 55, 56, 70].

### Q5: System Dynamics & Step Responses
*   **System Type Identification:** Analyzing step response plots to classify sensors by their dynamic behavior (Type 0, Type 1, and Type 2 systems)[cite: 57, 75].
*   **Parameter Extraction:** Utilizing MATLAB's `cftool` to fit custom curves and extract transfer function coefficients including time constants, natural frequencies, and damping ratios[cite: 58, 59, 60, 61, 71].

## Files Included
*   **Documentation:** `HW1.pdf` (detailed Persian report) and `Inst_HW1.pdf` (assignment questions)[cite: 36, 65].
*   **Scripts:** `Q2.m`, `Q2_B.m`, `Q4.m`, `Q5.m` for LS/RLS estimation, calibration plotting, and step response analysis[cite: 72, 73, 74, 75].
*   **Datasets:** `data_A.csv`, `data_B_train.csv`, `data_B_test.csv`, `SensorDataQ2.csv`, and `Data.csv`.
