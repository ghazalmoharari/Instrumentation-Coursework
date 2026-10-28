# Instrumentation - Homework 2

## Overview
This folder contains the solutions, circuit simulations, and MATLAB implementations for Homework 2 of the Instrumentation course. The project focuses on analog signal conditioning, active and passive filter design, analog-to-digital (ADC) and digital-to-analog (DAC) conversion, and the practical application of instrumentation amplifiers[cite: 77, 78, 81].

## Topics Covered

### Q1: Analog Signal Conditioning & Filters
*   **Filter Design:** Designing passive RC high-pass ($f_c = 20$ Hz) and low-pass ($f_c = 200$ Hz) filters to isolate useful vibration sensor data[cite: 77, 83]. 
*   **Bandpass & Notch Filters:** Combining filters to create a bandpass response and calculating its center frequency ($63.25$ Hz), bandwidth, and Q-factor[cite: 77, 83, 84]. Simulating an LC Notch filter ($f_c = 50$ Hz) to reject powerline interference[cite: 78, 86].
*   **Simulink Analysis:** Generating Bode plots to verify the frequency response and attenuation characteristics of the designed filters[cite: 78, 85].

### Q2: NTC Thermistor & ADC Interfacing
*   **Temperature Sensing:** Utilizing an NTC thermistor (negative temperature coefficient) within a voltage divider to generate temperature-dependent analog voltages[cite: 78, 87].
*   **Voltage Comparators:** Designing a Proteus circuit using LM358 op-amps and digital logic gates (XOR, AND) to function as a basic ADC[cite: 78, 88].
*   **Threshold Logic:** Configuring the circuit to trigger specific LED indicators (Red, Blue, or both) as the sensor voltage crosses 1.25V, 2.5V, and 3.75V thresholds[cite: 78, 89].

### Q3: 4-Bit R-2R Digital-to-Analog Converter (DAC)
*   **Theoretical Calculation:** Determining the output voltage of a 4-bit R-2R ladder for specific binary inputs (e.g., 1011, 1110) utilizing a 4.8V reference[cite: 79, 90].
*   **Proteus Implementation:** Building the DAC using CD4053 analog multiplexers and comparing theoretical outputs with simulation results, accounting for internal switch resistance[cite: 90, 91].
*   **Waveform Generation:** Driving the DAC with an up/down digital counter (4029) and a D flip-flop (4013) to synthesize stepped triangle waveforms[cite: 79, 94, 95].

### Q4: Instrumentation Amplifiers & Wheatstone Bridges
*   **Biomedical Signal Amplification:** Designing a multi-stage conditioning circuit to amplify a 500$\mu$V ECG signal to a 0-5V ADC range, including a differential buffer and active filtering[cite: 80, 96, 97].
*   **AD620 Configuration:** Calculating the gain resistor ($R_g = 504\Omega$) required to achieve a gain of 100 for an AD620 instrumentation amplifier[cite: 81, 103].
*   **Wheatstone Bridge Advantages:** Analyzing the use of a Wheatstone bridge for a strain-gauge pressure sensor to eliminate common-mode noise and compensate for temperature drift[cite: 81, 102].
