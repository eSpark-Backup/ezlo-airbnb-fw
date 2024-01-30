#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "BME680.h"
#include "pms5003.hpp"
#include "mics6814.h"
#include "ch2o_sensor.h"

// #define GAS_SENSOR_TEST 0

bme680_fields_t gas_sensor_fields;
ch2o_sensor_data_t ch2o_data;

void app_main()
{
  // gas_sensor_fields.precision = disabled;
  // bme680_setup(gas_sensor_fields.precision);

  // PM25_AQI_Data data;
  // pms_init(&data);

  // mics6814_t adc;
  // mics6814_init(&adc, GPIO_NUM_0); // For Harware Interrupt
  // // mics6814_init(&adc, -1); // For Software Interrupt
  // mics6814_calibrate(&adc);
  ch2o_sensor_init(&ch2o_data);

  while (1)
  {
    if(ch2o_data.available)
    {
        printf("ch2o ppm : %0.2f\r\n", ch2o_data.ppm);
        ch2o_data.available = false;
    }
    
// #if GAS_SENSOR_TEST
//     bme680_test();
// #else
//     pms_print_data(&data);
//     if (bme680_read(&gas_sensor_fields) == ESP_OK)
//     {
//       printf("Temperature = %0.2f *C\n", gas_sensor_fields.temperature);
//       printf("Pressure = %0.2f hPa\n", gas_sensor_fields.pressure);
//       printf("Humidity = %0.2f %%\n", gas_sensor_fields.humidity);
//       printf("Gas = %0.2f KOhms\n", gas_sensor_fields.gas_resistance);
//       printf("Approx. Altitude = %0.2f m\n\n", gas_sensor_fields.altitude);
//     }
// #endif
    // float no2 = mics6814_get_gas(&adc, MICS6814_CH_NO2);
    // float nh3 = mics6814_get_gas(&adc, MICS6814_CH_NH3);
    // float co = mics6814_get_gas(&adc, MICS6814_CH_CO);

    // printf("NO2 = %0.2f\n", no2);
    // printf("NH3 = %0.2f\n", nh3);
    // printf("CO = %0.2f\n", co);
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}
