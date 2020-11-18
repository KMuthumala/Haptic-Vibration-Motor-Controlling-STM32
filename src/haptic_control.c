#include "haptic_control.h"

#include <stdio.h>
#include <stdlib.h>


int Haptic_init(I2C_HandleTypeDef *i2c) {
	 device_address=DRV2605_ADDR;
	hi2c=i2c;
  uint8_t id = readRegister8(DRV2605_REG_STATUS);

  writeRegister8(DRV2605_REG_MODE, 0x00); // out of standby

  writeRegister8(DRV2605_REG_RTPIN, 0x00); // no real-time-playback

  writeRegister8(DRV2605_REG_WAVESEQ1, 1); // strong click
  writeRegister8(DRV2605_REG_WAVESEQ2, 0); // end sequence

  writeRegister8(DRV2605_REG_OVERDRIVE, 0); // no overdrive

  writeRegister8(DRV2605_REG_SUSTAINPOS, 0);
  writeRegister8(DRV2605_REG_SUSTAINNEG, 0);
  writeRegister8(DRV2605_REG_BREAK, 0);
  writeRegister8(DRV2605_REG_AUDIOMAX, 0x64);

  // ERM open loop

  // turn off N_ERM_LRA
  writeRegister8(DRV2605_REG_FEEDBACK, readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
  // turn on ERM_OPEN_LOOP
  writeRegister8(DRV2605_REG_CONTROL3, readRegister8(DRV2605_REG_CONTROL3) | 0x20);

  return 1;
}

uint8_t readRegister8(uint8_t reg) {
  uint8_t x;

  uint8_t array[2];
  array[0]=reg;
  HAL_I2C_Master_Transmit(hi2c,device_address,array[1],1,10);
  HAL_I2C_Master_Receive(hi2c,device_address,&array[1],1,10);
  x=array[1];

  return x;
}

void writeRegister8(uint8_t reg, uint8_t val) {
	uint8_t array[2];
	array[0]=reg;
	array[1]=val;
	HAL_I2C_Master_Transmit(hi2c,device_address,array,2,10);
}

uint16_t addressFinder(){
  uint16_t a=0;
	while(HAL_I2C_IsDeviceReady(hi2c,a, 3, 10) != HAL_OK){
	  //sprintf(msg, "\r\n%d", a);
	  //HAL_UART_Transmit(&huart2,(uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
		a+=1;
		if(a==256){
			return 0;
		}
	}
	return a;
}

void setWaveform(uint8_t slot, uint8_t w) {
	writeRegister8(DRV2605_REG_WAVESEQ1+slot, w);
}

void selectLibrary(uint8_t lib) {
  writeRegister8(DRV2605_REG_LIBRARY, lib);
}

void go() {
  writeRegister8(DRV2605_REG_GO, 1);
}

void stop() {
  writeRegister8(DRV2605_REG_GO, 0);
}

void setMode(uint8_t mode) {
  writeRegister8(DRV2605_REG_MODE, mode);
}

void setRealtimeValue(uint8_t rtp) {
  writeRegister8(DRV2605_REG_RTPIN, rtp);
}

void playEffect(MotorType motor_no,uint8_t effect_no){


	switch(motor_no){

		case MOTOR_1:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
			   break;

		case MOTOR_2:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
			   break;

		case MOTOR_3:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
			   break;

		case MOTOR_4:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
			   break;

		default:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
			   break;


	}
	// set the effect to play
	setWaveform(0, effect_no);  // play effect
	setWaveform(1, 0);       // end waveform

	// play the effect!
	go();

}


