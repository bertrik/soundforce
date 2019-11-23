void gate_int() {
  //start attack/delay/sustain
  if (!digitalRead(gate)) {
    start_env = 1;
    ADS = 1;

    start_rel = 0;
    REL = 0;
    attack_start_point = output_signal;
  }

  //start release
  else {
    start_env = 0;
    ADS = 0;

    start_rel = 1;
    REL = 1;
    release_start_point = output_signal;
  }
}

//if the retrig input is going up start the attack
void retrig_int() {
  if (!digitalRead(gate)) {
    start_env = 1;
    ADS = 1;

    start_rel = 0;
    REL = 0;
    attack_start_point = output_signal;
  }
}
