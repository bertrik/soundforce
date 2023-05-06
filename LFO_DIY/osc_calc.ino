void osc_calc(unsigned int osc_number) {
  switch (OSC[osc_number].wave_type) {

    //-----------------------//
    //----------SINE---------//
    //-----------------------//
    case 0:
      if (OSC[osc_number].samples_per_cycle < 429000) {
        OSC[osc_number].osc_out_new = sine_12_bits[(10000 * OSC[osc_number].sample_counter) / OSC[osc_number].samples_per_cycle];
        flag_overflow = 0;
      }

      else {
        //in some cases, the sine wave would restart from the beginning of the array too soon
        unsigned long array_position = OSC[osc_number].sample_counter / (OSC[osc_number].samples_per_cycle / 10000);
        if (array_position > 9999) {
          array_position = 9999;
        }
        OSC[osc_number].osc_out_new = sine_12_bits[array_position];
        flag_overflow = 1;
      }

      break;

    //---------------------//
    //-------PULSE 50------//
    //---------------------//
    case 1:
      if (OSC[osc_number].sample_counter <= OSC[osc_number].pulse_width_in_cycles_50) {
        OSC[osc_number].osc_out_new = 4095;
      }
      else {
        OSC[osc_number].osc_out_new = 0;
      }
      break;

    //---------------------//
    //-------PULSE 25------//
    //---------------------//
    case 2:
      if (OSC[osc_number].sample_counter <= OSC[osc_number].pulse_width_in_cycles_25) {
        OSC[osc_number].osc_out_new = 4095;
      }
      else {
        OSC[osc_number].osc_out_new = 0;
      }
      break;

    //-----------------------//
    //-------RISING SAW------//
    //-----------------------//
    case 3:

      if (OSC[osc_number].samples_per_cycle < 1048800) {
        OSC[osc_number].osc_out_new = (4095 * OSC[osc_number].sample_counter) / OSC[osc_number].samples_per_cycle;
        flag_overflow = 0;
      }

      else {
        OSC[osc_number].osc_out_new = (OSC[osc_number].sample_counter / (OSC[osc_number].samples_per_cycle / 4095));
        flag_overflow = 1;
      }
      break;

    //----------------------//
    //-----FALLING SAW------//
    //----------------------//
    case 4:
      if (OSC[osc_number].samples_per_cycle < 1048800) {
        OSC[osc_number].osc_out_new = 4095 - (4095 * OSC[osc_number].sample_counter) / OSC[osc_number].samples_per_cycle;
        flag_overflow = 0;
      }

      else {
        OSC[osc_number].osc_out_new = 4095 - (OSC[osc_number].sample_counter / (OSC[osc_number].samples_per_cycle / 4095));
        flag_overflow = 1;
      }
      break;

    //-----------------------//
    //-------TRIANGLE--------//
    //-----------------------//
    case 5:
      if (OSC[osc_number].samples_per_cycle < 524400) {
        OSC[osc_number].osc_out_new = triangle_wavetable[(8190 * OSC[osc_number].sample_counter) / OSC[osc_number].samples_per_cycle];
        flag_overflow = 0;
      }

      else {
        OSC[osc_number].osc_out_new = triangle_wavetable[(OSC[osc_number].sample_counter / (OSC[osc_number].samples_per_cycle / 8190))];
        flag_overflow = 1;
      }
      break;

    //---------------------------//
    //------STEPPED TRIANGLE------//
    //---------------------------//
    case 6:
      //how many times you can fit the length of a step in the sample counter, this gives you at which step you are
      OSC[osc_number].stepped_triangle_step = OSC[osc_number].sample_counter / OSC[osc_number].width_stepped_triangle;

      switch (OSC[osc_number].stepped_triangle_step) {
        case 0:
          OSC[osc_number].osc_out_new = 0;
          break;

        case 1:
          OSC[osc_number].osc_out_new  = 1365;
          break;

        case 2:
          OSC[osc_number].osc_out_new  = 2730;
          break;

        case 3:
          OSC[osc_number].osc_out_new  = 4095;
          break;

        case 4:
          OSC[osc_number].osc_out_new  = 4095;
          break;

        case 5:
          OSC[osc_number].osc_out_new  = 2730;
          break;

        case 6:
          OSC[osc_number].osc_out_new  = 1365;
          break;

        case 7:
          OSC[osc_number].osc_out_new = 0;
          break;
      }
      break;

    //--------------------------//
    //----RANDOM WALK NOISE-----//
    //--------------------------//
    case 7:
      if (OSC[osc_number].sample_counter <= OSC[osc_number].pulse_width_in_cycles_50) {
        OSC[osc_number].osc_state = 1;
      }
      else {
        OSC[osc_number].osc_state = 0;
      }
      //Sample and hold random numbers or continuous noise for period < 10100 (shortest setting)
      if (OSC[osc_number].osc_last != OSC[osc_number].osc_state ||
          OSC[osc_number].period < 10150) {
        OSC[osc_number].osc_last = OSC[osc_number].osc_state;
        
        //Random walk
        OSC[osc_number].last_rand+=random(-255, 256);
        while (OSC[osc_number].last_rand<0){
          OSC[osc_number].last_rand+=random(0, 127);
        }
        while (OSC[osc_number].last_rand>4095){
          OSC[osc_number].last_rand-=random(0, 127);
        }
        OSC[osc_number].osc_out_new =(uint32_t) OSC[osc_number].last_rand;
      }
      break;


    //-------------------------//
    //-------PINK NOISE--------//
    //-------------------------//
    case 8:
      if (OSC[osc_number].sample_counter <= OSC[osc_number].pulse_width_in_cycles_50) {
        OSC[osc_number].osc_state = 1;
      }
      else {
        OSC[osc_number].osc_state = 0;
      }
      //Sample and hold random numbers or continuous noise for period < 10100 (shortest setting)
      if (OSC[osc_number].osc_last != OSC[osc_number].osc_state ||
          OSC[osc_number].period < 10150) {
        OSC[osc_number].osc_last = OSC[osc_number].osc_state;
        //Random white noise
        float white = (float)random(-1023, 1024);
        //Simple version of filter to shape white noise to pink noise
        //See: http://www.firstpr.com.au/dsp/pink-noise/
        OSC[osc_number].pink_b0 = 0.99765 * OSC[osc_number].pink_b0 + white * 0.0990460;
        OSC[osc_number].pink_b1 = 0.96300 * OSC[osc_number].pink_b1 + white * 0.2965164;
        OSC[osc_number].pink_b2 = 0.57000 * OSC[osc_number].pink_b2 + white * 1.0526913;
        OSC[osc_number].osc_out_new = (uint32_t)( OSC[osc_number].pink_b0 + 
                                                  OSC[osc_number].pink_b1 + 
                                                  OSC[osc_number].pink_b2 + white * 0.1848 + 2048 );
        
        if (OSC[osc_number].osc_out_new>4095){
          OSC[osc_number].osc_out_new=4095;
        }
      }
      break;

    //--------------------------//
    //-------WHITE NOISE--------//
    //--------------------------//
    case 9:
      if (OSC[osc_number].sample_counter <= OSC[osc_number].pulse_width_in_cycles_50) {
        OSC[osc_number].osc_state = 1;
      }
      else {
        OSC[osc_number].osc_state = 0;
      }
      //Sample and hold random numbers or continuous noise for period < 10100 (shortest setting)
      if (OSC[osc_number].osc_last != OSC[osc_number].osc_state ||
          OSC[osc_number].period < 10150) {
        OSC[osc_number].osc_last = OSC[osc_number].osc_state;
        //Random white noise
        OSC[osc_number].osc_out_new = random(0, 4095);
      }
      break;
  }

  //Rolling average filter
  OSC[osc_number].filter_reg=(OSC[osc_number].filter_reg+OSC[osc_number].osc_out_new)/2;
  OSC[osc_number].osc_out=int(OSC[osc_number].filter_reg);


  OSC[osc_number].sample_counter++;

  if (OSC[osc_number].sample_counter > OSC[osc_number].samples_per_cycle - 1) {
    OSC[osc_number].sample_counter = 0;
  }
}
