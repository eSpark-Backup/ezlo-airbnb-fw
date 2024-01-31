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
#define ENS_160_SENSOR 1
#define MICS6814_SENSOR 0
#define ZE08_CH2O_SENSOR 0

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
#if MICS6814_SENSOR
    mics6814_t mics6814;
    mics6814.adc.ezlopi_i2c = (s_ezlopi_i2c_master_t*) malloc(sizeof(s_ezlopi_i2c_master_t));
    mics6814.adc.ezlopi_i2c->enable = true;
    mics6814.adc.ezlopi_i2c->address = ADS101X_I2C_ADDRESS_VDD;
    mics6814.adc.ezlopi_i2c->channel = I2C_NUM_0;
    mics6814.adc.ezlopi_i2c->sda = ADS101X_I2C_SDA;
    mics6814.adc.ezlopi_i2c->scl = ADS101X_I2C_SCL;
    mics6814.adc.ezlopi_i2c->clock_speed = ADS101X_I2C_SPEED;
    mics6814_init(&mics6814, GPIO_NUM_0); // For Harware Interrupt
    // // mics6814_init(&mics6814, -1); // For Software Interrupt
    mics6814_calibrate(&mics6814);
#endif
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

#if ENS_160_SENSOR
    ens160_t ens160_sensor;
    ens160_sensor.ezlopi_i2c = (s_ezlopi_i2c_master_t*) malloc(sizeof(s_ezlopi_i2c_master_t));
    ens160_sensor.ezlopi_i2c->enable = true;
    ens160_sensor.ezlopi_i2c->address = ENS160_I2C_ADDRESS;
    ens160_sensor.ezlopi_i2c->channel = I2C_NUM_0;
    ens160_sensor.ezlopi_i2c->sda = ENS160_I2C_SDA;
    ens160_sensor.ezlopi_i2c->scl = ENS160_I2C_SCL;
    ens160_sensor.ezlopi_i2c->clock_speed = ENS160_I2C_SPEED;
    // Init the sensor
    if( NO_ERR != dfrobot_ens160_i2c_begin(&ens160_sensor) ){
      ESP_LOGE(TAG, "Communication with device failed, please check connection");
      delay(3000);
    }
    ESP_LOGI(TAG, "Begin ok!");
    dfrobot_ens160_set_pwr_mode(&ens160_sensor, ENS160_STANDARD_MODE);
    dfrobot_ens160_set_temp_and_hum(&ens160_sensor, /*temperature=*/25.0, /*humidity=*/50.0);
#endif

    while (1)
    {
#if ENS_160_SENSOR
        dfrobot_ens160_get_data(&ens160_sensor);
#endif

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
        // float no2 = mics6814_get_gas(&mics6814, MICS6814_CH_NO2);
        // float nh3 = mics6814_get_gas(&mics6814, MICS6814_CH_NH3);
        // float co = mics6814_get_gas(&mics6814, MICS6814_CH_CO);
        // printf("NO2 = %0.2f\n", no2);
        // printf("NH3 = %0.2f\n", nh3);
        // printf("CO = %0.2f\n", co);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}