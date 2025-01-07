#include <stdlib.h>
#include <cjext.h>
#include <sdkconfig.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ezlopi_util_trace.h"
#include "ezlopi_core_device_value_updated.h"
#include "ezlopi_cloud_constants.h"

#include "radar_app.h"
#include "ezlopi_service_gpioisr.h"
#include "sensor_0097_presence_sensor.h"

static const char *presence_token_vals[3] = {
    "unknown",
    "not_detected",
    "presence",
};

static const char *motion_direction_token_vals[3] = {
    "unknown",
    "incoming",
    "outgoing",
};

// DMA_ATTR static uint16_t samples_spi_buf_1[BUFF_SIZE];
// DMA_ATTR static uint16_t samples_buf1[TOTAL_DATA * 2];
// DMA_ATTR static uint16_t samples_buf2[TOTAL_DATA * 2];

static ezlopi_error_t __prepare(void *arg);
static ezlopi_error_t __init(l_ezlopi_item_t *item);
static ezlopi_error_t __notify(l_ezlopi_item_t *item);
static ezlopi_error_t __get_hub_item(l_ezlopi_item_t *item, void *args);
static ezlopi_error_t __get_cjson_ezlopi_value(l_ezlopi_item_t *item, void *arg);

static int32_t radar_get_fifo_data(s_device_args_t *device_args);

ezlopi_error_t SENSOR_0097_presence_sensor(e_ezlopi_actions_t action, l_ezlopi_item_t *item, void *arg, void *user_arg)
{
    ezlopi_error_t ret = EZPI_SUCCESS;
    // TRACE_E("%s", ezlopi_actions_to_string(action));
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
    {
        ret = __get_hub_item(item, arg);
        break;
    }
    case EZLOPI_ACTION_GET_EZLOPI_VALUE:
    {
        ret = __get_cjson_ezlopi_value(item, arg);
        break;
    }
    case EZLOPI_ACTION_NOTIFY_1000_MS:
    {
        // __notify(item);
        break;
    }
    default:
    {
        break;
    }
    }

    return ret;
}

static ezlopi_error_t __notify(l_ezlopi_item_t *item)
{
    ezlopi_error_t ret = EZPI_FAILED;

    s_device_args_t *device_args = (s_device_args_t *) item->user_arg;

    // esp_err_t error = radar_get_data(device_args);
    // TRACE_I("%s", esp_err_to_name(error));
    static bool presence = 0;
    
    if (ezlopi_item_name_presence == item->cloud_properties.item_name && presence != device_args->target_result.presence)
    {
        presence = device_args->target_result.presence;
        EZPI_core_device_value_updated_from_device_broadcast(item);
        ret = EZPI_SUCCESS;
        
    }   
    else if ((ezlopi_item_name_motion_direction == item->cloud_properties.item_name) && device_args->target_result.presence)
    { 
        TRACE_W("range %f, speed, %f", device_args->target_result.range, device_args->target_result.speed);
        EZPI_core_device_value_updated_from_device_broadcast(item);
        ret = EZPI_SUCCESS;
    }
    
    return ret;
}

static void __add_array_to_object(cJSON *cj_params, const char *const *arr, const int count)
{
    if (cj_params && arr)
    {
        cJSON *enum_array = cJSON_CreateStringArray(__FUNCTION__, arr, count);
        cJSON_AddItemToObject(__FUNCTION__, cj_params, "enum", enum_array);
        // TRACE_I("%s", cJSON_Print(__FUNCTION__, cj_params));
    }
}

