#include <NESpad.h>

/**
  ---- Prerequisites -------------------------------------------

  ATtiny boards from David A. Mellis
  https://github.com/damellis/attiny

  And select:
    Board -> attiny -> ATtiny 24/44/84
    Clock -> Internal 8 Mhz
    Processor -> ATtiny44


  NESpad library from Josh Marinacci
  https://github.com/joshmarinacci/nespad-arduino


  ---- Wire connection -----------------------------------------

  NES controller conection          MSX controller conection                                                        
          ____                           _____________      
         | 1   \                         \ 1 2 3 4 5 /      
         | 2  5 |                         \ 6 7 8 9 /       
         | 3  6 |                           -------         
         | 4  7 |                                           
          ------                      ┌─────┬───────────┐   
                                      │ MSX │   ATtiny  │   
    ┌─────┬───────────┐               ├─────┼───────────┤   
    │ NES │   ATtiny  │               │  1  │  PA0 [13] │   
    ├─────┼───────────┤               │  2  │  PA1 [12] │   
    │  1  │  VCC [1]  │               │  3  │  PA2 [11] │   
    │  2  │  PB1 [3]  │               │  4  │  PA3 [10] │   
    │  3  │  PB0 [2]  │               │  5  │  VCC [1]  │   
    │  4  │  PB2 [5]  │               │  6  │  PA5 [8]  │   
    │  5  │  GND [14] │               │  7  │  PA4 [9]  │   
    │  6  │  NC       │               │  8  │  NC       │   
    │  7  │  NC       │               │  9  │  GND [14] │   
    └─────┴───────────┘               └─────┴───────────┘   
*/

//  Turbo speed config
#define TURBO_INITIAL_SPEED 20
#define TURBO_MAX_SPEED 20
#define TURBO_INCREASE_SPEED 10

//  Turbo variables
uint8_t turbo_select_counter;
uint8_t turbo_select_toggler;
uint8_t turbo_select_pushed;
uint8_t turbo_select_speed;

uint8_t turbo_start_counter;
uint8_t turbo_start_toggler;
uint8_t turbo_start_pushed;
int8_t turbo_start_speed;


//  NES controller reader
NESpad nintendo = NESpad(10, 9, 8);

//  Input buffer
byte state = 0;

//  Output buffer
uint8_t state_processed;

void setup() {
  // Set port B as outputs and set to high
  DDRA = B11111111;
  PORTA = B11111111;

  // Initilization turbo vars
  turbo_select_counter = 0;
  turbo_select_toggler = 0;
  turbo_select_pushed = 0;

  turbo_start_counter = 0;
  turbo_start_toggler = 0;
  turbo_start_pushed = 0;
}

void loop() {
  
  state = nintendo.buttons();

  // Reorder buttons
  state_processed = ((state & (1 << 0)) << 5) |  // A -> bit5
           ((state & (1 << 1)) << 3) |  // B -> bit4
           ((state & (1 << 7)) >> 4) |  // R -> bit3
           ((state & (1 << 6)) >> 4) |  // L -> bit2
           ((state & (1 << 5)) >> 4) |  // D -> bit1
           ((state & (1 << 4)) >> 4);   // U -> bit0

  // Turbo SELECT (B)
  if(state & NES_SELECT) {
    turbo_select_pushed = 1;
  } else if(turbo_select_pushed == 1) {
    run_b_turbo();
    turbo_select_pushed = 0;
  }

  if(turbo_select_toggler == 1 && (turbo_select_counter >> 3) == 0) {
    state_processed = state_processed & 0b11101111;
  }

  // Turbo START (A)
  if(state & NES_START) {
    turbo_start_pushed = 1;
  } else if(turbo_start_pushed == 1) {
    run_a_turbo();
    turbo_start_pushed = 0;
  }

  if(turbo_start_toggler == 1 && (turbo_start_counter >> 3) == 0) {
    state_processed = state_processed & 0b11011111;
  }

  // Put data button into MSX port
  PORTA = ~(state_processed & 0b00111111);

  // Turbo counters
  turbo_select_counter++;
  if(turbo_select_counter > turbo_select_speed) turbo_select_counter = 0;
  
  turbo_start_counter++;
  if(turbo_start_counter > turbo_start_speed) turbo_start_counter = 0;
  
  delay(5);
}

//  Turbo operations (A)
void run_a_turbo() {
  if(turbo_start_toggler == 0) {
    turbo_start_toggler = 1;
    turbo_start_speed = TURBO_INITIAL_SPEED;
  } else if(turbo_start_speed < TURBO_MAX_SPEED) {
    turbo_start_speed += TURBO_INCREASE_SPEED;
  } else {
    turbo_start_toggler = 0;
    turbo_start_speed = 0;
  }
}

//  Turbo operations (B)
void run_b_turbo() {
  if(turbo_select_toggler == 0) {
    turbo_select_toggler = 1;
    turbo_select_speed = TURBO_INITIAL_SPEED;
  } else if(turbo_select_speed < TURBO_MAX_SPEED) {
    turbo_select_speed += TURBO_INCREASE_SPEED;
  } else {
    turbo_select_toggler = 0;
    turbo_select_speed = 0;
  }
}
