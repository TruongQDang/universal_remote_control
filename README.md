# Universal Remote Control
A universal remote control based on the AVR ATmega328P, capable of recording and replaying IR signals from other remotes, enabling an all-in-one solution for controlling multiple electronic devices.

## Project Structure
- `hardware/` - MCU hardware drivers, where related registers (e.g., EEPROM, GPIO) are grouped into structs for cleaner, more readable access.
- `src/` - Application logic implementing signal capture, processing, and playback.

## Signal Capture & Playback
IR signals are recorded using the Timer’s Input Capture mode, measuring the duration of each pulse and storing the sequence in an array. Playback is performed via PWM mode, reproducing the recorded pulse lengths. For persistence after power-off, captured data is stored in EEPROM.

## Build & Flash
**Toolchain installation (Ubuntu):**
```
sudo apt-get install avrdude binutils-avr avr-libc gcc-avr
```

**Compile:**
```
make all
```

**Flash to MCU:**
```
make flash
```


