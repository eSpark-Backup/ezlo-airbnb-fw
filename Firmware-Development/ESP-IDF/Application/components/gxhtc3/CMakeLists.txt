file(GLOB_RECURSE config_src "*.c")

idf_component_register(SRCS "shtc3.c"
    INCLUDE_DIRS "."
    REQUIRES   ezlopi_hal
                )