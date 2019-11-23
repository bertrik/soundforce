void read_Controls() {

  //--------------//
  //----Toggle----//
  //--------------//
  lin_log = !digitalRead(lin_toggle);
  if (lin_log != previous_lin_log) {
    new_pot_values = 1;
  }

  cycle = !digitalRead(cycle_toggle);

  if (cycle && !cycling) {
    cycling = 1;
    //detachInterrupt(gate);
  }

  if (!cycle && cycling) {
    cycling = 0;
    //attachInterrupt(gate, gate_int, CHANGE);
  }

  //if the toggle switch is in short position, store a 0 in legnth_of_time
  if ((!digitalRead(short_toggle)) && (!(length_of_time == 0))) {
    length_of_time = 0;
    new_pot_values = 1;
  }

  // a 2 if it's long
  if (!digitalRead(long_toggle) && (!(length_of_time == 2))) {
    length_of_time = 2;
    new_pot_values = 1;
  }

  //if neither is true, it must be in the middle position, medium range -> 1
  if (digitalRead(short_toggle) && digitalRead(long_toggle) && (!(length_of_time == 1))) {
    length_of_time = 1;
    new_pot_values = 1;
  }

  //------------//
  //----POTS----//
  //------------//

  attack_pot_read = adc_read(ADC2, 1);
  if (abs(attack_pot_read - old_attack_pot_read) > analog_threshold) {
    old_attack_pot_read = attack_pot_read;
    new_pot_values = 1;
  }

  decay_pot_read = adc_read(ADC2, 2);
  if (abs(decay_pot_read - old_decay_pot_read) > analog_threshold) {
    old_decay_pot_read = decay_pot_read;
    new_pot_values = 1;
  }

  sustain_pot_read = adc_read(ADC2, 8);
  if (abs(sustain_pot_read - old_sustain_pot_read) > analog_threshold) {
    old_sustain_pot_read = sustain_pot_read;
    new_pot_values = 1;
  }

  release_pot_read = adc_read(ADC2, 9);
  if (abs(release_pot_read - old_release_pot_read) > analog_threshold) {
    old_release_pot_read = release_pot_read;
    new_pot_values = 1;
  }

  if (new_pot_values) {
    new_pot_values = 0;

    mapped_attack_pot_read = 4095 - attack_pot_read;
    mapped_decay_pot_read = 4095 - decay_pot_read;
    mapped_sustain_pot_read = 4095 - sustain_pot_read;
    mapped_release_pot_read = 4095 - release_pot_read;

    switch (length_of_time) {
      case 0:
        attack = map_unsigned_long(mapped_attack_pot_read, 0, 4095, 2, 15000);
        decay = map_unsigned_long(mapped_decay_pot_read, 0, 4095, 6, 25000);
        release_ = map_unsigned_long(mapped_release_pot_read, 0, 4095, 6, 25000);
        break;

      case 1:
        attack = map_unsigned_long(mapped_attack_pot_read, 0, 4095, 18, 100000);
        decay = map_unsigned_long(mapped_decay_pot_read, 0, 4095, 35, 100000);
        release_ = map_unsigned_long(mapped_release_pot_read, 0, 4095, 35, 100000);
        break;

      case 2:
        attack = map_unsigned_long(mapped_attack_pot_read, 0, 4095, 93, 600000);
        decay = map_unsigned_long(mapped_decay_pot_read, 0, 4095, 300, 600000);
        release_ = map_unsigned_long(mapped_release_pot_read, 0, 4095, 300, 600000);
        break;
    }

    sustain = map_unsigned_long(mapped_sustain_pot_read, 0, 4095, 0, 100);
    attack = attack * 100;
    decay = decay * 100;
    release_ = release_ * 100;

    /*
        Serial.print(attack);
        Serial.print(" - ");
        Serial.print(decay);
        Serial.print(" - ");
        Serial.print(sustain);
        Serial.print(" - ");
        Serial.println(release_);
    */

  }
}
