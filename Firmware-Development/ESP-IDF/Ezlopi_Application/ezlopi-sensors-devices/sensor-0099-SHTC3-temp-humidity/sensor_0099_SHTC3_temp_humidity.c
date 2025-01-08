#include <shtc3.h>
#include <math.h>
#include "ezlopi_util_trace.h"

// #include "ezlopi_core_timer.h"
#include "ezlopi_core_cloud.h"
#include "ezlopi_core_cjson_macros.h"
#include "ezlopi_core_valueformatter.h"
#include "ezlopi_core_device_value_updated.h"

// #include "ezlopi_hal_i2c.h"

#include "ezlopi_cloud_items.h"
#include "ezlopi_cloud_constants.h"

#include "sensor_0099_SHTC3_temp_humidity.h"

typedef struct s_ezlopi_shtc3_data
{
    float temperature;
    float humidity;
} s_ezlopi_shtc3_data_t;

static ezlopi_error_t __prepare(void* arg);
static ezlopi_error_t __init(l_ezlopi_item_t* item);
static ezlopi_error_t __get_cjson_value(l_ezlopi_item_t* item, void* arg);
static ezlopi_error_t __notify(l_ezlopi_item_t* item);

static ezlopi_error_t __shtc3_item_cloud_properties_temp(l_ezlopi_item_t* item, cJSON* cj_device, void* user_arg);
static ezlopi_error_t __shtc3_item_cloud_properties_humidity(l_ezlopi_item_t* item, cJSON* cj_device, void* user_arg);


ezlopi_error_t SENSOR_0099_SHTC3_temp_humidity(e_ezlopi_actions_t action, l_ezlopi_item_t* item, void* arg, void* user_arg)
{
    ezlopi_error_t ret = EZPI_SUCCESS;
    switch (action)
    {
    case EZLOPI_ACTION_PREPARE:
    {
        ret = __prepare(arg);
        break;
    }
    case EZLOPI_ACTION_INITIALIZE:
    {
        ret = __init(item);
        break;
    }
    case EZLOPI_ACTION_HUB_GET_ITEM:
    case EZLOPI_ACTION_GET_EZLOPI_VALUE:
    {
        ret = __get_cjson_value(item, arg);
        break;
    }
    case EZLOPI_ACTION_NOTIFY_1000_MS:
    {
        ret = __notify(item);
        break;
    }
    default:
    {
        break;
    }
    }
    return ret;
}

static void __prepare_device_cloud_properties(l_ezlopi_device_t* device, cJSON* cj_device)
{
    device->cloud_properties.category = category_temperature;
    device->cloud_properties.subcategory = subcategory_not_defined;
    device->cloud_properties.device_type = dev_type_sensor;
    device->cloud_properties.info = NULL;
    device->cloud_properties.device_type_id = NULL;
}

static int __shtc3_item_cloud_properties_temp(l_ezlopi_item_t* item, cJSON* cj_device, void* user_arg)
{
    int ret = 0;
    if (item && cj_device)
    {
        item->user_arg = user_arg;
        item->cloud_properties.show = true;
        item->cloud_properties.has_getter = true;
        item->cloud_properties.has_setter = false;
        item->cloud_properties.item_name = ezlopi_item_name_temp;
        item->cloud_properties.value_type = value_type_temperature;
        item->cloud_properties.scale = scales_celsius;
        item->cloud_properties.item_id = EZPI_core_cloud_generate_item_id();
        CJSON_GET_VALUE_DOUBLE(cj_device, ezlopi_dev_type_str, item->interface_type);

        item->is_user_arg_unique = true;
        item->interface.i2c_master.enable = true;
        item->interface.i2c_master.clock_speed = I2C_MASTER_FREQ_100KHZ;
        CJSON_GET_VALUE_GPIO(cj_device, ezlopi_gpio_scl_str, item->interface.i2c_master.scl);
        CJSON_GET_VALUE_GPIO(cj_device, ezlopi_gpio_sda_str, item->interface.i2c_master.sda);
        // CJSON_GET_VALUE_DOUBLE(cj_device, ezlopi_slave_addr_str, item->interface.i2c_master.address);
        item->interface.i2c_master.address = SHTC3_ADDR;
    }
    return ret;
}

static int __shtc3_item_cloud_properties_humidity(l_ezlopi_item_t* item, cJSON* cj_device, void* user_arg)
{
    int ret = 0;

    if (item && cj_device)
    {
        item->user_arg = user_arg;
        item->cloud_properties.show = true;
        item->cloud_properties.has_getter = true;
        item->cloud_properties.has_setter = false;
        item->cloud_properties.item_name = ezlopi_item_name_humidity;
        item->cloud_properties.value_type = value_type_humidity;
        item->cloud_properties.scale = scales_percent;
        item->cloud_properties.item_id = EZPI_core_cloud_generate_item_id();
        CJSON_GET_VALUE_DOUBLE(cj_device, ezlopi_dev_type_str, item->interface_type);
        
        item->is_user_arg_unique = true;
        item->interface.i2c_master.enable = true;
        item->interface.i2c_master.clock_speed = I2C_MASTER_FREQ_100KHZ;
        CJSON_GET_VALUE_GPIO(cj_device, ezlopi_gpio_scl_str, item->interface.i2c_master.scl);
        CJSON_GET_VALUE_GPIO(cj_device, ezlopi_gpio_sda_str, item->interface.i2c_master.sda);
        // CJSON_GET_VALUE_DOUBLE(cj_device, ezlopi_slave_addr_str, item->interface.i2c_master.address);
        item->interface.i2c_master.address = SHTC3_ADDR;
    }

    return ret;
}

