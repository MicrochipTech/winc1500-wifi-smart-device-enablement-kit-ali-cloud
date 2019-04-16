/**
 * \file
 *
 * \brief sensor board demo, connect to Ali Cloud.
 *
 * Copyright (c) 2017-2018 Microchip Technology Inc. and its subsidiaries. 
 * You may use this software and any derivatives exclusively with Microchip 
 * products.
 *
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <asf.h>
#include "wearable.h"
#include "env_sensor.h"
#include "bme280\bme280_support.h"
#include "conf_sensor.h"
#include "veml60xx\veml60xx.h"
#include "bhi160.h"
#include "bhi160\bhy_uc_driver.h"

#define TEMP_DISP_COUNTER	1	// update when same value is get 1 time
#define HUM_DISP_COUNTER	1
#define UV_DISP_COUNTER 1
#define PRESSURE_DISP_COUNTER 1

int16_t gi16Disp_temperature;
uint8_t gu8Disp_humidity;
uint32_t gu32Disp_uv;
uint16_t gu16Disp_pressure;
//environment data to BLE
environment_data_t environ_data;

static void (*env_sensor_update_cb)(environment_data_t, unsigned char);




static void get_env_sensor_data_from_chip(environment_data_t *env_data)
{
	s32 env_temperature;
	u32 env_pressure;
	u32 env_humidity;
	uint16_t als_data;
	
	bme280_set_power_mode(BME280_FORCED_MODE);
	
	if(ERROR == bme280_read_pressure_temperature_humidity(&env_pressure, &env_temperature, &env_humidity)){
		//error
		DBG_LOG("Reading BME280 has failed");
	}
	if(STATUS_OK != veml60xx_read_alsdata(&als_data)){
		//error
		DBG_LOG("Reading VEML60xx has failed");
	}
	
	/* Temperature: No data processing required for Temperature data. Data with resolution 0.01(x100) directly txd*/
	env_data->temperature = (int16_t)env_temperature;
	/* Pressure: Returns pressure in Pa as unsigned 32 bit integer. Output value of ?6386?equals 96386 Pa = 963.86 hPa*/
	env_data->pressure = (uint16_t)(env_pressure / 100);
	/*ALS: lx/step = 0.07 */
	///env_data->uv = ((uint32_t)(als_data) * 7000 / 100000));
	env_data->uv = ((uint32_t)als_data * 7000);
	//printf("\r\ntest=%d\r\n",(uint32_t) als_data);
	/* Humidity: An output value of 42313 represents 42313 / 1024 = 41.321 %rH*/
	env_data->humidity = (uint8_t)(env_humidity / 1024);
}

void env_sensor_data_init()
{
	environment_data_t environment_data;
	
	get_env_sensor_data_from_chip(&environment_data);
	
	gi16Disp_temperature = environment_data.temperature;
	gu8Disp_humidity = environment_data.humidity;
	gu32Disp_uv	= environment_data.uv;
	gu16Disp_pressure = environment_data.pressure;
	
	return;
}



void env_sensor_execute()
{
	static int16_t pre_temp = 0;
	static uint8_t pre_hum = 0;
	static uint32_t pre_uv = 0;
	static uint16_t pre_pressure = 0;
	
	static int temp_cnt;
	static int hum_cnt;
	static int uv_cnt;
	static int pressure_cnt;
	
	static int uv_not_equal_cnt;
	
	unsigned char updateFlag = 0;
	
	environment_data_t environment_data;
	get_env_sensor_data_from_chip(&environment_data);
	printf("DBG2: temperature = %d, humidity = %d, uv = %lu, pressure = %d\r\n", environment_data.temperature, environment_data.humidity, environment_data.uv, environment_data.pressure);
	
	// check temperature
	if (pre_temp != environment_data.temperature)
	{
		temp_cnt = 0;
		pre_temp = environment_data.temperature;
	}
	else
		temp_cnt++;
	
	if ((temp_cnt >= TEMP_DISP_COUNTER) && gi16Disp_temperature!= pre_temp)
	{
		gi16Disp_temperature = pre_temp;
		updateFlag |= TEMP_UPDATE_BIT; 
	}
	
	// check humidity
	if (pre_hum != environment_data.humidity)
	{
		hum_cnt = 0;
		pre_hum = environment_data.humidity;
	}
	else
		hum_cnt++;
	
	if ((hum_cnt >= HUM_DISP_COUNTER) && gu8Disp_humidity!= pre_hum)
	{
		gu8Disp_humidity = pre_hum;
		updateFlag |= HUM_UPDATE_BIT; 
	}
	
	// check uv
	if (pre_uv != environment_data.uv)
	{
		uv_cnt = 0;
		pre_uv = environment_data.uv;
		uv_not_equal_cnt++;
	}
	else
	{
		uv_cnt++;
		uv_not_equal_cnt = 0;
	}
	if (((uv_cnt >= UV_DISP_COUNTER) && gu32Disp_uv!= pre_uv) || (uv_not_equal_cnt >1))
	//if (((uv_cnt >= UV_DISP_COUNTER) && gu32Disp_uv!= pre_uv))
	{
		gu32Disp_uv = pre_uv;
		updateFlag |= UV_UPDATE_BIT; 
		uv_not_equal_cnt = 0;
	}
	
	// check pressure
	if (pre_pressure != environment_data.pressure)
	{
		pressure_cnt = 0;
		pre_pressure = environment_data.pressure;
	}
	else
		pressure_cnt++;
	
	if ((pressure_cnt >= PRESSURE_DISP_COUNTER) && gu16Disp_pressure!= pre_pressure)
	{
		gu16Disp_pressure = pre_pressure;
		updateFlag |= PRESSURE_UPDATE_BIT; 
	}
		
	if (updateFlag > 0)
		env_sensor_update_cb(environment_data, updateFlag);
	
}

void get_env_sensor_data_for_display(environment_data_t *env_data)
{
	env_data->temperature = gi16Disp_temperature;
	env_data->humidity = gu8Disp_humidity;
	env_data->uv = gu32Disp_uv;
	env_data->pressure = gu16Disp_pressure;
	
}

void register_env_sensor_udpate_callback_handler(void* cb)
{
	env_sensor_update_cb = cb;
}