#define LED 11 
#define BUTTON 2
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  EICRA |= (1 << ISC01);
  EIMSK |= (1 << INT0);
  sei();
  
  Serial.begin(9600);
  Serial.println("astept comenzi");
}
 
char buf[20];
int i = 0;
long ts = 0;
bool blink_state = false;

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
 
void loop() 
{
  
}