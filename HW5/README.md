# Instrumentation - Homework 5

## Overview
This folder contains the solutions, embedded C code, and Proteus simulations for Homework 5 of the Instrumentation course. The project focuses on integrating STM32 microcontrollers with mechanical actuators and advanced sensors. It covers DC motor speed/direction control using H-bridges, precise stepper motor positioning for a solar tracker, ultrasonic and IR obstacle detection, and rotational speed/angle measurement using potentiometers and rotary encoders[cite: 179, 199, 201].

## Topics Covered

### Q1: DC Motor Control (L293D Driver & PWM)
*   **Hardware Interface:** Addressed the current and voltage limitations of direct microcontroller-to-motor connections by implementing the L293D motor driver, which provides H-bridge logic and flyback diode protection[cite: 179].
*   **STM32 Configuration:** Utilized hardware timers (TIM1, Channel 1) to generate a dynamic PWM signal to the L293D `EN1` pin to control motor speed[cite: 180, 181].
*   **Software Design:** Implemented software debouncing (50ms) for push buttons. Mapped button presses to increment/decrement PWM duty cycle (0-100%) and toggle direction pins (`IN1`, `IN2`). Displayed real-time status (Duty % and CW/CCW direction) on a 16x2 LCD[cite: 180, 181, 182, 183].

### Q2: Solar Tracker Simulation (Stepper Motor)
*   **Actuator Selection:** Analyzed the advantages of stepper motors over DC motors for solar tracking, specifically their open-loop precision and holding torque, which maintains panel position against wind loads without mechanical brakes[cite: 184, 200].
*   **Manual & Auto Tracking:** Programmed a ULN2003A driver using a Full-Step sequence array. Implemented a manual mode (push-button step control) and an auto mode utilizing `HAL_GetTick()` to advance the motor 1.8$^\circ$ every 2 seconds until reaching 180$^\circ$[cite: 184, 185, 186, 200]. 
*   **Reset Logic:** Added a conditional reset function that only triggers when the tracker reaches 180$^\circ$, automatically driving the motor continuously back to 0$^\circ$[cite: 185, 200].

### Q3: Ultrasonic and IR Proximity Sensors
*   **Ultrasonic (HC-SR04):** Configured a 1MHz hardware timer to measure the Time of Flight (ToF) of ultrasonic pulses. Divided the microsecond echo time by 58 to calculate and display the obstacle distance in centimeters dynamically via a simulated potentiometer[cite: 188, 189, 201].
*   **Infrared (IR) Sensor:** Interfaced an active-low IR obstacle sensor. Programmed GPIO read logic to display "Object Detected" or "Path Clear" on the LCD in real-time, functioning as a basic collision monitoring system[cite: 189, 190].

### Q4: Position and Speed Measurement Systems
*   **Potentiometer Angle Measurement:** Scaled the 12-bit ADC output (0-4095) to simulate an angular range of 0-270$^\circ$ with a resolution of 0.0659$^\circ$. Activated a warning LED when the angle exceeded 180$^\circ$[cite: 191, 201].
*   **Optocounter Simulation:** Used a Pulse Generator and EXTI (Rising Edge) interrupts to simulate an optical encoder disc with 28 slots. Calculated motor RPM based on pulse counts over a 1-second timer window, triggering a flashing LED alarm above 500 RPM[cite: 191, 192, 194].
*   **Quadrature Encoder (4X Mode):** Implemented a high-precision rotary encoder reading using both Channel A and Channel B on dual-edge EXTI interrupts. Resolved rotation direction and enhanced angular resolution to $\approx 0.3125^\circ$ using the 4X decoding algorithm, updating RPM and shaft angle in real-time on the LCD[cite: 192, 193, 198, 202].

## Files Included
*   **Documentation:** `HW5_810102468.pdf` and `Inst_HW5.pdf` (Persian reports and assignment instructions)[cite: 179, 199].
*   **Simulation Files:** Proteus `.pdsprj` files for DC motor control, stepper tracking, and encoder integration[cite: 182, 187, 190, 198].
*   **Source Code:** STM32 `main.c` files for GPIO toggling, Timer interrupts, EXTI decoding, and LCD printing.
