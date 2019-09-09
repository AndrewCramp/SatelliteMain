#ifndef SDCARD_H_
#define SDCARD_H_
#include "stm32f4xx_hal.h"
#include "fatfs.h"

int SD_Config(void );
int createFile(char name[]);
UINT writeData(char name[], char data[], int size);
UINT formattedWriteInt(char name[], char data[], int size);
UINT formattedWriteFloat(char name[], float data[], int size);
int initFileSystem();
char getFileSize(char filName[]);
FATFS myFatFS;
FIL fileObj;

SemaphoreHandle_t SD_Semaphore;

#endif /* SDCARD_H_ */
