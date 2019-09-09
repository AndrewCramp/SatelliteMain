#include "Satellite.h"
#include "bn0.h"
#include "SDCard.h"
#include "main.h"
#include <stdio.h>
void CAN_Config_Filter(CAN_HandleTypeDef hcan){
	  /* USER CODE BEGIN CAN1_Init 2*/
	  CAN_FilterTypeDef CAN_Filter;
	  CAN_Filter.FilterBank = 14;
	  CAN_Filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	  CAN_Filter.FilterMode = CAN_FILTERMODE_IDMASK;
	  CAN_Filter.FilterScale = CAN_FILTERSCALE_32BIT;
	  CAN_Filter.FilterMaskIdHigh = 0x0000;
	  CAN_Filter.FilterMaskIdLow = 0x0000;
	  CAN_Filter.FilterIdHigh = 0x0000;
	  CAN_Filter.FilterIdLow = 0x0000;
	  CAN_Filter.FilterActivation = ENABLE;
	  if(HAL_CAN_ConfigFilter(&hcan, &CAN_Filter) != HAL_OK){
		  Error_Handler();
	  }
}

void CAN_Transmit(CAN_HandleTypeDef hcan, CAN_TxHeaderTypeDef Message_Header, uint8_t message[],uint32_t destination_mailbox){
	 if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 0){
		 if(HAL_CAN_AddTxMessage(&hcan, &Message_Header,message,&destination_mailbox) != HAL_OK){
			 //error handling
		 }
	 }
}

void CAN_Receive(CAN_HandleTypeDef hcan,CAN_RxHeaderTypeDef Message_Header, uint8_t message[]){
	  if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) != 0){
		  if(HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0, &Message_Header,message) != HAL_OK){
			  //Handle errors
		  }
	  }
}



int twosCompliment(volatile int data){
	if(data > 32768){
		data = data ^ 0b1111111111111111;
		data = data+1;
		data = data * -1;
	}
	return data;
}

int intToString(int intVal, char * buffer,int len,int negative){
	int i = 0;
	char temp[20];
	if(intVal < 0){
		negative = 1;
		intVal*=-1;
	}
	while(intVal  > 0){
		if(intVal>=0){
			temp[i++] = (intVal%10) + '0';
			intVal = intVal/10;
		}
	}
	while(i < len){
			temp[i++] = '0';
	}
	if(negative){
			temp[i++] = '-';
	}
	for(int j = 0; j < i;j++){
		buffer[i-1-j] = temp[j];
	}
	buffer[i] = '\0';
	return i;
}
void floatToString(double number, char buffer[]){
	int intVal = (int)number;
	int negative = 0;
	if(number < 0)
		negative = 1;
	int place = intToString(intVal,buffer,1,negative);
	char temp[20];
	buffer[place] = '.';
	number -= intVal;
	if(number>0){
		number*=100000.0;
	}else{
		number*=-100000.0;
	}
	int place2 = intToString((int)number, temp,5,0);
	int i = place+1;
	for(i = place+1; i < place+2+place2; i++){
		buffer[i] = temp[i-place-1];
	}
}

void logGravData(){
	volatile vector grav;
	calib_Status calib;
	calib = getCalibrationStatus();
	grav = getGravVector();
	float gravArray[5] = {grav.X,grav.Y,grav.Z,calib.ACC,bnO_getTemp()};
	formattedWriteFloat("GravData.csv",gravArray,5);
}

void logAccelData(){
	volatile vector accel;
	calib_Status calib;
	calib = getCalibrationStatus();
	accel = getAccelVector();
	float gravArray[5] = {accel.X,accel.Y,accel.Z,calib.ACC,bnO_getTemp()};
	formattedWriteFloat("AccelData.csv",gravArray,5);
}

void logGyroData(){
	volatile vector accel;
	calib_Status calib;
	calib = getCalibrationStatus();
	accel = getAccelVector();
	float gravArray[5] = {accel.X,accel.Y,accel.Z,calib.ACC,bnO_getTemp()};
	formattedWriteFloat("AccelData.csv",gravArray,5);
}
