/*
 * hts221.h
 *
 *  Created on: Dec 25, 2019
 *      Author: niket
 */

#ifndef SRC_HTS221_HTS221_H_
#define SRC_HTS221_HTS221_H_

//#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal.h"

#define HTS221_8BIT_DEVICE_ID 0xBE

typedef enum {
	HTS221_WHO_AM_I = 0x0f,
	HTS221_AV_CONF = 0x10,
	HTS221_CTRL_REG1 = 0x20,
	HTS221_CTRL_REG2 = 0x21,
	HTS221_CTRL_REG3 = 0x22,
	HTS221_STATUS_REG = 0x27,
	HTS221_HUMIDITY_OUT_L = 0x28,
	HTS221_HUMIDITY_OUT_H = 0x29,
	HTS221_TEMP_OUT_L = 0x2A,
	HTS221_TEMP_OUT_H = 0x2B,

	HTS221_T0_DEGC_X8 = 0x32,
	HTS221_T1_DEGC_X8 = 0x33,
	HTS221_T1_T0_MSB = 0x35,

	HTS221_T0_OUT_L = 0x3C,
	HTS221_T0_OUT_H = 0x3D,
	HTS221_T1_OUT_L = 0x3E,
	HTS221_T1_OUT_H = 0x3F,
} HTS221_RegisterMap_e;


void HTS221_init(I2C_HandleTypeDef *hi2c);
double HTS221_read_temperature(I2C_HandleTypeDef *hi2c);

// Helper functions
uint8_t HTS221_read_register_map(I2C_HandleTypeDef *hi2c,
		HTS221_RegisterMap_e register_map);


#endif /* SRC_HTS221_HTS221_H_ */
