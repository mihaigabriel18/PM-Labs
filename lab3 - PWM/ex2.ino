#define LED 11 
#define BUTTON 2
#define LED_R 11
#define LED_G 10
#define LED_B 9

void setup() {
  pinMode(LED, OUTPUT);
//  pinMode(LED_R, OUTPUT);
//  pinMode(LED_G, OUTPUT);
//  pinMode(LED_B, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  EICRA |= (1 << ISC01);
  EIMSK |= (1 << INT0);
  sei();
  
  Serial.begin(9600);
  Serial.println("astept comenzi");
  setColourRgb(0,0,0);
}
 
char buf[20];
int i = 0;
long ts = 0;
bool blink_state = false;
int r = 255, g = 0, b = 120;

ISR(INT0_vect)
{
  // cod întrerupere externă PD2 /INT0
  // verificare tranziție pe front crescător, descrescător sau oricare
  // (după cum este configurat INT0)
  int res = digitalRead(LED);
  if (millis() - ts > 300) {
    if (res == 0) 
      digitalWrite(LED, HIGH);
    else
      digitalWrite(LED, LOW);
    ts = millis();
  }
  
  
}
 
void loop() {
  unsigned int rgbColour[3];

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;
    int thirdColour = incColour == 2 ? 0 : incColour + 1;
    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      rgbColour[thirdColour] += 1;
      
      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(5);
    }
  }
}

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(LED_R, red);
  analogWrite(LED_G, green);
  analogWrite(LED_B, blue);
 }