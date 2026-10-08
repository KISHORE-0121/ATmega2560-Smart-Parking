# ATmega2560 Smart Parking Assist & Slot Monitoring System

## Project Overview

The **ATmega2560 Smart Parking System** is a bare-metal embedded application designed to assist parking slot occupancy detection and monitoring. The system uses infrared (IR) and ultrasonic sensors to detect available parking slots and provide real-time feedback through LEDs, displays, and sensor data.

This project addresses the common problem of locating available parking slots by automating slot occupancy detection and status visualization. By leveraging the ATmega2560 microcontroller's capabilities, the system can monitor multiple parking slots and provide immediate feedback to assist vehicle entry and parking decisions.

### Problem Statement
Finding available parking slots in congested areas is time-consuming and frustrating for drivers. This system automates the process by:
- Detecting slot occupancy using IR sensors
- Measuring distance to obstacles using ultrasonic sensors
- Displaying real-time slot availability
- Assisting drivers in quick decision-making

---

## Objectives

1. **Detect Parking Slot Occupancy** – Use IR sensors to determine if a slot is occupied or available
2. **Monitor Available Slots** – Track the number of available parking slots in real-time
3. **Assist Vehicle Parking** – Guide drivers with visual and distance-based feedback
4. **Display Parking Information** – Show slot status on 7-segment displays
5. **Build Reusable Bare-Metal Drivers** – Develop register-level drivers without relying on Arduino libraries
6. **Maintain Modular Architecture** – Design software for scalability, testing, and future enhancements

---

## Key Features

### ✅ Implemented Features

| Driver | Capability | Status |
|--------|-----------|--------|
| **GPIO** | Read/write digital pins, toggle outputs | ✅ Implemented |
| **LED** | ON, OFF, Toggle with GPIO integration | ✅ Implemented |
| **Timer1** | CTC mode, millisecond delays (prescaler 64) | ✅ Implemented |
| **ADC** | 10-bit analog-to-digital conversion, multi-channel support | ✅ Implemented |
| **PWM** | Pulse-width modulation with duty cycle control | ✅ Implemented |
| **7-Segment Display** | Single/multi-digit display, number formatting | ✅ Implemented |
| **IR Sensor** | Binary occupancy detection (Available/Occupied) | ✅ Implemented |
| **Ultrasonic Sensor** | Distance measurement in centimeters | ✅ Implemented |

### 📋 Planned Features

- External Interrupt-driven sensor events
- Keypad/Menu interface
- LCD display integration
- Multi-slot management system
- Central parking display panel
- EEPROM data logging
- Wireless communication module integration

---

## Hardware Requirements

| Component | Purpose |
|-----------|---------|
| **ATmega2560** | Main microcontroller (16 MHz clock) |
| **IR Sensors** (x2-4) | Slot occupancy detection |
| **Ultrasonic Sensor** (HC-SR04) | Distance measurement for parking assistance |
| **LEDs** | Status indication (Available/Occupied) |
| **7-Segment Display** | Real-time availability count |
| **Push Buttons** | User interaction and mode selection |
| **Power Supply** | 5V regulated supply for MCU and sensors |
| **Crystal Oscillator** | 16 MHz for accurate timing |

---

## Software Architecture

The project follows a **layered driver architecture** that separates application logic from hardware-specific operations:

```
┌─────────────────────────────────┐
│   Application Layer             │
│  (Parking logic, main.c)        │
└────────────┬────────────────────┘
             ↓
┌─────────────────────────────────┐
│   Driver API Layer              │
│  (gpio, led, timer, adc,        │
│   pwm, ir, ultrasonic, seg7)    │
└────────────┬────────────────────┘
             ↓
┌─────────────────────────────────┐
│  ATmega2560 Memory-Mapped       │
│  Registers                      │
│  (DDR, PORT, PIN, TCCR, etc.)   │
└────────────┬────────────────────┘
             ↓
┌─────────────────────────────────┐
│   Hardware                      │
│  (Sensors, LEDs, Display)       │
└─────────────────────────────────┘
```

**Key Design Principles:**
- **Abstraction**: Applications use driver APIs; drivers handle register manipulation
- **Modularity**: Each driver is self-contained with minimal dependencies
- **Reusability**: Drivers are designed for use across multiple projects
- **No Arduino Overhead**: Pure register-level programming for efficiency

---

## Repository Structure

