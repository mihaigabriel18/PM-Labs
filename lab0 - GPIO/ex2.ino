#include "usart.h"
#define BUT 2

void setup()
{
  DDRD &= ~(1 << BUT);
  PORTD |= (1 << BUT);
//  pinMode(BUT, INPUT_PULLUP);
  USART0_init();
  USART0_print("Hello\n");
}

char pre_val = 0;
void loop()
{
  
  char val = PIND & (1 << BUT);

//  Serial.println(val != 0);
  if (val != pre_val) {
  if (val == 0)
    USART0_print("Am lasat butonul\n");
  else
    USART0_print("Am ridicat butonul\n");
  }
  pre_val = val;
}