# Instrumentation - Homework 3

## Overview
This folder contains the solutions, C source code, and Proteus simulations for Homework 3 of the Instrumentation course. The project focuses on programming STM32 ARM microcontrollers (specifically the STM32F103C6) using STM32CubeIDE. It covers foundational embedded systems concepts including GPIO configuration, External Interrupts (EXTI), hardware Timers for PWM and precise delays, Analog-to-Digital Conversion (ADC), and 16x2 LCD interfacing[cite: 128, 140, 144].

## Topics Covered

### Q1: 7-Segment Display & External Interrupts
*   **Counter Implementation:** Configured standard GPIO pins to drive a common-cathode 7-segment display, cycling continuously from 0 to 9 when initiated by a push button[cite: 140, 146].
*   **EXTI Handlers:** Programmed two external interrupt pins to instantly halt the main loop counter and force the display to show '1' or '9', preventing a return to the counting state[cite: 140, 147].

### Q2: DC Motor Control (PWM & L298 Driver)
*   **Timer & PWM:** Configured the STM32 TIM unit (Timer 1) to generate a 1kHz PWM signal by calculating the appropriate Prescaler (PSC) and Auto-Reload Register (ARR) values[cite: 119, 141]. 
*   **Speed & Direction Control:** Mapped 12-bit ADC readings from a potentiometer to adjust the PWM duty cycle for speed control. Interfaced with an L298 motor driver, using a push button to reverse the motor's rotational direction[cite: 120, 141].

### Q3: Interrupt-Driven Countdown Timer
*   **Precise Timing:** Configured Timer 3 to trigger an interrupt exactly every 1 second without blocking the main CPU execution[cite: 123]. 
*   **System Logic:** Implemented a 15-second countdown triggered by a logic state change. During the countdown, an LED and buzzer toggle every second, transitioning to a solid ON state at zero. The remaining time and system status are actively updated on a 16x2 LCD[cite: 123, 141, 142].

### Q4: Smart Sensor Identification System
*   **ADC Interfacing:** Configured a 12-bit ADC with a 3.3V reference to achieve an 8mV resolution for accurate analog voltage readings[cite: 124, 125, 145].
*   **Dynamic Identification:** Programmed conditional logic to identify simulated sensors based on voltage thresholds (LM35 Temperature, HIH4000 Humidity, MPX4115 Pressure, MQ-135 Air Quality)[cite: 127, 145]. 
*   **Data Conversion:** Converted raw ADC voltages into their corresponding physical units and formatted the output for an alphanumeric LCD[cite: 127, 145].

## Files Included
*   **Documentation:** `HW3.pdf` and `Inst_HW3.pdf` (Persian reports and assignment instructions)[cite: 111, 128].
*   **Source Code:** `main.c` featuring the HAL-based STM32 logic for GPIO, ADC polling, and EXTI callbacks[cite: 146, 147].
*   **Simulation Files:** Proteus design files (`q4_4.pdsprj`, `q4_7.pdsprj`, etc.) simulating the STM32 interacting with displays, motors, and sensors[cite: 109, 110, 145].
