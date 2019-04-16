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

#ifndef __ALI_IOT_MESSAGE_H__
#define __ALI_IOT_MESSAGE_H__

#define ALI_ATTR_PROPERTY_NAME	"method"
#define ALI_ATTR_PROPERTY_SET_NAME	"thing.service.property.set"
#define ALI_ATTR_PROPERTY_POST_NAME	"thing.event.property.post"
#define ALI_ATTR_PARAMS_NAME	"params"
#define ALI_ATTR_PARAMS_Light_SWITCH_NAME	"LightSwitch"
#define ALI_ATTR_PARAMS_BRIGHTNESS_NAME		"Brightness"
#define ALI_ATTR_PARAMS_CUR_TEMPERATURE_NAME	"CurrentTemperature"
#define ALI_ATTR_PARAMS_REL_HUMIDITY_NAME	"RelativeHumidity"
#define ALI_ATTR_ID_NAME	"id"
#define ALI_ATTR_VERSION_NAME	"version"

#define ALI_PARAMS_MAX_SIZE	5
#define ALI_PARAMS_NAME_MAX_LEN	20
#define ALI_PARAMS_VAL_STR_MAX_LEN	30

typedef enum Ali_Iot_Params
{
	ALI_PARAMS_Light_Switch = 0,
	ALI_PARAMS_MAX
};

typedef enum Ali_Iot_Type
{
	ALI_TYPE_INT = 0,
	ALI_TYPE_DOUBLE = 1,			//treat double as int, round to 2 digit after decimal, because not supporting float. Float can be support by disable "Use sized optimized library" in ARM/ GNU Linker 
	ALI_TYPE_STR = 2,
}Ali_Iot_Type;

typedef struct ali_params
{
	char parameter[ALI_PARAMS_NAME_MAX_LEN];
	//int	val;
	int val;
	char str[ALI_PARAMS_VAL_STR_MAX_LEN];
	int type;
} ali_params;




typedef enum Ali_Iot_Property
{
	ALI_PROPERTY_SET = 0,
	ALI_PROPERTY_POST = 1,
	ALI_PROPERTY_UNKNOWN 
}Ali_Iot_Property;

typedef enum {
	ALI_IOT_MSG_RC_NONE_ERROR = 0,
	ALI_IOT_MSG_RC_SUCCESS = 0,
	ALI_IOT_MSG_RC_FAIL= 1,
	ALI_IOT_MSG_RC_DECODE_FAIL= 2,
}Ali_Iot_Message_RC;


Ali_Iot_Message_RC ali_iot_message_parser(char* json_msg, Ali_Iot_Property* property, ali_params info[], int* params_size);
Ali_Iot_Message_RC ali_iot_message_pack(char *json_str, Ali_Iot_Property property, ali_params params[], int params_size);
#endif /*__ALI_IOT_MESSAGE_H__*/
