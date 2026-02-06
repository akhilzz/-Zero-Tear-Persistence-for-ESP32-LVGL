# Zero-Tear Persistence for ESP32 & LVGL

This repository contains a minimal, robust solution for saving data to NVS on the ESP32-S3 without causing screen tearing or flickering on RGB LCDs.

## The Problem
Writing to Flash memory on the ESP32-S3 often causes high-current spikes and bus contention that interrupts DMA transfers to the display, resulting in visible "tearing" or horizontal glitches.

## The Solution: Bus Arbitration Architecture
1. **Asynchronous Task**: Data is buffered in RAM and saved in the background on Core 0.
2. **Debouncing**: Changes are only committed after 3 seconds of idle time.
3. **Bus Locking**: Before writing to flash, we call `lvgl_port_stop_render()` to silence the SPI/DMA bus.
4. **Explicit Commit**: We use `preferences.end()` to ensure data is physically flushed to silicon.

## Files
- `ProfileManager.cpp/h`: The core logic for safe persistence.
- `AppTypes.h`: Data structures for your application.
- `minimal_persistence_example.ino`: Demo showing how to trigger saves.
- `esp_panel_board_custom_conf.h`: RGB Display and Touch configuration for Waveshare 4.3".
- `lv_conf.h`: Optimized LVGL memory and feature settings.

## Hardware Configuration
This example is pre-configured for the **Waveshare ESP32-S3-Touch-LCD-4.3**. It uses:
- **RGB565** 16-bit color depth.
- **GT911** I2C Touch controller.
- **CH422G** IO Expander for backlight and reset control.
- **Bounce Buffers**: Enabled to prevent screen drifting an tearing during high-speed DMA.

## How to use
1. Copy `ProfileManager` and `AppTypes` into your project.
2. In your display port, implement `lvgl_port_stop_render` and `lvgl_port_resume_render`.
3. Call `Profiles.saveProfiles(data, count)` in your UI.
4. Data will persist in the background without a single flicker!
"# -Zero-Tear-Persistence-for-ESP32-LVGL" 