static ezlopi_error_t __prepare(void* arg)
{
    ezlopi_error_t ret = EZPI_ERR_PREP_DEVICE_PREP_FAILED;
    s_ezlopi_prep_arg_t* prep_arg = (s_ezlopi_prep_arg_t*)arg;

    if (prep_arg)
    {
        cJSON* cj_device = prep_arg->cjson_device;
        s_ezlopi_shtc3_data_t* user_data = (s_ezlopi_shtc3_data_t*)ezlopi_malloc(__FUNCTION__, sizeof(s_ezlopi_shtc3_data_t));
        if (user_data)
        {
            memset(user_data, 0, sizeof(s_ezlopi_shtc3_data_t));
            l_ezlopi_device_t* device = EZPI_core_device_add_device(cj_device, "SHT");
            if (device)
            {
                __prepare_device_cloud_properties(device, cj_device);
                
                l_ezlopi_item_t* temperature_item = EZPI_core_device_add_item_to_device(device, SENSOR_0099_SHTC3_temp_humidity);
                if (temperature_item)
                {
                    __shtc3_item_cloud_properties_temp(temperature_item, cj_device, user_data);
                }
                else
                {
                    EZPI_core_device_free_device(device);
                    ezlopi_free(__FUNCTION__, user_data);
                    return ret;
                }

                l_ezlopi_item_t* humidity_item = EZPI_core_device_add_item_to_device(device, SENSOR_0099_SHTC3_temp_humidity);
                if (humidity_item)
                {
                    ret = EZPI_SUCCESS;
                    __shtc3_item_cloud_properties_humidity(humidity_item, cj_device, user_data);
                }
                else
                {
                    EZPI_core_device_free_device(device);
                    ezlopi_free(__FUNCTION__, user_data);
                    return ret;
                }
            }
        }
    }

    return ret;
}

static ezlopi_error_t __init(l_ezlopi_item_t* item)
{
    ezlopi_error_t ret = EZPI_ERR_INIT_DEVICE_FAILED;
    if (item)
    {
        s_ezlopi_shtc3_data_t* user_data = (s_ezlopi_shtc3_data_t*)item->user_arg;
        if (user_data)
        {
            if (item->interface.i2c_master.enable && ezlopi_item_name_temp == item->cloud_properties.item_name)
            {
                ret = EZPI_SUCCESS;
                shtc3_init_sensor(&item->interface.i2c_master);
            }
            else if(ezlopi_item_name_humidity == item->cloud_properties.item_name)
            {
                
            }
        }
    }
    return ret;
}

static ezlopi_error_t __get_cjson_value(l_ezlopi_item_t* item, void* arg)
{
    ezlopi_error_t ret = EZPI_FAILED;

    if (item && arg)
    {
        cJSON* cj_device = (cJSON*)arg;
        if (cj_device)
        {
            s_ezlopi_shtc3_data_t* user_data = (s_ezlopi_shtc3_data_t*)item->user_arg;

            if (user_data)
            {
                if (ezlopi_item_name_temp == item->cloud_properties.item_name)
                {
                    EZPI_core_valueformatter_float_to_cjson(cj_device, user_data->temperature, item->cloud_properties.scale);
                }
                else if (ezlopi_item_name_humidity == item->cloud_properties.item_name)
                {
                    EZPI_core_valueformatter_float_to_cjson(cj_device, user_data->humidity, item->cloud_properties.scale);
                }

                ret = EZPI_SUCCESS;
            }
        }
    }

    return ret;
}

static ezlopi_error_t __notify(l_ezlopi_item_t* item)
{
    ezlopi_error_t ret = EZPI_FAILED;

    if (item)
    {
        s_ezlopi_shtc3_data_t* user_data = (s_ezlopi_shtc3_data_t*)item->user_arg;
        if (user_data)
        {
            float temperature = user_data->temperature;
            float humidity = user_data->humidity;
            
            shtc3_wakeup_sensor(&item->interface.i2c_master);
            shtc3_read_out(&item->interface.i2c_master,&temperature, &humidity);
            shtc3_sleep_sensor(&item->interface.i2c_master);

            if (ezlopi_item_name_temp == item->cloud_properties.item_name)
            {
                if (fabs(user_data->temperature - temperature) > 0.05)
                {
                    user_data->temperature = temperature;
                    EZPI_core_device_value_updated_from_device_broadcast(item);
                }
            }
            if (ezlopi_item_name_humidity == item->cloud_properties.item_name)
            {
                if (fabs(user_data->humidity - humidity) > 0.05)
                {
                    user_data->humidity = humidity;
                    EZPI_core_device_value_updated_from_device_broadcast(item);
                }
            }
            ret = EZPI_SUCCESS;
        }
    }

    return ret;
}