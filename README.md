# Alt firmware for SoundForce STM32DUINO LFO DIY Eurorack Module

Firmware for SoundForce's DIY EuroRack STM32DUINO LFO Module (Dual trigger/gate to CV)

## Improvements and Additions
Added new additional noise types which are selectable from the waveform potentiometer control:

* Pink noise
* Random walk noise
* Noise with sample and hold

## Enhancements
Added a rolling average filter to the oscillators to help reduce steps between samples.

## Compilation
Inherits from the [bertrik/soundforce](https://github.com/bertrik/soundforce) branch that added files required to compile the firmware from PlatformIO

# Reference
Firmwares for http://sound-force.nl/?page_id=2212 updated for use with platformio
