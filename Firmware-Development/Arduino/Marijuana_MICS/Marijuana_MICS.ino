#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#define SCL 17
#define SDA 18
#define I2C_ADDRESS 0x49
#define MAX_CO_PPM  1000
#define MAX_NH3_PPM 500
#define MAX_NO2_PPM 10

const float max_volts = 5.0;
const float max_adc_steps = 4096.0;

Adafruit_ADS1015 ads1015;


void setup() 
{
  Serial.begin(9600);
  delay(1000);

  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)");
  Wire.setPins(SDA, SCL);
  if(!ads1015.begin(I2C_ADDRESS))
      Serial.println("faild to connect I2c...");
}

void loop() 
{
  float CO_gas, NH3_gas, NO2_gas; 
  CO_gas = ads1015.readADC_SingleEnded(2);
  NH3_gas = ads1015.readADC_SingleEnded(1);
  NO2_gas = ads1015.readADC_SingleEnded(0);
  Serial.println(CO_gas);
  Serial.println(NH3_gas);
  Serial.println(NO2_gas);

  CO_gas = map(CO_gas, 0, max_adc_steps, 1, MAX_CO_PPM);

  NH3_gas = map(NH3_gas, 0, max_adc_steps, 1, MAX_NH3_PPM);

  NO2_gas = map(NO2_gas, 0, max_adc_steps, 0.05, MAX_NO2_PPM);


  Serial.print("CO_gas(PPM): ");
  Serial.println(CO_gas);
  Serial.print("NH3_gas(PPM): ");
  Serial.println(NH3_gas);
  Serial.print("NO2_gas(PPM): ");
  Serial.println(NO2_gas);

  delay(3000);
}