static void __add_values_to_cjson(l_ezlopi_item_t *item, cJSON *cj_params, bool is_hub_request)
{
    s_device_args_t *device_args = (s_device_args_t *) item->user_arg;

    char valueFormatted[20];
    if (ezlopi_item_name_velocity == item->cloud_properties.item_name)
    {
        snprintf(valueFormatted, 20, "%0.3f", device_args->target_result.speed);
        cJSON_AddStringToObject(__FUNCTION__, cj_params, "valueFormatted", valueFormatted);
        cJSON_AddNumberToObject(__FUNCTION__, cj_params, "value", device_args->target_result.speed);
    }
    if (ezlopi_item_name_presence == item->cloud_properties.item_name)
    {
        if (is_hub_request)
        {
            __add_array_to_object(cj_params, presence_token_vals, 3);
        }
        cJSON_AddStringToObject(__FUNCTION__, cj_params, "valueFormatted", presence_token_vals[device_args->target_result.presence + 1]);
        cJSON_AddStringToObject(__FUNCTION__, cj_params, "value", presence_token_vals[device_args->target_result.presence + 1]);
    }
    if (ezlopi_item_name_distance == item->cloud_properties.item_name)
    {
        snprintf(valueFormatted, 20, "%0.5f", device_args->target_result.range);
        cJSON_AddStringToObject(__FUNCTION__, cj_params, "valueFormatted", valueFormatted);
        cJSON_AddNumberToObject(__FUNCTION__, cj_params, "value", device_args->target_result.range);
    }
    if (ezlopi_item_name_motion_direction == item->cloud_properties.item_name)
    {
        if (is_hub_request)
        {
            __add_array_to_object(cj_params, motion_direction_token_vals, 3);
        }
    
        const char *motion_direction = motion_direction_token_vals[device_args->target_result.direction];
    
        cJSON_AddStringToObject(__FUNCTION__, cj_params, "valueFormatted", motion_direction);
        cJSON_AddStringToObject(__FUNCTION__, cj_params, "value", motion_direction);
        TRACE_W("----------------------- %s", motion_direction);
    }
    if ((ezlopi_item_name_angle_position == item->cloud_properties.item_name) && (scales_north_pole_degress == item->cloud_properties.scale))
    {
        snprintf(valueFormatted, 20, "%0.3f", device_args->target_result.angle_azimuth);
        cJSON_AddStringToObject(__FUNCTION__, cj_params, "valueFormatted", valueFormatted);
        cJSON_AddNumberToObject(__FUNCTION__, cj_params, "value", device_args->target_result.angle_azimuth);
    }
    if ((ezlopi_item_name_angle_position == item->cloud_properties.item_name) && (scales_elevation_degrees == item->cloud_properties.scale))
    {
        snprintf(valueFormatted, 20, "%0.3f", device_args->target_result.angle_elevation);
        cJSON_AddStringToObject(__FUNCTION__, cj_params, "valueFormatted", valueFormatted);
        cJSON_AddNumberToObject(__FUNCTION__, cj_params, "value", device_args->target_result.angle_elevation);
    }
}

static ezlopi_error_t __get_hub_item(l_ezlopi_item_t *item, void *args)
{
    ezlopi_error_t ret = EZPI_FAILED;

    cJSON *cj_params = (cJSON *)args;
    if (cj_params)
    {
        __add_values_to_cjson(item, cj_params, true);
        ret = EZPI_SUCCESS;
    }

    return ret;
}

static ezlopi_error_t __get_cjson_ezlopi_value(l_ezlopi_item_t *item, void *arg)
{
    ezlopi_error_t ret = EZPI_FAILED;

    cJSON *cj_params = (cJSON *)arg;
    if (cj_params)
    {
        __add_values_to_cjson(item, cj_params, false);
        ret = EZPI_SUCCESS;
    }

    return ret;
}

static ezlopi_error_t __init(l_ezlopi_item_t *item)
{
    ezlopi_error_t ret = EZPI_ERR_INIT_DEVICE_FAILED;

    if (item && item->user_arg)
    {
        s_device_args_t *device_args = (s_device_args_t *)item->user_arg;
        if (NULL == device_args->task_handle)
        {
            device_args->task_handle = 3;
            if(ESP_OK == initializeRadarTask(item))
            {
                TRACE_I("'radar_app_init' OK");
                ret = EZPI_SUCCESS;
            }
        }
        else
        {
            TRACE_W("Presence sensor already initiated!");
        }
    }

    return ret;
}

static ezlopi_error_t __prepare(void *arg)
{
    ezlopi_error_t ret = EZPI_ERR_PREP_DEVICE_PREP_FAILED;

    s_ezlopi_prep_arg_t *prep_arg = (s_ezlopi_prep_arg_t *)arg;
    if (prep_arg)
    {
        s_device_args_t *device_args = malloc(sizeof(s_device_args_t));
        if (device_args)
        {
            memset(device_args, 0, sizeof(s_device_args_t));
            dump("device_args", &device_args, 0, sizeof(s_device_args_t));

            device_args->dma_spi_buffer[0] = heap_caps_malloc(BUFF_SIZE * sizeof(uint16_t), MALLOC_CAP_DMA); // samples_spi_buf_1;
            device_args->dma_spi_buffer[1] = heap_caps_malloc(BUFF_SIZE * sizeof(uint16_t), MALLOC_CAP_DMA); // samples_spi_buf_2;
            
            if(!device_args->dma_spi_buffer[0]) TRACE_E("Unable to allocate memory for dma_spi_buffer[0]");
            if(!device_args->dma_spi_buffer[1]) TRACE_E("Unable to allocate memory for dma_spi_buffer[1]");
            device_args->samples_spi_buf = device_args->dma_spi_buffer[0];

            presence_sensor_dev_item_prep(prep_arg, device_args);
            ret = EZPI_SUCCESS;
        }
    }

    return ret;
}
