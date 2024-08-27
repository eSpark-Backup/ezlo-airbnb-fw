#include "device_airbnb.h"

static uint32_t parent_device_id = 0;

uint32_t ezlopi_airbnb_get_parent_device_id(uint32_t l_parent_device_id)
{
    if(!parent_device_id)
    {
        parent_device_id = l_parent_device_id;
        return 0;
    }
    return parent_device_id;
}