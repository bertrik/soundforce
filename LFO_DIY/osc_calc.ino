void osc_calc(unsigned int osc_number) {
  switch (OSC[osc_number].wave_type) {

    //STEPPED TRIANGLE
    //-----------------------//
    //----------SINE---------//
    //-----------------------//
    case 0:
      if (OSC[osc_number].samples_per_cycle < 429000) {
        OSC[osc_number].osc_out = sine_12_bits[(10000 * OSC[osc_number].sample_counter) / OSC[osc_number].samples_per_cycle];
        flag_overflow = 0;
      }

      else {
        //in some cases, the sine wave would restart from the beginning of the array too soon
        unsigned long array_position = OSC[osc_number].sample_counter / (OSC[osc_number].samples_per_cycle / 10000);
        if (array_position > 9999) {
          array_position = 9999;
        }
        OSC[osc_number].osc_out = sine_12_bits[array_position];
        flag_overflow = 1;
      }

      break;

    //---------------------//
    //-------PULSE 50------//
    //---------------------//
    case 1:
      if (OSC[osc_number].sample_counter <= OSC[osc_number].pulse_width_in_cycles_50) {
        OSC[osc_number].osc_out = 4095;
      }
      else {
        OSC[osc_number].osc_out = 0;
      }
      break;

    //---------------------//
    //-------PULSE 25------//
    //---------------------//
    case 2:
      if (OSC[osc_number].sample_counter <= OSC[osc_number].pulse_width_in_cycles_25) {
        OSC[osc_number].osc_out = 4095;
      }
      else {
        OSC[osc_number].osc_out = 0;
      }
      break;

    //-----------------------//
    //-------RISING SAW------//
    //-----------------------//
    case 3:

      if (OSC[osc_number].samples_per_cycle < 1048800) {
        OSC[osc_number].osc_out = (4095 * OSC[osc_number].sample_counter) / OSC[osc_number].samples_per_cycle;
        flag_overflow = 0;
      }

      else {
        OSC[osc_number].osc_out = (OSC[osc_number].sample_counter / (OSC[osc_number].samples_per_cycle / 4095));
        flag_overflow = 1;
      }
      break;

    //----------------------//
    //-----FALLING SAW------//
    //----------------------//
    case 4:
      if (OSC[osc_number].samples_per_cycle < 1048800) {
        OSC[osc_number].osc_out = 4095 - (4095 * OSC[osc_number].sample_counter) / OSC[osc_number].samples_per_cycle;
        flag_overflow = 0;
      }

      else {
        OSC[osc_number].osc_out = 4095 - (OSC[osc_number].sample_counter / (OSC[osc_number].samples_per_cycle / 4095));
        flag_overflow = 1;
      }
      break;

    //-----------------------//
    //-------TRIANGLE--------//
    //-----------------------//
    case 5:
      if (OSC[osc_number].samples_per_cycle < 524400) {
        OSC[osc_number].osc_out = triangle_wavetable[(8190 * OSC[osc_number].sample_counter) / OSC[osc_number].samples_per_cycle];
        flag_overflow = 0;
      }

      else {
        OSC[osc_number].osc_out = triangle_wavetable[(OSC[osc_number].sample_counter / (OSC[osc_number].samples_per_cycle / 8190))];
        flag_overflow = 1;
      }
      break;

    //---------------------------//
    //------TEPPED TRIANGLE------//
    //---------------------------//
    case 6:
      //how many times you can fit the length of a step in the sample counter, this gives you at which step you are
      OSC[osc_number].stepped_triangle_step = OSC[osc_number].sample_counter / OSC[osc_number].width_stepped_triangle;

      switch (OSC[osc_number].stepped_triangle_step) {
        case 0:
          OSC[osc_number].osc_out = 0;
          break;

        case 1:
          OSC[osc_number].osc_out  = 1365;
          break;

        case 2:
          OSC[osc_number].osc_out  = 2730;
          break;

        case 3:
          OSC[osc_number].osc_out  = 4095;
          break;

        case 4:
          OSC[osc_number].osc_out  = 4095;
          break;

        case 5:
          OSC[osc_number].osc_out  = 2730;
          break;

        case 6:
          OSC[osc_number].osc_out  = 1365;
          break;

        case 7:
          OSC[osc_number].osc_out = 0;
          break;
      }
      break;

    //--------------------//
    //-------NOISE--------//
    //--------------------//
    case 7:
      OSC[osc_number].osc_out = random(0, 4095);
      break;
  }

  OSC[osc_number].sample_counter++;

  if (OSC[osc_number].sample_counter > OSC[osc_number].samples_per_cycle - 1) {
    OSC[osc_number].sample_counter = 0;
  }
}
