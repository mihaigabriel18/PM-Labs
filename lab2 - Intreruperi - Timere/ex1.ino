unsigned const int a = 11;
unsigned const int b = 10;
unsigned const int c = 7;
unsigned const int d = 8;
unsigned const int e = 9;
unsigned const int f = 12;
unsigned const int g = 13;
unsigned const int dp = 6;
int ledState = LOW;         // Starea curenta a ledului
int buttonState;             // valoarea starii butonului
int lastButtonState = HIGH;   // valoarea anterioara a starii butonului
unsigned const int Button = 3;
unsigned const int LED = 5;
int i = 9;

void isr_btn() {
 i = 9;
 digitalWrite(LED, LOW);
}

void setup() {
 pinMode(Button, INPUT_PULLUP);

 pinMode(LED, OUTPUT);
 digitalWrite(LED, ledState);
 pinMode(a, OUTPUT);  //A
 pinMode(b, OUTPUT);  //B
 pinMode(c, OUTPUT);  //C
 pinMode(d, OUTPUT);  //D
 pinMode(e, OUTPUT);  //E
 pinMode(f, OUTPUT);  //F
 pinMode(g, OUTPUT);  //G
 cli();

 TCCR1A = 0;
 TCCR1B = 0;
 TCNT1  = 0;

 OCR1A = 31249;            // compare match register 16MHz/256/2Hz-1
 TCCR1B |= (1 << WGM12);   // CTC mode
 TCCR1B |= (1 << CS12);    // 256 prescaler
 TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
 sei();

 attachInterrupt(digitalPinToInterrupt(Button), isr_btn, FALLING);
}

void turnOff()
{
 digitalWrite(a, HIGH);
 digitalWrite(b, HIGH);
 digitalWrite(c, HIGH);
 digitalWrite(d, HIGH);
 digitalWrite(e, HIGH);
 digitalWrite(f, HIGH);
 digitalWrite(g, HIGH);
}

void displayDigit(int digit)
{
 if (digit != 0 && digit != 1 && digit != 7)
   digitalWrite(g, LOW);

 //Conditions for displaying segment a
 if (digit != 1 && digit != 4)
   digitalWrite(a, LOW);

 //Conditions for displaying segment b
 if (digit != 5 && digit != 6)
   digitalWrite(b, LOW);

 //Conditions for displaying segment c
 if (digit != 2)
   digitalWrite(c, LOW);

 //Conditions for displaying segment d
 if (digit != 1 && digit != 4 && digit != 7)
   digitalWrite(d, LOW);

 //Conditions for displaying segment e
 if (digit == 2 || digit == 6 || digit == 8 || digit == 0)
   digitalWrite(e, LOW);

 //Conditions for displaying segment f
 if (digit != 1 && digit != 2 && digit != 3 && digit != 7)
   digitalWrite(f, LOW);
}



ISR(TIMER1_COMPA_vect) {
 turnOff();
 if (i >= 0) { 
   displayDigit(i);
   i -= 1;
 } else {
   displayDigit(0);
 }
 if (i < 0) {
   digitalWrite(LED, HIGH);
 }
}

void loop()
{


}
