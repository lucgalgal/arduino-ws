/* Include required headers and/or libraries */
#include <Wire.h>

#define SLAVE_ADDRESS 0x76
#define BLOCK_FIRST_ADDRESS 0x88
#define BLOCK_LENGTH 118

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
  
  Wire.beginTransmission(SLAVE_ADDRESS);                     
  Wire.write(BLOCK_FIRST_ADDRESS);              
  Wire.endTransmission();

  /* Use static to prevent block re-allocation on each pass */
  /*             NAME    LENGTH OF THE ARRAY  VALUE*/
  static uint8_t memory_map[BLOCK_LENGTH] = { 0x00 };
  
  /* Array index, stores position to write a received byte */
  uint8_t memory_idx = 0;

  /* Used to check the received amount */
  uint8_t rx_bytes = 0;

       
  /* Request data from slave with address 0x76 */
  Wire.requestFrom(SLAVE_ADDRESS, BLOCK_LENGTH);

  /* Wait for data to be available */
  while (rx_bytes == 0)
  {
    rx_bytes=Wire.available();
  }

  /*Whenever a data is available store it in the array*/
  for(memory_idx = 0; memory_idx < rx_bytes; memory_idx++)
  {
    memory_map[memory_idx]=Wire.read();
  }

  /* Print the block */
  Serial.printf(" ++ | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
  Serial.printf("----+------------------------------------------------");
  for (memory_idx = 0; memory_idx < rx_bytes; memory_idx++)
  {
    /* Create a new line each 16 printed bytes */
    if ((memory_idx % 16) == 0)
    {
      Serial.printf("\n %02X |", memory_idx + BLOCK_FIRST_ADDRESS);
    }

    /* Print a byte */
    Serial.printf(" %02X", memory_map[memory_idx]);
  }
  Serial.printf("\nEnd of data\n");

  /* Ensure not to flood with a huge amount of fast data */
  delay(500);

  

   
  /*Call the function where I will obtain the temp measure*/
   // uint16_t Temp= FuncionTemperatura();
  /* Send it to console/monitor */
   // Serial.printf("Received Temperature: %02X\n", Temp);
  
  /* Ensure not to flood with a huge amount of fast data */
  // delay(500);
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

  /*Right now, I have to read 118 bytes, all the registers from the BME280*/
  Wire.requestFrom(0x76, 2);


  /* Wait for data to be available */
  while (Wire.available())
  {
    /* Receive the byte */
    MostSignificantB = Wire.read();
    LeastSignificantB = Wire.read();
    
    /* Send it to console/monitor */
   Serial.printf("Received MOST: %02X\n", MostSignificantB);
   Serial.printf("Received LEAST: %02X\n", LeastSignificantB);
  }

  return (MostSignificantB << 8) | LeastSignificantB;




}
