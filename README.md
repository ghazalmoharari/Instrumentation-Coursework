# Instrumentation & Measurement Systems

## Overview
This repository contains a comprehensive collection of assignments and projects completed for the Instrumentation course at the University of Tehran. The repository bridges the gap between theoretical measurement principles and practical embedded systems design. It covers sensor characterization, analog signal conditioning, data-driven modeling, and the programming of STM32 ARM Cortex-M microcontrollers to interface with industrial sensors and actuators.

## Coursework Structure

### [HW1: Control Loops, Estimation, & Sensor Dynamics](./HW1/)
Focuses on the mathematical and statistical foundations of instrumentation.
*   **Topics:** 4-20mA industrial loops, Least Squares (LS) & Recursive Least Squares (RLS) polynomial fitting, statistical sensor evaluation (accuracy, precision, hysteresis, drift), and step response analysis of Type 0, 1, and 2 systems.

### [HW2: Analog Signal Conditioning & Data Conversion](./HW2/)
Explores the hardware required to filter and digitize raw sensor signals.
*   **Topics:** Passive RC filters (High-pass/Low-pass), Active LC Notch filters, Op-Amp comparators, 4-bit R-2R DAC design, and Instrumentation Amplifiers (AD620) for Wheatstone bridges and biomedical signals.

### [HW3: STM32 Microcontroller Basics & Timers](./HW3/)
Introduces STM32CubeIDE and embedded C programming for real-time hardware control.
*   **Topics:** GPIO configuration, 7-Segment displays, External Interrupts (EXTI), Hardware Timers, PWM generation (L298 DC motor drivers), and 12-bit Analog-to-Digital Conversion (ADC) for smart sensor identification.

### [HW4: Temperature Sensors & Digital Scales](./HW4/)
Focuses on temperature measurement systems and high-precision physical simulations.
*   **Topics:** Characterization of RTDs (PT100), Thermistors (NTC), and Thermocouples. I2C communication with LM75 digital sensors. Design of a complete digital weight scale simulating load cell vibrations, smoothed via an active Low-Pass Filter and implementing a software Tare function.

### [HW5: Motor Control, Solar Tracking, & Encoders](./HW5/)
Covers the integration of microcontrollers with mechanical actuators and position sensors.
*   **Topics:** DC motor H-Bridge control, open-loop stepper motor positioning for an automated solar tracker, Ultrasonic (HC-SR04) & Infrared (IR) obstacle detection, and high-precision rotary encoder decoding (4X mode) for RPM and angle measurement.

### [HW6: Environmental Sensors & UI State Machines](./HW6/)
Integrates complex feedback loops and human-machine interfaces using a wide array of sensors.
*   **Topics:** Interfacing MQ-4 (Gas), MPX4115 (Pressure), and LDR (Light) sensors. Measuring water flow via pulse frequency counting. Designing an LCD/Button state machine to control a DHT11-driven climate system (heater, humidifier, fan). Monitoring BMP180 barometric data via I2C and streaming results to a UART Virtual Terminal.

## Software & Tools Used
*   **Programming Language:** C (STM32 HAL Library), MATLAB
*   **IDEs & Environments:** STM32CubeIDE, MATLAB
*   **Simulation & Hardware Design:** Proteus Design Suite (Isis)
