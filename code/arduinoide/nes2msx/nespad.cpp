#include "nespad.h"
#include "Arduino.h"

NESpad::NESpad(int strobe, int clock, int data)
  : m_strobe (strobe),
  m_clock (clock),
  m_data (data)
  {
    pinMode(strobe, OUTPUT);
    pinMode(clock,  OUTPUT);
    pinMode(data, INPUT_PULLUP);
  }

byte NESpad::buttons(void)
{
  byte ret = 0;
  byte i;
  strobe();
  for (i = 0; i < 8; i++) {
    ret |= shiftin() << i;
  }
  return ~ret;
}

void NESpad::strobe(void)
{
  digitalWrite(m_strobe,HIGH);
  delayMicroseconds(12);
  digitalWrite(m_strobe,LOW);
}

byte NESpad::shiftin(void)
{
  byte ret = digitalRead(m_data);
  delayMicroseconds(12);
  digitalWrite(m_clock,HIGH);
  delayMicroseconds(12);
  digitalWrite(m_clock,LOW);
  return ret;
}
