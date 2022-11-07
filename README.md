# Alt firmware for SoundForce STM32DUINO LFO DIY Eurorack Module

Firmware for SoundForce's DIY EuroRack STM32DUINO LFO Module (Dual trigger/gate to CV)

For Through Hole Braids also see: [Electro-resonance/Through-hole-braided-DAC](<https://github.com/Electro-resonance/Through-hole-braided-DAC>)

## Improvements and Additions
Added new additional noise types which are selectable from the waveform potentiometer control:

* Pink noise
* Random walk noise
* Noise with sample and hold

The random walk noise is quite different from typical white noise and pink noise. At low rates it provides a meandering control voltage, so useful for emergent ambient patches.

## Enhancements
Added a rolling average filter to the oscillators to help reduce steps between samples.

## Compilation
Inherits from the [bertrik/soundforce](https://github.com/bertrik/soundforce) branch that added files required to compile the firmware from PlatformIO

# Reference
Firmwares for http://sound-force.nl/?page_id=2212 updated for use with platformio
