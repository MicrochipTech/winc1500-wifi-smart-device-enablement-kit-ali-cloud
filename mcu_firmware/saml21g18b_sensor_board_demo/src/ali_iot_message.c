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
#include "iot_message.h"
#include "cJSON.h"
#include "datapoint.h"
#include "string.h"
#include "main.h"
#include "ali_iot_message.h"

extern uint8 gUuid[AWS_COGNITO_UUID_LEN];
char params_name[ALI_PARAMS_MAX][ALI_PARAMS_NAME_MAX_LEN] =
{
	ALI_ATTR_PARAMS_Light_SWITCH_NAME
};



Ali_Iot_Message_RC ali_iot_message_pack(char *json_str, Ali_Iot_Property property, ali_params params[], int params_size)
{
	int i, tmp = 0;
	
	tmp = sprintf(json_str, "%s", "{\"id\":\"1\",\"version\":\"1.0\",\"params\":{");
	json_str += tmp;
	
	for (i = 0; i< params_size; i++)
	{
		tmp = sprintf(json_str, "\"%s\":", params[i].parameter);
		json_str += tmp;

		switch (params[i].type)
		{
			case ALI_TYPE_INT:
				tmp = sprintf(json_str, "%d,", params[i].val);
				break;
			case ALI_TYPE_DOUBLE:
				tmp = sprintf(json_str, "%d.%02d,", params[i].val/100, params[i].val%100);
				break;
			case ALI_TYPE_STR:
				tmp = sprintf(json_str, "%s,", params[i].str);
				break;
		}
	
		json_str += tmp;
	}
	json_str --;
	tmp = sprintf(json_str, "},\"method\":\"");
	json_str += tmp;
	tmp = sprintf(json_str, "%s\"}", ALI_ATTR_PROPERTY_POST_NAME);
	json_str += tmp;
	
	return ALI_IOT_MSG_RC_SUCCESS;
}

Ali_Iot_Message_RC ali_iot_message_parser(char* json_msg, Ali_Iot_Property* property, ali_params info[], int* params_size)
{
	cJSON *json;
	cJSON *json_command, *json_subcommand;
	cJSON *json_params;
	
	*params_size = 0;
	json=cJSON_Parse(json_msg);
	
	if (!json) {
		printf("Error when decode json: [%s]\n",cJSON_GetErrorPtr());
		return MSG_CMD_UNKNOWN;
	}
	
	json_command = cJSON_GetObjectItem(json,ALI_ATTR_PROPERTY_NAME);
	
	if (0 == strcmp(json_command->valuestring,ALI_ATTR_PROPERTY_SET_NAME))
		property = ALI_PROPERTY_SET;
	else if (0 == strcmp(json_command->valuestring,ALI_ATTR_PROPERTY_POST_NAME))
		property = ALI_PROPERTY_POST;	
	else 
		property = ALI_PROPERTY_UNKNOWN;
		
	
	json_params = cJSON_GetObjectItem(json,ALI_ATTR_PARAMS_NAME);
	for (int i=0; i<ALI_PARAMS_MAX; i++)
	{
		json_command = cJSON_GetObjectItem(json_params,params_name[i]);
		
		if(NULL != json_command)
		{
			strcpy(info[i].parameter,params_name[i]);
			info[i].val = json_command->valueint;
			
			// To Do: add handling string params if needed
			
			(*params_size) ++;
		}
	}
	
	cJSON_Delete(json);	
	
	return ALI_IOT_MSG_RC_SUCCESS;
}		

