#ifndef sensor_lib_shim_h
#define sensor_lib_shim_h

#include "settings.h"

#if ULTRASONIC_LIB == 0
#include <HCSR04.h>
typedef UltraSonicDistanceSensor Sensor;
typedef float Distance;
#endif
#if ULTRASONIC_LIB == 1
#include "AJSR04M_clone.h"
typedef AJSR04M_clone Sensor;
typedef float Distance;
#endif

#endif