```
ATmega2560-Smart-Parking/
│
├── README.md                          # Project documentation
│
└── DRIVERS/
    └── DAY 1/
        ├── GPIO/                      # GPIO driver (low-level I/O)
        │   ├── gpio.h                 # Port register macros, function declarations
        │   └── gpio.cpp               # GPIO implementation (SetIO, Read, Write, Toggle)
        │
        ├── LED + SWITCH/              # LED driver (uses GPIO)
        │   ├── led_driver.H            # LED API declarations
        │   └── led_driver.CPP          # LED implementation (Init, On, Off, Toggle)
        │
        ├── TIMER/                     # Timer1 driver (delays, interrupts)
        │   ├── timer.h                # Timer function declarations
        │   └── timer.cpp              # Timer1 CTC mode, delay_ms implementation
        │
        ├── ADC/                       # Analog-to-Digital Converter
        │   ├── adc.h                  # ADC API (Init, Read)
        │   └── adc.c                  # ADC multi-channel implementation
        │
        ├── PWM/                       # Pulse-Width Modulation
        │   ├── pwm.h                  # PWM API (Init, SetDuty, Start, Stop)
        │   └── pwm.c                  # PWM implementation
        │
        ├── 7 SEGMENT/                 # 7-Segment Display driver
        │   ├── seg7.h                 # 7-Segment API (Init, DisplayDigit, DisplayNumber, Clear)
        │   └── seg7.c                 # 7-Segment display logic
        │
        ├── IR/                        # Infrared Sensor driver (occupancy)
        │   ├── ir.h                   # IR API (Init, Read)
        │   └── ir.c                   # IR occupancy detection
        │
        └── ULTRASONIC SENSOR/         # Ultrasonic distance sensor
            ├── ultra.h                # Ultrasonic API (GetDistanceCm)
            └── ultra.c                # HC-SR04 distance measurement

```

---

## Driver Architecture Details

### Header Files (.h)
- **Purpose**: Define API contracts, register macros, and type definitions
- **Content**: Function declarations, `#define` macros for registers and pin masks
- **Usage**: Include in application or other drivers

### Implementation Files (.c / .cpp)
- **Purpose**: Register-level implementations of driver APIs
- **Content**: Direct manipulation of memory-mapped registers (DDRA, PORTA, PINC, etc.)
- **Approach**: No indirect abstractions; efficiency through direct hardware access

### Driver Interdependencies
- **LED Driver** depends on **GPIO Driver** (uses GPIO_SetIO, GPIO_Write, GPIO_Toggle)
- **Timer Driver** uses **Timer1 registers** directly
- **Sensor Drivers** (IR, Ultrasonic) use **GPIO** for pin control
- **7-Segment Driver** uses **GPIO** for segment control
- **ADC Driver** operates independently on analog channels

---

## GPIO Driver

The GPIO driver provides the lowest-level hardware abstraction and is the foundation for all other drivers.

### Purpose
- Configure I/O pins as input or output
- Read digital input states
- Write digital output values
- Toggle pin states

### API Functions

```c
void GPIO_SetIO(volatile uint8_t *DDR, uint8_t pin, uint8_t mode);
// Configures pin as INPUT (0) or OUTPUT (1)

void GPIO_Write(volatile uint8_t *PORT, uint8_t pin, uint8_t value);
// Writes HIGH (1) or LOW (0) to a pin

uint8_t GPIO_Read(volatile uint8_t *PIN, uint8_t pin);
// Reads the current state of a pin (0 or 1)

void GPIO_Toggle(volatile uint8_t *PORT, uint8_t pin);
// Toggles a pin state (HIGH ↔ LOW)
```

### Supported Ports
Ports A–L with memory-mapped registers:
- **DDRA–DDRL**: Data Direction Registers (0=input, 1=output)
- **PORTA–PORTL**: Output registers
- **PINA–PINL**: Input registers (read-only)

### Example Usage
```c
// Set PB5 as output
GPIO_SetIO(&DDRB, 5, OUTPUT);

// Write HIGH to PB5
GPIO_Write(&PORTB, 5, HIGH);

// Read PC3
uint8_t state = GPIO_Read(&PINC, 3);

// Toggle PD7
GPIO_Toggle(&PORTD, 7);
```

---

## LED Driver

The LED driver builds on GPIO and provides high-level LED control.

### Purpose
- Initialize LEDs on a specified pin
- Control LED ON/OFF states
- Toggle LEDs for blinking effects

### API Functions

```c
void LED_Init(volatile uint8_t *DDR, uint8_t pin);
// Initializes an LED pin as output

void LED_On(volatile uint8_t *PORT, uint8_t pin);
// Turns LED ON (HIGH)

void LED_Off(volatile uint8_t *PORT, uint8_t pin);
// Turns LED OFF (LOW)

void LED_Toggle(volatile uint8_t *PORT, uint8_t pin);
// Toggles LED state
```

