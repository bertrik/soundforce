void freq_calc_osc(byte osc_number) {

  if (OSC[osc_number].period > 44000000) {
    OSC[osc_number].samples_per_cycle = (sampling_frequency) * (OSC[osc_number].period / 1000);
    //avoid_overflow = 1;
    // digitalWrite(PC13, HIGH);
  }

  else {
    OSC[osc_number].samples_per_cycle = (sampling_frequency * OSC[osc_number].period) / 1000;
    //avoid_overflow = 0;
    // digitalWrite(PC13, LOW);
  }

  if (pinged) {
    if (OSC[osc_number].multiplying) {
      OSC[osc_number].samples_per_cycle = OSC[osc_number].samples_per_cycle >> (OSC[osc_number].divider - 1);
    }

    else {
      OSC[osc_number].samples_per_cycle = OSC[osc_number].samples_per_cycle << (OSC[osc_number].divider - 1);
    }
  }

  //safety, if the sample couter is already bigger than the max amout of samples
  if (OSC[osc_number].sample_counter > OSC[osc_number].samples_per_cycle) {
    OSC[osc_number].sample_counter = 0;
  }

  //OSC[osc_number].width_stepped_triangle = OSC[osc_number].samples_per_cycle >> 3 ;
  OSC[osc_number].pulse_width_in_cycles_50 = OSC[osc_number].samples_per_cycle >> 1 ;
  OSC[osc_number].pulse_width_in_cycles_25 =  OSC[osc_number].pulse_width_in_cycles_50 >> 1;
  OSC[osc_number].width_stepped_triangle =  OSC[osc_number].pulse_width_in_cycles_25 >> 1;
}

//if array for freq conversion is only 1 octave
// -1.5 to 5.5 is 7 octaves, 4095/7 = 585
// we need an array for 585 relevant values

// then we take the ADC input, divide it by 585 as ints, results is the octave number
// ex: ADC = 1000, 1000/585 = 1, octave 1
// 1000 % 585 = 415, we get the value 415 from the conversion array and mutltiply it by 2 (octave 1)
