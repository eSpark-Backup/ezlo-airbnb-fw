#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "BME680.h"
#include "pms5003.hpp"
#include "mics6814.h"
#include "ze08-ch2o.h"
#include "ens160.h"

// #define GAS_SENSOR_TEST 0
// #define GAS_SENSOR_TEST 0
// #define GAS_SENSOR_TEST 0
#define ZE08_CH2O_SENSOR 1

static const char *TAG = "Application";
bme680_fields_t gas_sensor_fields;

#if ZE08_CH2O_SENSOR
ze08_ch2o_sensor_data_t ze08_ch2o_data;
#endif

void app_main(void)
{
    // gas_sensor_fields.precision = disabled;
    // bme680_setup(gas_sensor_fields.precision);
    // PM25_AQI_Data data;
    // pms_init(&data);
    // mics6814_t adc;
    // mics6814_init(&adc, GPIO_NUM_0); // For Harware Interrupt
    // // mics6814_init(&adc, -1); // For Software Interrupt
    // mics6814_calibrate(&adc);
#if ZE08_CH2O_SENSOR
    s_ezlopi_uart_t ze08_uart_config =
    {
        .baudrate = ZE08_CH2O_UART_BUAD_RATE,
        .tx = ZE08_CH2O_TX_PIN,
        .rx = ZE08_CH2O_RX_PIN,
        .enable = true,
    };
    ze08_ch2o_sensor_init(&ze08_uart_config, &ze08_ch2o_data);
#endif
    // Init the sensor
    // if( NO_ERR != DFRobot_ENS160_I2C_begin() ){
    //   ESP_LOGE(TAG, "Communication with device failed, please check connection");
    //   delay(3000);
    // }
    // ESP_LOGI(TAG, "Begin ok!");
    // DFRobot_ENS160_setPWRMode(ENS160_STANDARD_MODE);
    // DFRobot_ENS160_setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);
    while (1)
    {
    // DFRobot_ENS160_test();
#if ZE08_CH2O_SENSOR
        if (ze08_ch2o_data.available)
        {
            printf("ch2o ppm : %0.2f\r\n", ze08_ch2o_data.ppm);
            ze08_ch2o_data.available = false;
        }
#endif
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