### Implementation Detail
LED functions call corresponding GPIO functions, demonstrating driver composition.

### Example Usage
```c
// Initialize LED on PB5
LED_Init(&DDRB, 5);

// Turn ON
LED_On(&PORTB, 5);

// Turn OFF
LED_Off(&PORTB, 5);

// Toggle (blink effect)
LED_Toggle(&PORTB, 5);
```

---

## Timer Driver

The Timer1 driver provides precision timing using CTC (Clear Timer on Compare) mode.

### Purpose
- Generate millisecond-accurate delays
- Serve as foundation for scheduled events
- Support periodic interrupts

### Configuration
- **Mode**: CTC (Clear Timer on Compare)
- **Prescaler**: 64
- **Clock**: 16 MHz / 64 = 250 kHz
- **Compare Value**: OCR1A = 249 (generates 1 ms interrupt)

### API Functions

```c
void TIMER_Init(void);
// Initializes Timer1 in CTC mode with 1 ms resolution

void TIMER_Delay_ms(uint16_t ms);
// Blocks for specified milliseconds
```

### Timing Calculation
```
16 MHz / 64 = 250 kHz
1 ms = 250 clock cycles
OCR1A = 249 (0-indexed counting)
```

### Example Usage
```c
// Initialize timer
TIMER_Init();

// Wait 100 milliseconds
TIMER_Delay_ms(100);

// Wait 1 second
TIMER_Delay_ms(1000);
```

---

## ADC Driver

The ADC driver enables analog-to-digital conversion for sensor reading.

### Purpose
- Initialize ADC with register configuration
- Read 10-bit values from analog channels
- Support multi-channel conversion

### API Functions

```c
void ADC_Init(void);
// Initializes ADC module with default settings

uint16_t ADC_Read(uint8_t channel);
// Reads 10-bit ADC value from specified channel (0-15)
```

### Supported Channels
Channels 0–15 on ADC ports (F0–F7, K0–K7, etc.)

### Example Usage
```c
// Initialize ADC
ADC_Init();

// Read from channel 0
uint16_t value = ADC_Read(0);  // Returns 0–1023
```

---

## PWM Driver

The PWM driver enables pulse-width modulation for speed/brightness control.

### Purpose
- Generate PWM signals on specified timer output
- Control duty cycle (0–100%)
- Support motor or LED brightness control

### API Functions

```c
void PWM_Init(void);
// Initializes PWM on Timer1 or equivalent

void PWM_SetDuty(uint8_t duty);
// Sets duty cycle: 0 (0%) to 100 (100%)

void PWM_Start(void);
// Starts PWM generation

void PWM_Stop(void);
// Stops PWM generation
```

### Example Usage
```c
// Initialize PWM
PWM_Init();

// Set 50% duty cycle
PWM_SetDuty(50);

// Start generation
PWM_Start();

// Later: Stop PWM
PWM_Stop();
```

---

## 7-Segment Display Driver

The 7-Segment driver enables numeric display output.

### Purpose
- Initialize 7-segment display pins
- Display single digits (0–9)
- Display multi-digit numbers (0–99)
- Clear display

### API Functions

```c
void SEG7_Init(void);
// Initializes all 7-segment pins as outputs

void SEG7_DisplayDigit(uint8_t digit);
// Displays single digit: 0–9

void SEG7_DisplayNumber(uint8_t number);
// Displays two-digit number: 0–99

void SEG7_Clear(void);
// Clears the display
```

### Example Usage
```c
// Initialize display
SEG7_Init();

// Show single digit
SEG7_DisplayDigit(5);        // Displays "5"

// Show two-digit number
SEG7_DisplayNumber(42);      // Displays "42"

// Clear
SEG7_Clear();
```

---

## Sensor Drivers

### IR Sensor Driver

**Purpose**: Detect parking slot occupancy using infrared sensors.

**API Functions**:
```c
void IR_Init(void);
// Initializes IR sensor GPIO pins

uint8_t IR_Read(void);
// Returns IR_AVAILABLE (0) or IR_OCCUPIED (1)
```

**Operating Principle**:
- IR sensor outputs LOW when an object is detected (occupied)
- IR sensor outputs HIGH when no object is present (available)

**Example Usage**:
```c
IR_Init();
uint8_t status = IR_Read();
if (status == IR_OCCUPIED) {
    LED_On(&PORTB, 5);  // Turn on red LED
} else {
    LED_Off(&PORTB, 5);
}
```

---

### Ultrasonic Sensor Driver

**Purpose**: Measure distance to objects for parking assistance.

