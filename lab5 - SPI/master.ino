# include <SPI.h>

char str[ ]="Hello Slave, I'm Arduino Family\n";

void setup() 
{
Serial.begin(9600); // set baud rate to 115200 for usart
pinMode(SS, OUTPUT);
digitalWrite(SS, LOW);
pinMode(MOSI, OUTPUT);
pinMode(13, OUTPUT);


/* set master mode */
SPCR |= (1 << MSTR);
////
/////* set prescaler 16 */
SPCR |=  (1 << SPR0);
////
//* enable SPI  */
SPCR |= (1 << SPE);

Serial.println("Hello I'm SPI Mega_Master");
} 

void loop (void)
 {
 digitalWrite(SS, LOW); // enable Slave Select
 // send test string
 for(int i=0; i< sizeof(str); i++) {
    SPDR = (unsigned int) str[i];

/* Wait for transmission complete */
    while(!(SPSR & (1 << SPIF)));
  }
 digitalWrite(SS, HIGH); // disable Slave Select
 delay(2000);
}