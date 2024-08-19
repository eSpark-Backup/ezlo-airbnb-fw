# # Define service components
# set(EZLOPI_SERVICES
#         ezlopi-service-gpioisr
# )

idf_component_register(SRCS
                            "RangeDopplerMap/range_doppler_map.c"
                            "RangeAngleImage/range_angle_image.c"
                            "AngleCapon/angle_capon.c"
                            "Presence/presence.c"
                            "MotionAngle/motion_angle.c"
                        INCLUDE_DIRS "RangeDopplerMap" "RangeAngleImage" "AngleCapon" "MotionAngle" "Presence" "."
                        REQUIRES ifxAvian ifxRadar)


# set_property(TARGET ${COMPONENT_LIB} APPEND PROPERTY LINK_INTERFACE_MULTIPLICITY 3)