**API Function**:
```c
unsigned int ULTRA_GetDistanceCm(void);
// Returns distance in centimeters
```

**Operating Principle**:
- HC-SR04 sensor triggers a pulse on trigger pin
- Waits for echo pulse response on echo pin
- Calculates distance: Distance = (Echo Time × Speed of Sound) / 2
- Uses Timer1 for precise pulse measurement

**Measurement Range**: ~2 cm to ~400 cm

**Example Usage**:
```c
unsigned int distance = ULTRA_GetDistanceCm();
if (distance < 30) {
    // Vehicle too close to obstacle
    LED_On(&PORTB, 6);  // Turn on warning LED
}
```

---

## Build and Upload Instructions

### Prerequisites
- **Arduino IDE** 1.8.x or later
- **ATmega2560 Board Support** installed
- **USB-to-Serial Cable** (CH340 or FTDI)
- **AVR Toolchain** (included with Arduino IDE)

### Step 1: Setup Arduino IDE
1. Open **Arduino IDE**
2. Go to **Tools → Board → Arduino Mega or Mega 2560**
3. Select **Processor: ATmega2560**
4. Go to **Tools → Port** and select the COM port (e.g., COM3)

### Step 2: Prepare Project
1. Copy all files from `DRIVERS/DAY 1/` to your sketch folder or create a separate project
2. Create a `main.cpp` or `main.ino` that includes the driver headers:
   ```cpp
   #include "gpio.h"
   #include "led_driver.h"
   #include "timer.h"
   // ... other driver includes
   
   void setup() {
       GPIO_SetIO(&DDRB, 5, OUTPUT);
       LED_Init(&DDRB, 5);
       TIMER_Init();
   }
   
   void loop() {
       LED_On(&PORTB, 5);
       TIMER_Delay_ms(500);
       LED_Off(&PORTB, 5);
       TIMER_Delay_ms(500);
   }
   ```

### Step 3: Compile
1. Click **Verify** (checkmark icon)
2. Fix any compilation errors

### Step 4: Upload
1. Connect USB cable to Arduino Mega 2560
2. Click **Upload** (arrow icon)
3. Wait for "Upload Complete" message

### Troubleshooting
| Issue | Solution |
|-------|----------|
| Port not appearing | Check USB cable, install CH340 drivers if needed |
| Compilation errors | Verify file paths; ensure all headers are in scope |
| Upload fails | Select correct board (Mega 2560) and COM port |
| Code runs but no effect | Verify pin connections; check LED/sensor polarity |

---

## Testing

| Module | Test Case | Expected Result | Status |
|--------|-----------|-----------------|--------|
| GPIO | Set PB5 as output, write HIGH | Pin goes HIGH | ✅ Pass |
| LED | LED_On() on PB5 | LED illuminates | ✅ Pass |
| Timer | TIMER_Delay_ms(1000) | 1-second delay | ✅ Pass |
| ADC | ADC_Read(0) with known voltage | Correct 10-bit value | ✅ Pass |
| PWM | PWM_SetDuty(50) | 50% duty cycle output | ✅ Pass |
| 7-Segment | SEG7_DisplayNumber(42) | Display shows "42" | ✅ Pass |
| IR Sensor | IR_Read() with object present | Returns IR_OCCUPIED (1) | ✅ Pass |
| Ultrasonic | ULTRA_GetDistanceCm() at 10cm | Returns ~10 cm | ✅ Pass |

---

## Development Progress

### ✅ Completed
- [x] GPIO driver (register-level port control)
- [x] LED driver (high-level LED control)
- [x] Timer1 driver (CTC mode, millisecond delays)
- [x] ADC driver (10-bit multi-channel conversion)
- [x] PWM driver (duty cycle modulation)
- [x] 7-Segment Display driver (digit/number display)
- [x] IR Sensor driver (occupancy detection)
- [x] Ultrasonic Sensor driver (distance measurement)

### 🔄 In Progress
- Slot management system (aggregate occupancy status)
- Application-level parking logic

### 📋 Planned
- External Interrupt handler (INT0–INT7)
- Keypad/menu interface driver
- LCD display driver (16x2 or 20x4)
- EEPROM driver (data logging)
- Wireless module support (Bluetooth/WiFi)
- Central display panel integration

---

## Future Enhancements

1. **Multi-Slot Management**
   - Aggregate data from multiple IR sensors
   - Calculate total available slots
   - Implement slot-to-slot routing

2. **Enhanced User Interface**
   - LCD display showing detailed parking info
   - Keypad for menu navigation
   - Voice feedback system

3. **Distance-Based Guidance**
   - Use ultrasonic sensor to guide vehicle
   - Provide proximity warnings
   - Automatic brake trigger (conceptual)

