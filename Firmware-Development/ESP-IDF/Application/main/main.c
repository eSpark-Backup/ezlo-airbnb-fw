#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "pms5003.h"
#include "mics6814.h"
#include "ze08-ch2o.h"
#include "ens160.h"
#include "shtc3.h"
#include "pca9536.h"
#include "bme680_bsec.h"
#include "software_serial.h"

// #define GAS_SENSOR_TEST 0
#define ENS_160_SENSOR 1
#define MICS6814_SENSOR 1
#define ZE08_CH2O_SENSOR 1
#define SHTC3_SENSOR 0

static const char *TAG = "Application";

#if ZE08_CH2O_SENSOR
ze08_ch2o_sensor_data_t ze08_ch2o_data;
#endif

static void app_loop(void *pvParameters)
{
    // s_device_args_t *device_args = (s_device_args_t *) item->user_arg; 
    SwSerial *tmp = sw_new(38, 9, false, 512);
    printf("%u\n", tmp->bitTime);
    if (tmp != NULL)
    {
        sw_open(tmp, 115200); // 115200 > 9600
        while (1)   
        {
            vTaskDelay(100 / portTICK_RATE_MS);
            // printf("check recvd data");
            int len = sw_any(tmp);
            
            if (len > 0)
            {
                for (size_t i = 0; i < len; i++)
                {
                    printf("%02X ", sw_read(tmp));
                }
                printf("\r\n");
            }
        }
    }

    vTaskDelete(NULL);
    
}

void app_main(void)
{
    PM25_AQI_Data data;
    xTaskCreate(app_loop, "app_loop", 2 * 2048, NULL, 10, NULL);
    
    pms_init(&data);
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
    // mics6814_calibrate(&mics6814);
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
    ens160_sensor.ezlopi_i2c->enable = false;
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
    // pca9536_init(&spi_handle);

    // if (!pca9536_begin()){
    //     while (1);
        
    // }
    // pca9536_pin_mode(0, INPUT);
    // pca9536_pin_mode(1, OUTPUT);

    // pca9536_digital_write(1, HIGH);
    // vTaskDelay(2000 / portTICK_RATE_MS);
    // pca9536_digital_write(1, LOW);

#if SHTC3_SENSOR
    s_ezlopi_i2c_master_t spi_handle = {
        .enable = true,
        .address = PCA9536_ADDRESS,
        .clock_speed = 100000,
        .sda = 18,
        .scl = 17,
    };
    shtc3_init_sensor(&spi_handle);
    shtc3_wakeup_sensor(&spi_handle);
#endif

    // ESP_ERROR_CHECK(bme680_init());
    printf("aqi,tvoc(ppb),eco2(ppm),ch2o(ppm),no2,nh3,co,PM 1.0,PM 2.5,PM 10\n");
    vTaskDelay(3000/portTICK_RATE_MS);
    while (1)
    {
        // printf("Value = %d\r\n", pca9536_digital_read(0));
#if SHTC3_SENSOR
        float temperature, humidity;
        shtc3_read_out(&spi_handle, &temperature, &humidity);
        printf("temperature : %0.2f, humidity : %0.2f\r\n", temperature, humidity);
#endif
#if ENS_160_SENSOR
        dfrobot_ens160_get_data(&ens160_sensor);
//   printf("Sensor operating status : %d", status);
        printf("%d,", ens160_sensor.data.aqi);
        printf("%d,", ens160_sensor.data.tvoc);
        printf("%d,", ens160_sensor.data.eco2);
#endif

#if ZE08_CH2O_SENSOR
        if (ze08_ch2o_data.available)
        {
            printf("%0.2f,", ze08_ch2o_data.ppm);
            ze08_ch2o_data.available = false;
        }
#endif

        pms_print_data(&data);
#if MICS6814_SENSOR
        float no2 = mics6814_get_gas(&mics6814, MICS6814_GAS_NO2);
        float nh3 = mics6814_get_gas(&mics6814, MICS6814_GAS_NH3);
        float co = mics6814_get_gas(&mics6814, MICS6814_GAS_CO);
        // float c3h8 = mics6814_get_gas(&mics6814, MICS6814_GAS_C3H8);
        // float c4h10 = mics6814_get_gas(&mics6814, MICS6814_GAS_C4H10);
        // float ch4 = mics6814_get_gas(&mics6814, MICS6814_GAS_CH4);
        // float h2 = mics6814_get_gas(&mics6814, MICS6814_GAS_H2);
        // float c2h5oh = mics6814_get_gas(&mics6814, MICS6814_GAS_C2H5OH);
        printf("%0.2f,", no2);
        printf("%0.2f,", nh3);
        printf("%0.2f\n", co);
        // printf("c3h8 = %0.2f\n", c3h8);
        // printf("c4h10 = %0.2f\n", c4h10);
        // printf("ch4 = %0.2f\n", ch4);
        // printf("h2 = %0.2f\n", h2);
        // printf("c2h5oh = %0.2f\n", c2h5oh);
#endif
        vTaskDelay(200 / portTICK_RATE_MS);
    }
}