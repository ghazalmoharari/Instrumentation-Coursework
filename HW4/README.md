# Instrumentation - Homework 4

## Overview
This folder contains the solutions, embedded C code, and Proteus simulations for Homework 4 of the Instrumentation course. The project focuses on practical sensor interfacing with STM32 microcontrollers, specifically covering temperature sensors (RTD, Thermocouple, Thermistor), I2C digital communication, instrumentation amplifier design (AD620), and the complete simulation of a digital weight scale with active filtering and a software Tare function[cite: 151, 172, 177].

## Topics Covered

### Q1: Temperature Sensor Characterization & Interfacing
*   **Sensor Identification:** Used MATLAB's `cftool` to classify three datasets based on their characteristic curves:
    *   **RTD (PT100):** Identified by its highly linear resistance-temperature response ($R_0 \approx 100\Omega$, positive slope)[cite: 152, 154].
    *   **Thermistor (NTC):** Identified by its non-linear, exponential resistance decay as temperature increases[cite: 152, 153].
    *   **Thermocouple (Type K):** Identified by a linear voltage-temperature response with a sensitivity of $\approx 40\mu V/^\circ C$[cite: 153, 157].
*   **Circuit Design & Amplification:** Designed signal conditioning circuits using the AD620 instrumentation amplifier to scale the millivolt outputs of the RTD (Gain = 10) and Thermocouple (Gain = 500) into readable 0-5V ADC ranges[cite: 154, 157, 158]. 
*   **STM32 Integration:** Programmed the STM32 to sample the ADC, reverse-calculate the physical temperature using derived characteristic equations (e.g., the Beta equation for the NTC), and display the results on an LCD[cite: 155, 158, 159].

### Q2: Digital Temperature Sensing via I2C
*   **Protocol Selection:** Evaluated communication protocols for high-noise industrial environments, highlighting CAN for long distances and I2C for local digital sensor interfacing[cite: 161].
*   **LM75 Interfacing:** Configured the STM32 I2C peripheral to read 16-bit temperature registers from an LM75 digital sensor. Extracted the 9 most significant bits and scaled by 0.5$^\circ$C to display accurate temperature readings on an LCD in Proteus[cite: 161, 162].

### Q3: Complete Digital Scale Design (Load Cell Simulation)
*   **Signal Conditioning:** Configured an AD620 (Gain = 500) to amplify a simulated load cell Wheatstone bridge, mapping a 0-2kg mass directly to a 0-5V output[cite: 163, 164].
*   **Vibration Simulation & Filtering:** Modeled the mechanical ringing of a dropped weight using an underdamped RLC circuit. Designed an RC Low-Pass Filter ($f_c \approx 5$Hz) to smooth the transient oscillations and provide a stable DC voltage to the ADC[cite: 165, 166, 167].
*   **Embedded Software (Tare Function):** Programmed the STM32 to continuously read the filtered voltage, convert it to mass, and implemented a digital "Tare" feature using an external push button to zero the scale dynamically and display Net Weight on the LCD[cite: 169].
*   **Filter Optimization Analysis:** Evaluated the settling time ($\approx 4.5$ seconds) and discussed methods for improvement, such as using higher-order active Butterworth filters, time-varying filters, or implementing digital Moving Average (FIR/IIR) filters in software to achieve faster stabilization without sacrificing noise rejection[cite: 170, 171].

## Files Included
*   **Documentation:** `HW4_810102468.pdf` and `Inst_HW4.pdf` (Persian reports and assignment instructions)[cite: 151, 171].
*   **Source Code:** `main.c` featuring the HAL-based STM32 logic for ADC scaling, I2C register reading, and the digital Tare logic[cite: 161].
*   **Simulation Files:** Proteus design files integrating the STM32, AD620, LCDs, LM75, and simulated load cells[cite: 156, 158, 160, 162, 169].
