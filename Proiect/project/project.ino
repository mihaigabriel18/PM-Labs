#include <Keypad.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 10     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

Servo myservo;
LiquidCrystal_I2C lcd(0x27,20,4);
DHT dht(DHTPIN, DHTTYPE);

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int buzzer = 1;

const int PASSWD_SIZE = 4;
char passwd_digits[PASSWD_SIZE];
int entered_digits = 0;
char correct_passwd[PASSWD_SIZE] = {'1', '2', '3', '4'};
int flag = 0;
int will_read_temp = 1;

void init_lcd() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(2,0);
    lcd.print("Please enter");
    lcd.setCursor(1,1);
    lcd.print("the password...");
    
}

int counter = 1000;
 
void setup(){
  init_lcd();
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  myservo.attach(11);
  myservo.write(0);

  cli();

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);    // 1024 prescaler
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt

  sei();

  dht.begin();
}

void reset_lock() {
  flag = 0;
  entered_digits = 0;
  passwd_digits[0] = passwd_digits[1] = passwd_digits[2] = passwd_digits[3] = 0;
}

ISR(TIMER2_COMPA_vect) {
  counter--;
}

bool is_digit(char c) {
  if (c >= '0' && c <= '9')
    return true;

  return false;
}

void print_password_progress() {
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print(" Enter password ");
  lcd.setCursor(0,1);

  if (entered_digits == 4) {
    lcd.print("   ");
    lcd.print("-");
    lcd.print(passwd_digits[0]);
    lcd.print(" ");
    lcd.print(passwd_digits[1]);
    lcd.print(" ");
    lcd.print(passwd_digits[2]);
    lcd.print(" ");
    lcd.print(passwd_digits[3]);  
    lcd.print("-");
  } else if (entered_digits == 3) {
    lcd.print(passwd_digits[0]);
    lcd.print("...");
    lcd.print(passwd_digits[1]);
    lcd.print("...");
    lcd.print(passwd_digits[2]);
    lcd.print("...");
  } else if (entered_digits == 2) {
    lcd.print(passwd_digits[0]);
    lcd.print("...");
    lcd.print(passwd_digits[1]);
    lcd.print("...");
  } else if (entered_digits == 1) {
    lcd.print(passwd_digits[0]);
    lcd.print("...");
  } else {
    lcd.print("...");
  }
}

bool check_passwd(char my_passwd[], char correct_passwd[]) {
  for (int i = 0; i < PASSWD_SIZE; i++)
    if (my_passwd[i] != correct_passwd[i])
      return false;

  return true;
}

void ring_buzzer(char message[]) {
  lcd.clear();
  lcd.print(message);
  for (int i = 0; i < 25; i++) {            
    digitalWrite(buzzer, LOW);
    delay(150);
  }
  digitalWrite(buzzer, HIGH);
  lcd.clear();
}

void open_door() {
  for (int i = 0; i < 130; i++) {   
    myservo.write(i);
    delay(20);
  }
}

void close_door() {
  for (int i = 130; i >= 0; i--) {
    myservo.write(i);
    delay(20);
  }
}

void toggle_door() {
  open_door();
  delay(2000);
  close_door();
}

void reset_lcd() {
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Please enter");
  lcd.setCursor(1,1);
  lcd.print("the password...");
}

void loop(){
  if (entered_digits != 0 && counter <= 0 && flag == 1) {
    ring_buzzer("   TIMEOUT   ");
    reset_lock();
    reset_lcd();
  }

  char custom_key = customKeypad.getKey();
  
  if (is_digit(custom_key)){
    if (entered_digits == 0) {      
      counter = 1000;
      flag = 1;
    }
     // "password entry" mode
     passwd_digits[entered_digits++] = custom_key;

     print_password_progress();

     if (entered_digits == 4) {
        if (check_passwd(passwd_digits, correct_passwd)) {
          // check for temperature, if not reached threshold ring buzzer
          counter = 1000;
          while (counter > 0) {
            float t = dht.readTemperature();

            delay(200);
            lcd.clear();
            lcd.print("Time remaining: ");
            lcd.print(counter);
            lcd.setCursor(0, 1);
            lcd.print("Temperature");
            lcd.print(t);

            if (t >= 20.0f || !will_read_temp) {
              lcd.clear();
              lcd.print("OPEN");
              toggle_door();
              reset_lock();
              reset_lcd();
              return;
            }
          }

          lcd.clear();
          lcd.print("   TIMEOUT!!!  ");
          delay(2000);
          ring_buzzer("   WRONG   ");
          reset_lock();
          reset_lcd();
        } else {
          ring_buzzer("   WRONG   ");
          reset_lcd();
        }
        reset_lock();
     }
  } else if (custom_key == 'A') {
    will_read_temp = 1 - will_read_temp;
    lcd.clear();
    if (will_read_temp)
      lcd.print("will read temp");
    else {      
      lcd.print("will NOT read");
      lcd.setCursor(0, 1);
      lcd.print("temp");
    }
    delay(2000);
    reset_lcd();
  }
}
