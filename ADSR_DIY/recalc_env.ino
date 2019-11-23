void recalc_env() {
  if (ADS) {
    if (start_env) {
      start_env = 0;
      A_counter = 0;
      ADC_phase = 0;
    }

    switch (ADC_phase) {
      //-------------------//
      //------ATTACK-------//
      //-------------------//
      case 0:
        //Attack phase
        if (A_counter == 0) {
          attack_samples_amount = (attack * sampling_frequency) / 1000;
          if (end_of_release && !digitalRead(end_of_output)) {
            digitalWrite(end_of_output, HIGH);
          }
        }

        else if (A_counter < attack_samples_amount - 1) {
          output_signal = (A_counter * (4095 - attack_start_point)) / attack_samples_amount + attack_start_point;

          if  (!lin_log) {
            output_signal = map(output_signal, attack_start_point, 4095, 0, 4095);
            output_signal = lookup_log_attack[output_signal];
            output_signal = map(output_signal, 0, 4095, attack_start_point, 4095);
          }
        }

        //If we reached the end of the attack phase, then change the phase byte and reset the d counter
        else if (A_counter == attack_samples_amount - 1) {
          D_counter = 1;
          ADC_phase = 1;
        }

        A_counter++;
        break;

      //------------------//
      //------DECAY-------//
      //------------------//
      case 1:
        if (D_counter == 1) {
          decay_samples_amount = (decay * sampling_frequency) / 1000;
          if (end_of_attack && digitalRead(end_of_output)) {
            digitalWrite(end_of_output, LOW);
          }
        }

        if (D_counter < decay_samples_amount - 1) {
          output_signal = 4095 - (((4095 * (100 - sustain)) / 100) * D_counter) / decay_samples_amount;

          /*
                    Serial.print(D_counter);
                    Serial.print(" --- ");
                    Serial.print(decay_samples_amount);
                    Serial.print(" --- ");
                    Serial.println((((4095 * (100 - sustain)) / 100)* D_counter) / decay_samples_amount);
          */

          if  (!lin_log) {
            output_signal = map(output_signal, 4095 * sustain / 100, 4095, 4095, 0);
            output_signal = lookup_exp_decay[output_signal];
            output_signal = map(output_signal, 0, 4095, 4095 * sustain / 100, 4095);
          }
        }

        if (D_counter == decay_samples_amount - 1) {
          S_counter = 0;
          ADC_phase = 2;
        }

        D_counter++;

        break;

      //--------------------//
      //-------SUSTAIN------//
      //--------------------//
      case 2:
        //restart the attack state after the decay phase
        if (cycling) {
          start_env = 1;
          ADS = 1;

          start_rel = 0;
          REL = 0;
          attack_start_point = output_signal;
        }

        else if (S_counter == 0) {
          if (end_of_attack && !digitalRead(end_of_output)) {
            digitalWrite(end_of_output, HIGH);
          }

          if (end_of_decay && digitalRead(end_of_output)) {
            digitalWrite(end_of_output, LOW);
          }
          output_signal = 4095 * sustain / 100;

          //Increment S_counter from 0 to 1, only that one time
          S_counter = 1;
        }
        break;
    }
  }

  if (REL) {
    if (start_rel) {
      start_rel = 0;
      R_counter = 0;

      if (end_of_decay && !digitalRead(end_of_output)) {
        digitalWrite(end_of_output, HIGH);
      }

      release_samples_amount = (release_ * sampling_frequency) / 1000; //divide 1Mil because attack is in us and sf in hz

      output_signal = release_start_point;
    }

    else if (R_counter < release_samples_amount) {
      output_signal = release_start_point -  (R_counter * release_start_point) / release_samples_amount;

      if  (!lin_log) {
        output_signal = map(output_signal, 0, release_start_point, 4095, 0);
        output_signal = lookup_exp_decay[output_signal];
        output_signal = map(output_signal, 0, 4095, 0, release_start_point);
      }
    }

    if (R_counter == release_samples_amount - 1) {
      output_signal = 0;
      REL = 0;
      if (end_of_release && digitalRead(end_of_output)) {
        digitalWrite(end_of_output, LOW);
      }
    }
    R_counter++;
  }
}
