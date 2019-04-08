/* Include required headers and/or libraries */ 
#include <Wire.h> 
#include "SparkFunBME280.h" 
/*For the file*/
#include <FS.h>
#define TESTFILE "/test_file.txt"
#define MY_STR_LEN 1024
/*For the sensor*/
#define SLAVE_ADDRESS 0x76 
/* Instantiate a BME280 object called BME280_obj */ 
BME280 BME280_obj; 

/*Prototype of the fuctions I'll use*/
 data BMfuction ();
 void store (data sensor);
 
/* 
 * Single-pass function to configure the app 
 */ 
void setup() 
{ 
  /* Start serial for output */ 
  Serial.begin(115200); 
 
  /* Join I2C bus and set it to 400 kHz */ 
  Wire.begin(0, 2); 
  Wire.setClock(400000); 
 
  /* Address the sensor */ 
  BME280_obj.setI2CAddress(SLAVE_ADDRESS); 
 
  /* Check communication before continue */ 
  if (BME280_obj.beginI2C(Wire) == false) 
  { 
    Serial.printf("The sensor did not respond. Please check wiring.\n"); 
 
    /* Stop here (WDT will reset at some point) */ 
    while(1); 
  } 
   /* Initialize the file system */
  Serial.printf("Initializing SPIFFS\n");
  if (SPIFFS.begin() == false)
  {
    Serial.printf("SPIFFS cannot be initialized\n");

    /* Stop here (WDT will reset at some point) */
    while(1) {};
  }
} 
 
/* 
 * Recurrent task, called forever 
 */ 
void loop() 
{ 
   struct values
  {
    uint16_t H;
    uint16_t P;
    uint16_t A; 
    uint16_t T;    
  };

  struct values sensor;
  /* Welcome message! Useful as a control point */ 
  Serial.printf("Ahoy! ESP8266 here!\n---\n"); 
  
  sensor= data BMfuction ();
   
 
  /* Ensure not to flood with a huge amount of fast data */ 
  delay(500); 
} 

data BMfuction ()
{
/*Declare all variables I'll need*/ 

struct data 
{
  uint16_t H;
  uint16_t P;
  uint16_t A; 
  uint16_t T;    
};

struct data sensor;
   
  /*Read all sensors */ 
  sensor.H = BME280_obj.readFloatHumidity(); 
  Serial.printf("Humidity: %f \n",H); 
   
 
  sensor.P = BME280_obj.readFloatPressure(); 
  Serial.printf(" Pressure: %f \n",P); 
   
  sensor.A = BME280_obj.readFloatAltitudeMeters(); 
  Serial.printf(" Alt: %f \n",A); 
   
  //Serial.print(BME280_obj.readFloatAltitudeFeet(), 1); 
   
  sensor.T = BME280_obj.readTempC(); 
  Serial.printf(" Temp: %f \n",T); 
   
  //Serial.printf(BME280_obj.readTempF(), 2); 
 
  Serial.printf("\n"); 

  return sensor;
}

void store (data sensor)
{
  struct data 
  {
  uint16_t H;
  uint16_t P;
  uint16_t A; 
  uint16_t T;    
  };

  File test_file;
  
  uint8_t my_string[MY_STR_LEN];
  uint16_t my_line = 0;

  /* The file already exist? */
  if (SPIFFS.exists(TESTFILE))
  {
    Serial.printf("File '" TESTFILE "'' IS found'\n");
  }
  else
  {
    Serial.printf("File '" TESTFILE "'' NOT found'\n");
  }

  /* Mode 'a+' create if not exists:
   *  - Read from the beginning of the file
   *  - Append new data at the end
   *  * Useful for buffers ;)
   */
  test_file = SPIFFS.open(TESTFILE, "a+");
  if (!test_file)
  {
    /* Oh man, this is serious */
    Serial.printf("Cannot open '" TESTFILE "'' for appending'\n");
  }
  else
  {
    Serial.printf("Opened '" TESTFILE "'\n");

    /* Opened, now put some (at the end of the file) */
    Serial.printf("Filling file '" TESTFILE "' with some data\n");
    test_file.printf("Data Humidity: %d", sensor.H);
    test_file.printf("Data Pressure: %d", sensor.P);
    test_file.printf("Data Height: %d", sensor.A);
    test_file.printf("Data Temperature: %d", sensor.T);
    test_file.close();
    
    test_file.close();

    /* Mode 'r' create if not exists:
     *  - Read from the beginning of the file
     *  - Fails if file not exists
     *  * Useful for safe readings without data modification
     */
    test_file = SPIFFS.open(TESTFILE, "r");
    Serial.printf("Reopened '" TESTFILE "' for reading\n");
    Serial.printf("Contents of file '" TESTFILE "'\n");
    my_line = 0;
    while (test_file.position() < test_file.size())
    {
      test_file.readBytesUntil('\n', my_string, MY_STR_LEN);
      Serial.printf("Line %03d: %s\n", my_line++, my_string);
    }

    /* Done, free/close the file */
    test_file.close();
    Serial.printf("Closed '" TESTFILE "'\n");

    /* Remove the file */
    SPIFFS.remove(TESTFILE);
    Serial.printf("Removed '" TESTFILE "'\n");
  }

  /* Process is locked until reset is performed */
  Serial.printf("Locking now\n");
  while (true)
  {
    /* Ensure other tasks are working (avoid WDT reset) */
    yield();
  }

}
