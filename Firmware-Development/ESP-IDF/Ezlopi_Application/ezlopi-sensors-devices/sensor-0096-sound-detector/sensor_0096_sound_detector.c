
#include <stdlib.h>
#include <string.h>

#include "ezlopi_util_trace.h"

#include "ezlopi_core_actions.h"
#include "ezlopi_core_devices_list.h"
#include "ezlopi_core_cjson_macros.h"
#include "ezlopi_core_devices.h"
#include "ezlopi_core_valueformatter.h"
#include "ezlopi_core_device_value_updated.h"

#include "ezlopi_cloud_constants.h"
#include "device_airbnb.h"
#include "sensor_0096_sound_detector.h"
#include "sensor_0096_sound_detector_settings.h"
#include "EZLOPI_USER_CONFIG.h"

static int __prepare(void* arg, void* user_arg);
static int __init(l_ezlopi_item_t* item);
static int __get_item(l_ezlopi_item_t* item, void* arg);
static int __get_cjson_value(l_ezlopi_item_t* item, void* arg);
static int __notify(l_ezlopi_item_t* item);

static int __prepare_sound_detector_sensor(cJSON* cj_device, void* user_arg);

static void __prepare_sound_detector_device_cloud_properties(l_ezlopi_device_t* device);
static void __prepare_sound_detector_item_cloud_properties(l_ezlopi_item_t* item, cJSON* cj_device, void* user_arg);

/*
{\
    \"dev_type\": 6,\
    \"dev_name\": \"Sound Sensor\",\
    \"id_room\": 35,\
    \"id_item\": 69,\
    \"gpio_tx\": 48,\
    \"gpio_rx\": 45,\
    \"baud\": 9600\
}\
*/

int sensor_0096_sound_detector(e_ezlopi_actions_t action, l_ezlopi_item_t* item, void* arg, void* user_arg)
{
    int ret = 0;
    // TRACE_D("%s", ezlopi_actions_to_string(action));
    switch (action)
    {
    case EZLOPI_ACTION_PREPARE:
    {
        ret = __prepare(arg, user_arg);
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

static int __get_cjson_value(l_ezlopi_item_t* item, void* arg)
{
    int ret = 0;

    cJSON* cj_params = (cJSON*)arg;

    ezlopi_valueformatter_float_to_cjson(cj_params, 123, item->cloud_properties.scale);
    
    return ret;
}

static int __notify(l_ezlopi_item_t* item)
{
    int ret = 0;

    // ze08_ch2o_sensor_data_t* ze08_ch2o_sensor = (ze08_ch2o_sensor_data_t*)item->user_arg;
    // if (ze08_ch2o_sensor && ze08_ch2o_sensor->available)
    // {
        ezlopi_device_value_updated_from_device_broadcast(item);
    //     ze08_ch2o_sensor->available = false;
    // }
    // else
    // {
    //     ret = 1;
    // }

    return ret;
}

static int __init(l_ezlopi_item_t* item)
{
    int ret = 0;
    return ret;
}

static void __prepare_sound_detector_device_cloud_properties(l_ezlopi_device_t* device)
{
    device->cloud_properties.category = category_level_sensor;
    device->cloud_properties.subcategory = subcategory_sound;
    device->cloud_properties.device_type = dev_type_sensor;
    device->cloud_properties.device_type_id = NULL;
    device->cloud_properties.info = NULL;
    device->cloud_properties.parent_device_id = ezlopi_airbnb_get_parent_device_id(device->cloud_properties.device_id);
}

static void __prepare_sound_detector_item_cloud_properties(l_ezlopi_item_t* item, cJSON* cj_device, void* user_arg)
{
    item->interface_type = EZLOPI_DEVICE_INTERFACE_UART;
    item->cloud_properties.has_getter = true;
    item->cloud_properties.has_setter = false;
    item->cloud_properties.item_id = ezlopi_cloud_generate_item_id();
    item->cloud_properties.item_name = ezlopi_item_name_loudness;
    item->cloud_properties.value_type = value_type_loudness;
    item->cloud_properties.scale = scales_decibel;
}

static int __prepare(void* arg, void* user_arg)
{
    int ret = 0;
    s_ezlopi_prep_arg_t* prep_arg = (s_ezlopi_prep_arg_t*)arg;
    if (prep_arg)
    {
        ret = __prepare_sound_detector_sensor(prep_arg->cjson_device, NULL);
    }

    return ret;
}

static int __prepare_sound_detector_sensor(cJSON* cj_device, void* user_arg)
{
    int ret = 0;
    l_ezlopi_device_t* device = ezlopi_device_add_device(cj_device, NULL);

    if (device)
    {
        ret = 1;
        __prepare_sound_detector_device_cloud_properties(device);

        l_ezlopi_item_t* item = ezlopi_device_add_item_to_device(device, sensor_0096_sound_detector);
        if (item)
        {
            __prepare_sound_detector_item_cloud_properties(item, cj_device, user_arg);
            sensor_0096_sound_detector_settings_initialize(device, user_arg);
        }
        else
        {
            ret = -1;
            ezlopi_device_free_device(device);
        }
    }
    else
    {
        ret = -1;
    }

    return ret;
}
