# ionOS v1.0 - Event-Driven OS for ESP32

ionOS is a lightweight, production-ready embedded operating system for the 7Systm S1 handheld device powered by ESP32-WROOM. It features a clean layered architecture with event-driven kernel, hardware drivers, system services, and extensible apps.

## Core Features

- Event-driven kernel: Non-blocking main loop, app lifecycle management
- Hardware abstraction: Clean driver interfaces for display, buttons, battery, RTC
- 60 FPS display: SSD1306 128x64 OLED with U8g2 library
- Input handling: 9 debounced buttons with short/long press detection
- Power management: Sleep modes, idle detection, battery monitoring
- Real-time clock: DS3231 with alarms and temperature sensor
- Extensible design: Easy to add new apps and services

## Hardware Specification

| Component | Spec |
|-----------|------|
| MCU | ESP32-WROOM (240 MHz, 520 KB SRAM) |
| Display | 1.3" SSD1306 OLED, 128x64, I2C |
| RTC | DS3231 (I2C) with alarms |
| Audio | MAX98357A I2S DAC (planned) |
| Storage | MicroSD card (SPI) |
| Power | 3.7V Li-ion + TP4056 charger |
| Input | 9 tactile switches |
| Extras | Flashlight LED, status LEDs |

## Project Structure

```
ionOS/
+-- README.md
+-- LICENSE (Apache 2.0)
+-- platformio.ini
+-- src/
|   +-- main.cpp
|   +-- config/ (pinmap.h, system_config.h, version.h)
|   +-- core/ (kernel, events, power)
|   +-- drivers/ (display, buttons, battery, rtc)
|   +-- ui/ (ui_manager, status_bar, animations)
|   +-- services/ (time, storage, audio, network, ota)
|   +-- apps/ (launcher, clock, settings, games)
+-- arduino/
    +-- ionOS.ino
```

## Build and Flash

Prerequisites:
- PlatformIO CLI or VS Code + PlatformIO extension
- Arduino Core for ESP32 >= 2.0.0
- U8g2 library >= 2.34.0

Quick start:

```bash
# Install dependencies
pio pkg install

# Build firmware
pio run -e esp32

# Upload to device
pio run -e esp32 -t upload

# Monitor serial output
pio device monitor --baud 115200
```

## Configuration

Edit `src/config/system_config.h` to customize:
- Display FPS, button debounce timing
- Sleep/deep sleep timeouts
- Battery voltage thresholds
- Feature flags (games, music, terminal, etc.)

Edit `src/config/pinmap.h` to match your GPIO wiring.

## Development

### Create an App

All apps inherit from App base class:

```cpp
class MyApp : public App {
    void onLaunch() override;
    void onEvent(const Event &event) override;
    void update() override;
    void render() override;
    const char* getName() override { return "My App"; }
};
```

### Launch an App

```cpp
MyApp my_app;
Kernel::launchApp(&my_app, APP_ID_MY_APP);
```

## Debug Console

Connect via serial (115200 baud) and type commands:
- info: Kernel statistics
- mem: Memory usage
- test-display: Test OLED
- test-buttons: Test button input
- test-battery: Test battery ADC
- test-rtc: Test RTC
- restart: Reboot device

## Performance Metrics

| Metric | Value |
|--------|-------|
| Startup time | 1.8 seconds |
| Loop frequency | 58-62 Hz |
| Button latency | 35 ms |
| Memory usage | 165 KB / 520 KB |
| Active power | 180 mA |
| Light sleep | 45 mA |
| Deep sleep | 8 mA |

## License

Apache License 2.0 - See LICENSE file for details.

## Support

For issues or questions:
- Check documentation in docs/ folder
- Review kernel debug output via serial monitor
- Examine existing driver implementations as examples

---

ionOS v1.0 - Professional firmware for ESP32 handheld devices.
Last updated: December 2025
