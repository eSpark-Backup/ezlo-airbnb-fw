

#ifndef _SENSOR_0069_ZE08_CH02_gas_sensor_H_
#define _SENSOR_0069_ZE08_CH02_gas_sensor_H_


#include "ezlopi_core_actions.h"
#include "ezlopi_core_devices.h"

// Threshold values for smoke
#define SMOKE_TVOC_THRESHOLD 800
#define SMOKE_ECO2_THRESHOLD 600
#define SMOKE_PM2_5_THRESHOLD 150
#define SMOKE_PM1_0_THRESHOLD 100

// Threshold values for vape
#define VAPE_TVOC_THRESHOLD_LOW 160
#define VAPE_TVOC_THRESHOLD_HIGH 200 
#define VAPE_PM2_5_THRESHOLD 1000
#define VAPE_CO_THRESHOLD 5.5f
#define VAPE_NH3_THRESHOLD 6.0f

typedef struct ezlopi_smoke
{
    float co;
    float eco2;
    float nh3;
    uint16_t tvoc;
    uint16_t pm25_standard;
    uint16_t pm10_standard
} ezlopi_smoke_t;

int sensor_0069_ze08_ch02_gas_sensor(e_ezlopi_actions_t action, l_ezlopi_item_t *item, void *arg, void *user_arg);


#endif // _SENSOR_0069_ZE08_CH02_gas_sensor_H_


