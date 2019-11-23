void tap0_int() {
  OSC[0].int_was_triggered = 1;
  OSC[0].clock_ms = micros();
  OSC[0].period =  OSC[0].clock_ms - OSC[0].previous_clock_ms;
  OSC[0].previous_clock_ms = OSC[0].clock_ms;
}

void tap1_int() {
  OSC[1].int_was_triggered = 1;
  OSC[1].clock_ms = micros();
  OSC[1].period =  OSC[1].clock_ms - OSC[1].previous_clock_ms;
  OSC[1].previous_clock_ms = OSC[1].clock_ms;
}

void reset_int() {
  OSC[0].sample_counter = 0;
  OSC[1].sample_counter = 0;
}

