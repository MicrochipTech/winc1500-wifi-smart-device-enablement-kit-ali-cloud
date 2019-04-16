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
#include "motion_sensor.h"
#include "conf_sensor.h"
#include "bhi160.h"
#include "rtc.h"


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

unsigned char gu8UpdateFlag = 1;

void (*rotation_vector_update_cb)(device_rotation_vector_t, unsigned char);



void motion_sensor_data_init()
{
	/*
	environment_data_t environment_data;
	
	get_env_sensor_data_from_chip(&environment_data);
	
	gi16Disp_temperature = environment_data.temperature;
	gu8Disp_humidity = environment_data.humidity;
	gu32Disp_uv	= environment_data.uv;
	gu16Disp_pressure = environment_data.pressure;
	*/
	return;
}

void set_motion_sensor_update_timer(unsigned char time)	// arg time is in second unit
{
	rotation_data_timer = time; // 5 second timeout to update 
}

void motion_sensor_execute()
{
	
	if (rotation_data_timer > 0)
	{
		DBG_LOG("\r\nprocess motion\r\n");
		process_motion_sensor_data();
		DBG_LOG("rotation data = %d, %d, %d, %d\r\n", quaternion_data.w, quaternion_data.x, quaternion_data.y, quaternion_data.z);
		
		rotation_vector_update_cb(quaternion_data, rotation_data_timer);
	}
}

void register_rotation_vector_udpate_callback_handler(void* cb)
{
	rotation_vector_update_cb = cb;
}