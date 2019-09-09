#include "main.h"
#include "Satellite.h"
#include "Camera.h"

int CAM_write(CAMERA_REGISTER reg, uint8_t pValue){
	while(HAL_I2C_IsDeviceReady(phi2c, WRITE_ID,1,0xff)!= HAL_OK){
		HAL_Delay(50);
	}
	if(HAL_I2C_Mem_Write(phi2c, WRITE_ID, reg,sizeof(reg), &pValue, sizeof(pValue),0xff) != HAL_OK){
		//error handling
		for(;;);
		return 0;
	}
	return 1;
}

signed char CAM_read(CAMERA_REGISTER reg){
	uint8_t dataBuffer = 0;
	if(HAL_I2C_Mem_Read(phi2c, READ_ID, reg, sizeof(reg), &dataBuffer, sizeof(uint8_t),0xff) != HAL_OK){
		//error handling
		return 0;
	}
	return dataBuffer;
}

void CAM_Image_Windowing(int x, int y, int width, int height){
	char xLSB = x & 0xff;
	char xMSB = x & 0xff00;
	char yLSB = y & 0xff;
	char yMSB = y & 0xff00;
	char widthLSB = width & 0xff;
	char widthMSB = width & 0xff00;
	char heightLSB = height & 0xff;
	char heightMSB = height & 0xff00;



}

