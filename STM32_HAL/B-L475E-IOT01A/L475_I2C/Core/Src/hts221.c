/*
 * hts221.c
 *
 *  Created on: Dec 25, 2019
 *      Author: niket
 */
#include "hts221.h"
#define HTS221_DEBUG 1

#if HTS221_DEBUG
#include <stdio.h>
#define debugPrintf printf
#else
#define debugPrintf(fmt, ...)
#endif

#define HTS221_TIMEOUT 1000

// Static function declarations
static void HTS221_write_register(I2C_HandleTypeDef *hi2c,
		HTS221_RegisterMap_e register_map, uint8_t data);
static void HTS221_power_on(I2C_HandleTypeDef *hi2c);
static void HTS221_init_defaults(I2C_HandleTypeDef *hi2c);

// Static variables
static int16_t t0_out, t1_out;
static double t0_degC, t1_degC;

/**
 *
 * Main Functions
 *
 */
void HTS221_init(I2C_HandleTypeDef *hi2c) {
	HTS221_power_on(hi2c);
	HTS221_init_defaults(hi2c);
}

double HTS221_read_temperature(I2C_HandleTypeDef *hi2c) {
	uint8_t tlow = HTS221_read_register_map(hi2c, HTS221_TEMP_OUT_L);
	uint8_t thigh = HTS221_read_register_map(hi2c, HTS221_TEMP_OUT_H);
	int16_t t_out = (thigh << 8) | tlow;

	debugPrintf("t_out: %x %d\r\n", t_out, t_out);
	debugPrintf("t0_out: %x %d\r\n", t0_out, t0_out);
	debugPrintf("t1_out: %x %d\r\n", t1_out, t1_out);

	debugPrintf("t0_degC: %f\r\n", t0_degC);
	debugPrintf("t1_degC: %f\r\n", t1_degC);

	double t_degC = ((t_out - t0_out) * (t1_degC - t0_degC) / (t1_out - t0_out))
			+ t0_degC;
	debugPrintf("Data: %f\r\n", t_degC);
	return t_degC;
}

/**
 *
 * Helper Functions
 *
 */
uint8_t HTS221_read_register_map(I2C_HandleTypeDef *hi2c,
		HTS221_RegisterMap_e register_map) {

	uint8_t rData;
	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Read(hi2c, HTS221_8BIT_DEVICE_ID | 0x01,
			(uint8_t) register_map, I2C_MEMADD_SIZE_8BIT, &rData,
			1 * sizeof(uint8_t), HTS221_TIMEOUT);
	debugPrintf("[Read] RegisterMap: 0x%x Status: %d, rData: 0x%x\r\n",
			register_map, status, rData);

	return rData;
}

/**
 *
 * Static functions
 *
 */
static void HTS221_write_register(I2C_HandleTypeDef *hi2c,
		HTS221_RegisterMap_e register_map, uint8_t data) {

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, HTS221_8BIT_DEVICE_ID,
			register_map, I2C_MEMADD_SIZE_8BIT, &data, 1 * sizeof(uint8_t),
			HTS221_TIMEOUT);
	debugPrintf("[Write] RegisterMap: 0x%x Status: %d, wData: 0x%x\r\n",
			register_map, status, data);
}

static void HTS221_power_on(I2C_HandleTypeDef *hi2c) {

	debugPrintf("HTS221_power_on: Start\r\n");

	uint8_t wData;

	HTS221_read_register_map(hi2c, HTS221_WHO_AM_I);

	// Set it OFF -> HTS221_CTRL_REG1
	wData = 0x00;
	HTS221_write_register(hi2c, HTS221_CTRL_REG1, wData);

	// Set the AV_CONF register to default value
	HTS221_write_register(hi2c, HTS221_AV_CONF, 0x1B);

	// Set the power on
	wData = 0x00; // reset
	wData |= (1 << 7) | (1 << 2) | (1 << 0); // TODO, Make this dynamic later
	HTS221_write_register(hi2c, HTS221_CTRL_REG1, wData);

	HTS221_read_register_map(hi2c, HTS221_CTRL_REG1);
	HTS221_read_register_map(hi2c, HTS221_AV_CONF);

	debugPrintf("HTS221_power_on: Done\r\n");
}

static void HTS221_init_defaults(I2C_HandleTypeDef *hi2c) {
	uint8_t t1_t0_msb = HTS221_read_register_map(hi2c, HTS221_T1_T0_MSB);
	uint8_t t0_deg_x8 = HTS221_read_register_map(hi2c, HTS221_T0_DEGC_X8);
	uint8_t t1_deg_x8 = HTS221_read_register_map(hi2c, HTS221_T1_DEGC_X8);

	uint8_t t0_l = HTS221_read_register_map(hi2c, HTS221_T0_OUT_L);
	uint8_t t0_h = HTS221_read_register_map(hi2c, HTS221_T0_OUT_H);
	t0_out = (t0_h << 8) | t0_l;
	debugPrintf("t0_out: 0x%x %d\r\n", t0_out, t0_out);

	uint8_t t1_l = HTS221_read_register_map(hi2c, HTS221_T1_OUT_L);
	uint8_t t1_h = HTS221_read_register_map(hi2c, HTS221_T1_OUT_H);
	t1_out = (t1_h << 8) | t1_l;
	debugPrintf("t1_out: 0x%x %d\r\n", t1_out, t1_out);

	t0_degC = (t0_deg_x8 + (1 << 8) * (t1_t0_msb & 0x03)) / 8.0;
	t1_degC = (t1_deg_x8 + (1 << 8) * ((t1_t0_msb >> 2) & 0x03)) / 8.0;
	debugPrintf("t0_degC: %f\r\n", t0_degC);
	debugPrintf("t1_degC: %f\r\n", t1_degC);
}

