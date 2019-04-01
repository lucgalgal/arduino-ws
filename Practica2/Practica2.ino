/* Include required headers and/or libraries */
#include <Wire.h>

/* Single-pass function to configure the app */
uint16_t FuncionTemperatura();
void setup()
{
  /* Join i2c bus (address optional for master) */
  Wire.begin(0, 2);

  /* start serial for output */
  Serial.begin(115200);
}

/* Recurrent task, called forever */
void loop()
{
  /* Welcome message! Useful to know where  */
  Serial.printf("Ahoy! ESP8266 here!\n---\n");

  

  /*For reading I have to use the information given on the arduino documentation*/
  
  Wire.beginTransmission(0x76);                     
  Wire.write(0xD0);              
  Wire.endTransmission();
       
/* Request data from slave with address 0x76 */
  Wire.requestFrom(0x76, 1);

  /* Wait for data to be available */
  while (Wire.available())
  {
    /* Receive the byte */
    uint8_t c = Wire.read();

    /* Send it to console/monitor */
    Serial.printf("Received byte: %02X\n", c);
  }

  /*Call the function where I will obtain the temp measure*/
    uint16_t Temp= FuncionTemperatura();
  /* Send it to console/monitor */
    Serial.printf("Received Temperature: %02X\n", Temp);
  
  /* Ensure not to flood with a huge amount of fast data */
  delay(500);
}

uint16_t FuncionTemperatura(){
 /*Declaring the variables where I'll store the temp value*/
    uint8_t MostSignificantB=0;
    uint8_t LeastSignificantB=0;
  
 /*Reading from the slave*/
  Wire.beginTransmission(0x76);                     
 /*I write the direction from which I will read */
  Wire.write(0xFA);              
  Wire.endTransmission();

  /* Request data from slave with address 0x76, in this case, reading the Temp I have two bytes in which I obtain the temp */
  Wire.requestFrom(0x76, 2);


  /* Wait for data to be available */
  while (Wire.available())
  {
    /* Receive the byte */
    uint8_t MostSignificantB = Wire.read();
    uint8_t LeastSignificantB = Wire.read();
    
    /* Send it to console/monitor */
   Serial.printf("Received MOST: %02X\n", MostSignificantB);
   Serial.printf("Received LEAST: %02X\n", LeastSignificantB);
  }

  return (MostSignificantB << 8) | LeastSignificantB;




}
