set(EZLOPI_COMPONENTS
        ezlopi-util-trace
)
set(EZLOPI_CORE_COMPONENTS
        ezlopi-core-actions
        ezlopi-core-devices
        # ezlopi-core-timer
        ezlopi-core-valueformatter 
)
set(EZLOPI_CLOUD_COMPONENTS
    ezlopi-cloud-constants
)
idf_component_register(SRCS "sensor_0097_presence_sensor.c" "radar_app.c" "presence_sensor_dev_item_preps.c"
                        INCLUDE_DIRS .
                        REQUIRES ${EZLOPI_COMPONENTS}
                                ${EZLOPI_CORE_COMPONENTS}
                                ${EZLOPI_CLOUD_COMPONENTS}
                                radar_app
                                ifxRadar 
                                ifxAvian)


