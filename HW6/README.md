# Instrumentation - Homework 6

## Overview
This folder contains the solutions, embedded C code, and Proteus simulations for Homework 6 of the Instrumentation course. The project focuses on integrating a wide array of environmental and industrial sensors with STM32 microcontrollers. It covers advanced UI state machines, feedback control systems, I2C/UART communication, and real-time data processing[cite: 204, 210].

## Topics Covered

### Q1: Gas Detection (MQ-4)
*   **Sensor Interfacing:** Simulated an MQ-4 semiconductor gas sensor for Methane (CH4) and CNG detection[cite: 210].
*   **Threshold Logic:** Configured the STM32 ADC to read the sensor's analog voltage and trigger an LED warning when the gas concentration exceeds a predefined safety threshold[cite: 210, 211].

### Q2: Environmental Control & UI State Machine (DHT11)
*   **State Machine Design:** Built a multi-menu user interface using an LCD and three push buttons (Menu, Up, Down). The system transitions between a default monitoring mode, a Temperature Set-point mode, and a Humidity Set-point mode[cite: 204, 213].
*   **Feedback Control:** Programmed the STM32 to control three actuators based on DHT11 readings: a Heater (Red LED) if temp < target, a Humidifier (Blue LED) if humidity < target, and an Exhaust Fan (DC Motor via PWM) scaling proportionally with excess heat[cite: 205, 213].

### Q3: Light Sensing & Dual-Output Processing (LDR)
*   **Analog & Digital Processing:** Interfaced an LDR module utilizing both its Analog (A0) and Digital (D0) outputs[cite: 205, 214].
*   **Comparator Tuning:** Analyzed the effect of adjusting the on-board comparator threshold (SRV) on the D0 output, dynamically triggering an LED and updating the LCD based on ambient light levels[cite: 215, 216].

### Q4: Water Flow Measurement (Pulse Counting)
*   **Frequency Calculation:** Simulated a turbine-based water flow sensor using a square wave pulse generator. Used STM32 External Interrupts (EXTI) to count rising edges over a 1-second interval to calculate frequency[cite: 206, 218, 219].
*   **Flow Rate Conversion:** Converted the measured frequency into liters per minute ($Q = f / 7.5$) and verified the system response at 15Hz, 30Hz, and 60Hz[cite: 206, 219, 221].

### Q5: Absolute Pressure Sensing (MPX4115)
*   **Characteristic Equation:** Derived the transfer function for the MPX4115 sensor ($P = (V_{out}/V_s + 0.095)/0.009$) and mapped the 12-bit ADC readings directly to absolute pressure in kilopascals (kPa)[cite: 207, 225, 226].

### Q6: Industrial Tank Level Control
*   **Level Monitoring:** Simulated a fluid tank using a potentiometer. Mapped the ADC input to a 0-100% capacity range[cite: 207, 228].
*   **Pump Automation:** Programmed logic to activate a DC motor pump and a Red alert LED when the tank drops below 25%, switching to a Green normal LED and deactivating the pump once optimal levels are reached[cite: 228, 229].

### Q7: Barometric Pressure, Temp, & UART Serial Monitor (BMP180)
*   **I2C & UART Integration:** Interfaced the BMP180 sensor via hardware I2C to read temperature and pressure. Established a UART connection (Baud Rate: 115200) to stream formatted data to a Virtual Terminal[cite: 208, 230].
*   **Altitude Estimation & Alarms:** Calculated estimated altitude from barometric pressure and programmed a high-temperature warning string (`WARNING`) triggered above 40$^\circ$C[cite: 208, 230].
