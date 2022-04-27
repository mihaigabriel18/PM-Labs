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

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(LED_R, red);
  analogWrite(LED_G, green);
  analogWrite(LED_B, blue);
}

// Convert a given HSV (Hue Saturation Value) to RGB(Red Green Blue) and set the led to the color
//  h is hue value, integer between 0 and 360
//  s is saturation value, double between 0 and 1
//  v is value, double between 0 and 1
void setLedColorHSV(int h, double s, double v) {
  // this is the algorithm to convert from RGB to HSV
  double r = 0;
  double g = 0;
  double b = 0;

  double hf = h / 60.0;

  int i = (int)floor(h / 60.0);
  double f = h / 60.0 - i;
  double pv = v * (1 - s);
  double qv = v * (1 - s * f);
  double tv = v * (1 - s * (1 - f));

  switch (i)
  {
    case 0:
      r = v;
      g = tv;
      b = pv;
      break;
    case 1:
      r = qv;
      g = v;
      b = pv;
      break;
    case 2:
      r = pv;
      g = v;
      b = tv;
      break;
    case 3:
      r = pv;
      g = qv;
      b = v;
      break;
    case 4:
      r = tv;
      g = pv;
      b = v;
      break;
    case 5:
      r = v;
      g = pv;
      b = qv;
      break;
  }

  //set each component to a integer value between 0 and 255
  int red = constrain((int)255 * r, 0, 255);
  int green = constrain((int)255 * g, 0, 255);
  int blue = constrain((int)255 * b, 0, 255);

  setColourRgb(red, green, blue);
}

void loop() {
  for (int i = 0; i < 360; i++) {
    setLedColorHSV(i, 1, 1);
    delay(5);
  }
}