4. **Data Logging & Analytics**
   - Store occupancy history in EEPROM
   - Track peak hours
   - Generate usage reports

5. **Wireless Integration**
   - Bluetooth module for mobile app connectivity
   - Real-time cloud sync
   - Remote monitoring dashboard

6. **External Interrupt Events**
   - Edge-triggered sensor detection
   - Interrupt-driven system (vs. polling)
   - Reduced power consumption

7. **Modular Firmware Expansion**
   - Library-based driver distribution
   - Easy integration with other projects
   - Standardized driver API conventions

---

## Technical Concepts

### Bare-Metal Programming
Direct interaction with hardware registers without operating system abstraction; efficient, real-time capable.

### Embedded C / C++
C for high compatibility and efficiency; C++ for object-oriented designs (used in some drivers).

### Memory-Mapped I/O
Hardware registers accessible as memory addresses (e.g., `*(volatile uint8_t *)0x25` for PORTB).

### GPIO Registers
- **DDR (Data Direction)**: 0 = input, 1 = output
- **PORT (Output)**: Write HIGH or LOW
- **PIN (Input)**: Read current pin state

### Timers & CTC Mode
Compare mode: counter resets when it reaches a compare value (OCR1A), useful for precise delays.

### ADC (Analog-to-Digital Converter)
Converts analog voltage (0–5V) to digital value (0–1023 for 10-bit).

### PWM (Pulse-Width Modulation)
Rapidly switches pin HIGH/LOW to simulate analog output; duty cycle = (HIGH time / Total time) × 100%.

### Sensor Interfacing
- **IR Sensors**: Binary output (digital logic)
- **Ultrasonic**: Pulse-width encoding of distance

### Driver Abstraction
Separates hardware details from application logic; enables reuse and testing.

### Modular Firmware Architecture
Each driver is self-contained; minimal coupling between modules.

---

## Team / Contributors

| Name | Role |
|------|------|
| KISHORE-0121 | Project Lead, Lead Developer |
| TBD | Hardware Integration, Testing |
| TBD | Documentation & Wiki |

**Contributions Welcome!** Please fork, create feature branches, and submit pull requests.

---

## License

This project does not currently have a license. If you plan to use or distribute this code, please consider adding one.

**Recommended Licenses**:
- **MIT License** – Permissive, widely used in open-source projects
- **GPL v3** – Copyleft, requires derivative works to remain open-source
- **Apache 2.0** – Permissive with explicit patent protection

---

## Project Status & Disclaimer

**Status**: Under Active Development  
**Version**: 0.1 (Early Development)

### Important Notes
- This is a **hackathon/academic project** under development
- Features are implemented incrementally; not all functionality may be complete
- Use for **educational purposes** and **prototyping** primarily
- **Not recommended for production** parking systems without extensive testing and certification
- Sensor accuracy and reliability depend on environmental conditions and calibration
- Register-level code is MCU-specific; porting to other microcontrollers requires adaptation

### Known Limitations
- No interrupt-driven event handling (polling-based)
- Limited multi-tasking capability (no RTOS)
- Single-threaded execution model
- Sensor fusion and advanced signal processing not yet implemented

### Future Stability
- API and structure subject to change
- Driver documentation will be expanded
- Additional testing infrastructure planned

---

## Quick Start

1. Clone the repository:
   ```bash
   git clone https://github.com/KISHORE-0121/ATmega2560-Smart-Parking.git
   cd ATmega2560-Smart-Parking
   ```

2. Open your Arduino IDE and select **Arduino Mega 2560**

3. Copy driver files into your sketch directory

4. Create a `main.cpp` (example below):
   ```cpp
   #include "gpio.h"
   #include "led_driver.h"
   #include "timer.h"
   
   void setup() {
       TIMER_Init();
       LED_Init(&DDRB, 5);
   }
   
   void loop() {
       LED_On(&PORTB, 5);
       TIMER_Delay_ms(1000);
       LED_Off(&PORTB, 5);
       TIMER_Delay_ms(1000);
   }
   ```

5. Click **Upload** to program your Arduino Mega 2560

---

## References

- **ATmega2560 Datasheet**: [Microchip ATmega2560](https://www.microchip.com/en-us/product/ATmega2560)
- **Arduino Mega 2560 Pinout**: [Arduino Official Board Pinout](https://docs.arduino.cc/hardware/mega-2560/)
- **Bare-Metal AVR Programming**: Embedded Systems programming best practices

---

**Last Updated**: October 8, 2026  
**Repository**: https://github.com/KISHORE-0121/ATmega2560-Smart-Parking
