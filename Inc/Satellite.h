/*
 * Satellite.h
 *
 *  Created on: May 26, 2019
 *      Author: infl3
 */

#ifndef SATELLITE_H_
#define SATELLITE_H_
#include "stm32f4xx_hal.h"
void CAN_Config_Filter(CAN_HandleTypeDef hcan);
void CAN_Transmit(CAN_HandleTypeDef hcan, CAN_TxHeaderTypeDef Message_Header, uint8_t message[],uint32_t destination_mailbox);
void CAN_Receive(CAN_HandleTypeDef hcan1,CAN_RxHeaderTypeDef Message_Header, uint8_t message[]);
int twosCompliment(volatile int data);
void floatToString(double number, char buffer[]);
int intToString(int intVal, char * buffer, int len,int negative);

#endif /* SATELLITE_H_ */
