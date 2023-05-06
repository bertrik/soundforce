void read_inputs() {
  //--------------//
  //----Toggle----//
  //--------------//
  //mode = 0 --> normal dual independt LFOs
  //mode = 1 --> synced dual LFOs
  mode = digitalRead(mode_toggle);

  if (mode != previous_mode) {
    if (mode) {
      detachInterrupt(tap1);
      OSC[1].sample_counter = 0;
      OSC[0].sample_counter = 0;
      new_pot_values = 1;
    }

    else {
      attachInterrupt(tap1, tap1_int, FALLING);
      new_pot_values = 1;
    }
    previous_mode = mode;
  }

  pinged = !digitalRead(ping_toggle);

  if (pinged != previous_pinged) {
    new_pot_values = 1;
    previous_pinged = pinged;

    //when switching from synced LFOs to free LFOs
    //we reset the dividers to 1
    if (!pinged) {
      OSC[0].sample_counter = 0;
      OSC[1].sample_counter = 0;

      OSC[0].divider = div_ratio_array[0];
      OSC[0].multiplying = 1;
      OSC[1].divider = div_ratio_array[0];
      OSC[1].multiplying = 1;
    }

    if (mode) {
      OSC[0].sample_counter = 0;
      OSC[1].sample_counter = 0;
    }
  }

  //if the toggle switch is in short position, store a 0 in legnth_of_time
  if ((!digitalRead(short_toggle)) && (!(length_of_time == 2))) {
    length_of_time = 2;
    if (!pinged) {
      new_pot_values = 1;
    }
  }

  // a 2 if it's long
  else if (!digitalRead(long_toggle) && (!(length_of_time == 0))) {
    length_of_time = 0;
    if (!pinged) {
      new_pot_values = 1;
    }
  }

  //if neither is true, it must be in the middle position, medium range -> 1
  else if (digitalRead(short_toggle) && digitalRead(long_toggle) && (!(length_of_time == 1))) {
    length_of_time = 1;
    if (!pinged) {
      new_pot_values = 1;
    }
  }

  //------------//
  //----POTS----//
  //------------//

  //read the first pot
  wave1_pot_read = adc_read(ADC2, 1);
  //checking if the new reading is at least bigger than the treshold relatively to the previous reading
  if (abs(wave1_pot_read - old_wave1_pot_read) > analog_threshold || new_pot_values) {
    old_wave1_pot_read = wave1_pot_read;

    //setting the wave type according to the pot value in a bit switch case
    switch (wave1_pot_read) {
       case 0 ... 69: {
          OSC[0].wave_type = 9;
          break;
        }
      
       case 70 ... 169: {
          OSC[0].wave_type = 8;
          break;
        }

       case 170 ... 299: {
          OSC[0].wave_type = 7;
          break;
        }

      case 300 ... 810: {
          OSC[0].wave_type = 6;
          break;
        }

      case 811 ... 1287: {
          OSC[0].wave_type = 5;
          break;
        }

      case 1288 ... 1891: {
          OSC[0].wave_type = 4;
          break;
        }

      case 1892 ... 2442: {
          OSC[0].wave_type = 3;
          break;
        }

      case 2443 ... 3200: {
          OSC[0].wave_type = 2;
          break;
        }

      case 3201 ... 3780: {
          OSC[0].wave_type = 1;
          break;
        }

      case 3781 ... 4095: {
          OSC[0].wave_type = 0;
          break;
        }
    }
  }

  wave2_pot_read = adc_read(ADC2, 8);
  if (abs(wave2_pot_read - old_wave2_pot_read) > analog_threshold || new_pot_values) {
    old_wave2_pot_read = wave2_pot_read;
    switch (wave2_pot_read) {
       case 0 ... 69: {
          OSC[1].wave_type = 9;
          break;
        }
      
       case 70 ... 169: {
          OSC[1].wave_type = 8;
          break;
        }

       case 170 ... 299: {
          OSC[1].wave_type = 7;
          break;
        }
        
      case 300 ... 810: {
          OSC[1].wave_type = 6;
          break;
        }

      case 811 ... 1287: {
          OSC[1].wave_type = 5;
          break;
        }

      case 1288 ... 1891: {
          OSC[1].wave_type = 4;
          break;
        }

      case 1892 ... 2442: {
          OSC[1].wave_type = 3;
          break;
        }

      case 2443 ... 3200: {
          OSC[1].wave_type = 2;
          break;
        }

      case 3201 ... 3780: {
          OSC[1].wave_type = 1;
          break;
        }

      case 3781 ... 4095: {
          OSC[1].wave_type = 0;
          break;
        }
    }
  }

  div_freq1_pot_read = adc_read(ADC2, 2);
  if (abs(div_freq1_pot_read - old_div_freq1_pot_read) > analog_threshold || new_pot_values) {
    old_div_freq1_pot_read = div_freq1_pot_read;

    if (pinged) {
      switch (div_freq1_pot_read) {
        case 0 ... 350: {
            OSC[0].divider_map = 3;
            OSC[0].multiplying = 1;
            break;
          }

        case 351 ... 1000: {
            OSC[0].divider_map = 2;
            OSC[0].multiplying = 1;
            break;
          }

        case 1001 ... 1700: {
            OSC[0].divider_map = 1;
            OSC[0].multiplying = 1;
            break;
          }

        case 1701 ... 2400: {
            OSC[0].divider_map = 0;
            OSC[0].multiplying = 1;
            break;
          }

        case 2401 ... 3000: {
            OSC[0].divider_map = 1;
            OSC[0].multiplying = 0;
            break;
          }

        case 3001 ... 3730: {
            OSC[0].divider_map = 2;
            OSC[0].multiplying = 0;
            break;
          }

        case 3731 ... 4095: {
            OSC[0].divider_map = 3;
            OSC[0].multiplying = 0;
            break;
          }
      }
      OSC[0].divider  = div_ratio_array[OSC[0].divider_map];
    }

    else {
      switch (length_of_time) {
        case 0: {
            //range 1: 1minute to 1hz
            OSC[0].period = map_long(div_freq1_pot_read, 4095, 0, 60000, 1000);
            //please note we have to divide the max and min in the map function
            //AND then multiply by 1000 here and (10 in case 1)
            //because the integers are overflowing during remapping of the pot readings
            //new edit you don't transform to micros because we skip every 100 samples, we only do x10
            OSC[0].period = OSC[0].period * 1000;
            break;
          }
        case 1: {
            //range 2: 1hz to 50hz
            OSC[0].period = map_long(div_freq1_pot_read, 4095, 0, 100000, 2000);
            OSC[0].period = OSC[0].period * 10;
            break;
          }
        case 2: {
            //range 2: 50hz to 100hz
            OSC[0].period = map_long(div_freq1_pot_read, 4095, 0, 20000, 10000);
            break;
          }
      }

      freq_calc_osc(0);

      //if the 2 LFOs are synced then we set period2 to the same value as period1
      if (mode) {
        OSC[1].period = OSC[0].period;
        freq_calc_osc(1);
      }
    }
  }

  div_freq2_pot_read = adc_read(ADC2, 9);
  if (abs(div_freq2_pot_read - old_div_freq2_pot_read) > analog_threshold || new_pot_values) {
    old_div_freq2_pot_read = div_freq2_pot_read;

    if (pinged) {
      switch (div_freq2_pot_read) {
        case 0 ... 350: {
            OSC[1].divider_map = 3;
            OSC[1].multiplying = 1;
            break;
          }

        case 351 ... 1000: {
            OSC[1].divider_map = 2;
            OSC[1].multiplying = 1;
            break;
          }

        case 1001 ... 1700: {
            OSC[1].divider_map = 1;
            OSC[1].multiplying = 1;
            break;
          }

        case 1701 ... 2400: {
            OSC[1].divider_map = 0;
            OSC[1].multiplying = 1;
            break;
          }

        case 2401 ... 3000: {
            OSC[1].divider_map = 1;
            OSC[1].multiplying = 0;
            break;
          }

        case 3001 ... 3730: {
            OSC[1].divider = 2;
            OSC[1].multiplying = 0;
            break;
          }

        case 3731 ... 4095: {
            OSC[1].divider_map = 3;
            OSC[1].multiplying = 0;
            break;
          }
      }
      OSC[1].divider = div_ratio_array[OSC[1].divider_map];
    }

    else {
      if (!mode) {
        switch (length_of_time) {
          case 0: {
              //range 1: 1minute to 1hz
              OSC[1].period = map_long(div_freq2_pot_read, 4095, 0, 60000, 1000);
              //please note we have to divide the max and min in the map function
              //AND then multiply by 1000 here and (10 in case 1)
              //because the integers are overflowing during remapping of the pot readings
              OSC[1].period = OSC[1].period * 1000;
              break;
            }
          case 1: {
              //range 2: 1hz to 50hz
              OSC[1].period = map_long(div_freq2_pot_read, 4095, 0, 100000, 2000);
              OSC[1].period = OSC[1].period * 10;
              break;
            }
          case 2: {
              //range 2: 50hz to 100hz
              OSC[1].period = map_long(div_freq2_pot_read, 4095, 0, 20000, 10000);
              break;
            }
        }
        freq_calc_osc(1);
      }

      if (new_pot_values) {
        new_pot_values = 0;
      }
    }
  }
}
