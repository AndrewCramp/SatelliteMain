#ifndef SDCARD_H_
#define SDCARD_H_
#include "stm32f4xx_hal.h"

#define READ_ID 0xF8
#define WRITE_ID  0x78
#define SENSOR_W 2592
#define SENSOR_H 1944

typedef enum{
	TIMING_CONTROL_HS_MSB = 0x3800,
	TIMING_CONTROL_HS_LSB = 0x3801
}CAMERA_REGISTER;


#endif

