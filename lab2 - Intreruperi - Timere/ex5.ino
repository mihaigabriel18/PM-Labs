/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/
#define BUTTON 2

#include <Servo.h>

int rising = 1;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
char buf[20];
int i = 0;

void setup() {
  rising = 1;
  pinMode(BUTTON, INPUT_PULLUP);
  myservo.attach(5);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  Serial.begin(9600);
  Serial.println("astept comenzi");
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18);
}
long ts = 0;
ISR(PCINT2_vect){
  // cod întrerupere de tip pin change
  if ((PIND & (1 << PD3)) == 0){
      if (millis() - ts > 300) {
        rising = 1 - rising;
        ts = millis();
      }
  }
  // întreruperea a fost generată de alt pin
}

void loop() {
  if(Serial.available())
  {
    char current_char = Serial.read();
 
 
    if (current_char=='\n')
    {
      if (strcmp(buf,"reverse") == 0)
      {
        rising = 1 - rising;
      }
       buf[0] = '\0';
       i = 0;
    }
    else
    {
      buf[i] = current_char;
      buf[i+1] = '\0';
      i++;
    } 
  }
  if (rising == 1) {
      pos = pos == 180 ? 180 : pos + 1; // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);
  }
  else {
      pos = pos == 0 ? 0 : pos - 1;// goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);
  }
}