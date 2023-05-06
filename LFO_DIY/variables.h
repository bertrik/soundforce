//-------------------//
//-----MCP DACs------//
//-------------------//
#define CHANNEL_A false
#define CHANNEL_B true
#define GAIN_LOW false
#define GAIN_HIGH true

#define SPI1_NSS_PIN PA3

boolean gainA;
boolean gainB;
boolean shutdownA;
boolean shutdownB;

//-------------------//
//-----IOs labels----//
//-------------------//
#define long_toggle PB7
#define short_toggle PB9
#define mode_toggle PB6
#define ping_toggle PB8

#define tap0 PA12
#define tap1 PB5
#define reset_input PA11

#define wave1_pot PA1
#define div_freq1_pot PA2
#define wave2_pot PB0
#define div_freq2_pot PB1

//-------------------//
//-----Variables-----//
//-------------------//
unsigned long wave1_pot_read = 0;
unsigned long div_freq1_pot_read = 0;
unsigned long wave2_pot_read = 0;
unsigned long div_freq2_pot_read = 0;

unsigned long old_wave1_pot_read = 0;
unsigned long old_div_freq1_pot_read = 0;
unsigned long old_wave2_pot_read = 0;
unsigned long old_div_freq2_pot_read = 0;

int analog_threshold = 10;

bool new_pot_values = 1;
bool mode = 0;
byte previous_mode = 8; //initialize with something that's always different than 0 or 1
bool pinged = 0;
bool previous_pinged = 0;

byte length_of_time = 0;
int inputs_timer = 0;
boolean avoid_overflow = 0;
boolean flag_overflow = 0;

//-------------------//
//------TIMING-------//
//-------------------//
uint32_t sampling_frequency = 72;
bool resample = 1;

//---------------//
//--OSCILLATORS--//
//---------------//
struct OSC {
  uint32_t pulse_width_in_cycles_50 = 0;
  uint32_t pulse_width_in_cycles_25 = 0;
  uint32_t pulse_width_in_cycles_12 = 0;
  uint32_t width_stepped_triangle = 0;
  uint32_t stepped_triangle_step = 0;
  uint32_t sample_counter = 0;
  uint32_t sample_counter_overflow = 0;
  uint32_t samples_per_cycle = 0;
  uint32_t osc_out = 0;
  uint8_t osc_state=0;
  uint32_t osc_last=0;
  double osc_out_new=0;
  //Random sample
  int32_t last_rand=0;
  //Pink noise filter
  float pink_b0=0;
  float pink_b1=0;
  float pink_b2=0;
  //Register for rolling average
  double filter_reg=0;
  byte wave_type = 0;
  byte divider = 2;
  byte divider_map = 2;
  uint32_t period;
  uint32_t clock_ms;
  uint32_t previous_clock_ms;
  boolean int_was_triggered;
  boolean multiplying;
  uint32_t div_counter = 0;
};

//-------------------//
//---Wave types------//
//-------------------//
//float div_ratio_array[9] = {0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16};
unsigned long div_ratio_array[4] = {1, 2, 3, 4};
bool multiplying_1 = 0;
bool multiplying_2 = 0;

int OUT1_wave_type = 0;
int OUT2_wave_type = 0;

int old_OUT1_wave_type = 0;
int old_OUT2_wave_type = 0;
