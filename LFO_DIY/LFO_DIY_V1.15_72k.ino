#include <SPI.h>
#include "variables.h"
#include "triangle_array.h"
#include "sine_array.h"

struct OSC OSC[2];

void setup() {
  Serial.begin(9600);

  //Setup GPIO
  pinMode(wave1_pot, INPUT_ANALOG);
  pinMode(div_freq1_pot, INPUT_ANALOG);
  pinMode(wave2_pot, INPUT_ANALOG);
  pinMode(div_freq2_pot, INPUT_ANALOG);

  pinMode(long_toggle, INPUT_PULLUP);
  pinMode(short_toggle, INPUT_PULLUP);
  pinMode(mode_toggle, INPUT_PULLUP);
  pinMode(ping_toggle, INPUT_PULLUP);

  pinMode(tap0, INPUT);
  pinMode(tap1, INPUT);
  pinMode(reset_input, INPUT);

  pinMode(SPI1_NSS_PIN, OUTPUT);

  pinMode(PC13, OUTPUT);

  adc_set_sample_rate(ADC2, ADC_SMPR_1_5);

  //Interrupt input setup
  attachInterrupt(tap0, tap0_int, FALLING);
  attachInterrupt(tap1, tap1_int, FALLING);
  attachInterrupt(reset_input, reset_int, FALLING);

  //---//
  //MCP//
  //---//
  // Setup SPI
  SPI.begin(); //Initialize the SPI port.
  SPI.setBitOrder(MSBFIRST); // Set the SPI bit order
  SPI.setDataMode(SPI_MODE0); //Set the  SPI_2 data mode 0
  SPI.setClockDivider(SPI_CLOCK_DIV8);      // Slow speed (72 / 4 = 18 MHz SPI speed)

  MCP_begin();
  delay(10);

  //Look at the controls before starting the system
  read_inputs();

  timer_init();

  //if in free mode, start the timer right away, when using the pinged mode we wait until we get a ping
  if (!pinged) {
    Timer2.resume();
  }

}

void loop() {
  if (resample) {
    resample = 0;
    osc_calc(0);
    osc_calc(1);

    inputs_timer++;
    if (inputs_timer == 4800) {
      inputs_timer = 0;
      read_inputs();

      /*
            Serial.print("Sample counter - ");
            Serial.print(OSC[0].sample_counter);
            Serial.print("- Sample/cycle - ");
            Serial.print(OSC[0].samples_per_cycle);
            Serial.print("- Period - ");
            Serial.print(OSC[0].period);
            Serial.print("- avoid overflow - ");
            Serial.print(avoid_overflow);
            Serial.print("- flag overflow - ");
            Serial.println(flag_overflow);
      */

    }

    if (OSC[0].int_was_triggered) {
      OSC[0].int_was_triggered = 0;

      if (pinged) {
        OSC[0].div_counter++;

        //if we are mutliplying the clock every new gate tape is an opportunity to resync the lfo
        //calculting the new frequency/period, and resetting the counters
        if (OSC[0].multiplying) {
          freq_calc_osc(0);
          OSC[0].div_counter = 0;
          OSC[0].sample_counter = 0;
        }

        //if we are dividing the clock, we wait until we reached the desider amount of taps
        //to reysnc the lfo and necessary counters
        else {
          if (OSC[0].div_counter >= (1 << OSC[0].divider - 1)) {
            freq_calc_osc(0);
            OSC[0].div_counter = 0;
            OSC[0].sample_counter = 0;
          }
        }
      }

      //if the 2 LFOs are synced then we call the lfo 2 interrupt routines as well
      if (mode) {
        tap1_int();
      }
    }

    if (OSC[1].int_was_triggered) {
      OSC[1].int_was_triggered = 0;

      if (pinged) {
        OSC[1].div_counter++;

        //if we are mutliplying the clock every new gate tape is an opportunity to resync the lfo
        //calculting the new frequency/period, and resetting the counters
        if (OSC[1].multiplying) {
          freq_calc_osc(1);
          OSC[1].div_counter = 0;
          OSC[1].sample_counter = 0;
        }

        //if we are dividing the clock, we wait until we reached the desider amount of taps
        //to reysnc the lfo and necessary counters
        else {
          if (OSC[1].div_counter >= (1 << OSC[1].divider - 1)) {
            freq_calc_osc(1);
            OSC[1].div_counter = 0;
            OSC[1].sample_counter = 0;
          }
        }
      }
    }
  }
}
