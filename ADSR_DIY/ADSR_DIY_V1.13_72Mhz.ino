#include <SPI.h>
#include "variables.h"

void setup() {
  Serial.begin(9600);

  // Setup SPI
  SPI.begin(); //Initialize the SPI port.
  SPI.setBitOrder(MSBFIRST); // Set the SPI bit order
  SPI.setDataMode(SPI_MODE0); //Set the  SPI_2 data mode 0
  SPI.setClockDivider(SPI_CLOCK_DIV8);      // Slow speed (72 / 4 = 18 MHz SPI speed)

  //Setup GPIO
  pinMode(attack_pot, INPUT_ANALOG);
  pinMode(decay_pot, INPUT_ANALOG);
  pinMode(sustain_pot, INPUT_ANALOG);
  pinMode(release_pot, INPUT_ANALOG);
  pinMode(end_of_output, OUTPUT);

  pinMode(lin_toggle, INPUT_PULLUP);
  pinMode(cycle_toggle, INPUT_PULLUP);
  pinMode(long_toggle, INPUT_PULLUP);
  pinMode(short_toggle, INPUT_PULLUP);

  pinMode(gate, INPUT);
  pinMode(retrigger, INPUT);

  pinMode(PC13, OUTPUT);

  pinMode(SPI1_NSS_PIN, OUTPUT);

  adc_set_sample_rate(ADC2, ADC_SMPR_13_5);

  //Interrupt input setup
  attachInterrupt(gate, gate_int, CHANGE);
  attachInterrupt(retrigger, retrig_int, FALLING);

  //---//
  //MCP//
  //---//
  MCP_begin();
  delay(10);

  timer_init();
}

void loop() {
  if (resample) {
    resample = 0;
    recalc_env();

    int_counter++;
    if (int_counter == 96000) {
      int_counter = 0;
      toggle = !toggle;
      digitalWrite(PC13, toggle);
    }

    controls_counter++;
    if (controls_counter == 960) {
      read_Controls();
      controls_counter = 0;
    }
  }
}
