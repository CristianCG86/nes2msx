/**
  Clone of NESpad library from Josh Marinacci
  https://github.com/joshmarinacci/nespad-arduino
*/

#include <inttypes.h>

#define NES_A       B00000001
#define NES_B       B00000010
#define NES_SELECT  B00000100
#define NES_START   B00001000
#define NES_UP      B00010000
#define NES_DOWN    B00100000
#define NES_LEFT    B01000000
#define NES_RIGHT   B10000000

class NESpad {

  public:
    NESpad();
    NESpad(int strobe, int clock, int data);
    uint8_t buttons();

  private:
    void strobe();
    uint8_t shiftin();
    int m_strobe, m_clock, m_data;

};