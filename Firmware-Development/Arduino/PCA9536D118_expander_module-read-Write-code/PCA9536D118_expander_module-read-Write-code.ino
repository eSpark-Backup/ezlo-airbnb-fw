/*
  Using the PCA9536 -- Digital Output
  By: Jim Lindblom
  SparkFun Electronics
  Date: May 4, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14733
  This example demonstrates how to use pinMode and digitalWrite/write to set output values of the PCA9536
  
  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the Qwiic RGB Sensor onto the shield
  Watch each LED turn on one-at-a-time
*/

#include <SparkFun_PCA9536_Arduino_Library.h>
#define SDA 18
#define SCL 17
PCA9536 io;

void setup()
{
  Serial.begin(115200);
  Serial.println("PCA9536 example");

  Wire.begin(SDA, SCL);

  // Initialize the PCA9536 with a begin function
  if (io.begin() == false)
  {
    Serial.println("PCA9536 not detected. Please check wiring. Freezing...");
    // while (1)
    //   ;
  }

//  for (int i = 0; i < 4; i++)
  //{
    // pinMode can be used to set an I/O as OUTPUT or INPUT
    io.pinMode(0, INPUT);
    io.pinMode(1, INPUT);
  // }
}

void loop()
{
  Serial.println(io.digitalRead((0)));//temper switch IO0


  //   io.digitalWrite(1, HIGH);//Siren IO1
  //   delay(1000);
  // io.digitalWrite(1, LOW);//Siren IO1
  // delay(1000);
}