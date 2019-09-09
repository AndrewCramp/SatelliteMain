#include "SDCard.h"
#include "Satellite.h"
#include "main.h"
#include <stdio.h>
/*@brief mounts sd card
 * @ret return 1 if succesful 0 if unsucessfull
 */
int SD_Config(){
	FRESULT FR ;
	if(xSemaphoreTake(SD_Semaphore,1000)){
		FR = f_mount(&myFatFS,SDPath,1);
		xSemaphoreGive(SD_Semaphore);
	}else{
		//Failed to access SD peripherial
		return 0;
	}
	if(FR == FR_OK){
		return 1;
	}
	return 0;
}

int createFile(char name[]){
	if(xSemaphoreTake(SD_Semaphore,1000)){
		FRESULT FR = f_open(&fileObj, name, FA_CREATE_NEW);
		if(FR == FR_OK){
			f_close(&fileObj);
		}
		xSemaphoreGive(SD_Semaphore);
	}else{
		//Failed to access SD peripherial
		return 0;
	}
	return 1;
}
/*@bief writes data to Data.txt file on SDCard
 * @param data: data to be written, size: size of data to be written
 * @ret number of bytes written to card
 */
UINT writeData(char name[], char data[], int size){
	UINT bytesWritten = 0;
	if(xSemaphoreTake(SD_Semaphore,1000)){
		if(f_open(&fileObj, name, FA_WRITE | FA_OPEN_APPEND) == FR_OK){
			f_lseek(&fileObj, f_size(&fileObj));
			f_write(&fileObj,data,size,&bytesWritten);
			f_close(&fileObj);
		}
		xSemaphoreGive(SD_Semaphore);
	}else{
		//Failed to access SD peripherial
		return 0;
	}
	return bytesWritten;
}

UINT formattedWriteInt(char name[], char data[], int size){
	UINT bytesWritten = 0;
	if(xSemaphoreTake(SD_Semaphore,1000)){
		if(f_open(&fileObj, name, FA_WRITE | FA_OPEN_APPEND) == FR_OK){
			f_lseek(&fileObj, f_size(&fileObj));
			for(int i = 0; i < size; i++){
				f_printf(&fileObj,"%d",data[i]);
				if(i != size-1){
					f_printf(&fileObj,",");
				}
			}
			f_printf(&fileObj,"\n");
			f_close(&fileObj);
		}
		xSemaphoreGive(SD_Semaphore);
		}else{
			//Failed to access SD peripherial
			return 0;
		}
	return bytesWritten;
}

UINT formattedWriteFloat(char name[], float data[], int size){
	UINT bytesWritten = 0;
	RTC_TimeTypeDef RTC_Time;
	RTC_DateTypeDef RTC_Date;
	if(xSemaphoreTake(SD_Semaphore,1000)){
		volatile FRESULT FR= f_open(&fileObj, name, FA_WRITE | FA_OPEN_APPEND);
		if(FR == FR_OK){
			f_lseek(&fileObj, f_size(&fileObj));
			HAL_RTC_GetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);
			f_printf(&fileObj,"%d/%d/%d,",RTC_Date.Date,RTC_Date.Month,RTC_Date.Year);
			f_printf(&fileObj,"%d:%d:%d,",RTC_Time.Hours,RTC_Time.Minutes,RTC_Time.Seconds);
			for(int i = 0; i < size; i++){
				char buffer[20];
				floatToString(data[i],buffer);
				int test = f_printf(&fileObj,"%s",buffer);
				if(test){
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
					HAL_Delay(10);
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
					HAL_Delay(10);
				}else{
					HAL_Delay(1000);
					formattedWriteFloat(name, data,size);
				}
				if(i != size-1){
					f_printf(&fileObj,",");
				}
			}
			f_printf(&fileObj,"\n");
			FR = f_close(&fileObj);
		}else{
			xSemaphoreGive(SD_Semaphore);
			HAL_Delay(100);
			SD_Config();
			formattedWriteFloat(name, data,size);
		}
		xSemaphoreGive(SD_Semaphore);
	}else{
	//Failed to access SD peripherial
	return 0;
	}
	return bytesWritten;
}

UINT writeString(char name[], char data[]){
	UINT bytesWritten = 0;
	if(xSemaphoreTake(SD_Semaphore,1000)){
		if(f_open(&fileObj, name, FA_WRITE | FA_OPEN_APPEND) == FR_OK){
			f_lseek(&fileObj, f_size(&fileObj));
			f_printf(&fileObj,"%s",data);
			f_printf(&fileObj,"\n");
			f_close(&fileObj);
		}
		xSemaphoreGive(SD_Semaphore);
	}else{
		//Failed to access SD peripherial
		return 0;
	}
	return bytesWritten;
}


char getFileSize(char fileName[]){
	FIL sizeObj;
	char size;
	if(xSemaphoreTake(SD_Semaphore,1000)){
		f_open(&sizeObj,fileName,FA_READ);
		size = (int)f_size(&sizeObj);
		f_close(&sizeObj);
		xSemaphoreGive(SD_Semaphore);
	}else{
		//Failed to access SD peripherial
		return 0;
	}
	return size;
}

int initFileSystem(){
	SD_Semaphore = xSemaphoreCreateMutex();
	SD_Config();
	createFile("GravData.csv");
	if(getFileSize("GravData.csv") < 1){
		writeString("GravData.csv","Date,Time,GravX,GravY,GravZ,Calibration Status,BNO Temp");
	}
	return 1;
}
