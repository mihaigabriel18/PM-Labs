#define LED 13
#define BUT 2

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUT, INPUT);
  Serial.begin(9600);
  Serial.println("astept comenzi");
}

char buf[20];
long ts = 0;
int is_blink = 0;

void loop()
{
  
  if (Serial.available()){
    char a = Serial.read();
    

    if (a == '\n') {
      if (strcmp(buf, "on") == 0) {
        is_blink = 0;
        digitalWrite(LED, HIGH);
      } else if (strcmp(buf, "off") == 0) {
        is_blink = 0;
        digitalWrite(LED, LOW);
      } else if (strcmp(buf, "get") == 0) {
        Serial.println(digitalRead(LED) == 0 ? "Oprit" : "Pornit");
      } else if (strcmp(buf, "blink") == 0) {
        is_blink = !is_blink;
      } 
     
      buf[0] = '\0';
    } else {
        char aux[2];
        aux[0] = a;
        aux[1] = '\0';
        strcat(buf, aux);
    }
  }
  
     if (is_blink == 1) {
         if ((millis() - ts) >= 200) {
          digitalWrite(LED, digitalRead(LED) == HIGH ? LOW : HIGH);
          ts = millis();
        }
      }
}