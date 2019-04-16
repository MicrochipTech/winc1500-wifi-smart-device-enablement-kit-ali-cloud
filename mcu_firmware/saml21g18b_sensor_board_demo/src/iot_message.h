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
#ifndef __IOT_MESSAGE_H__
#define __IOT_MESSAGE_H__
#include "cJSON.h"
#include "datapoint.h"

#define ATTR_COMMAND_NAME	"command"
#define ATTR_SUBCOMMAND_NAME	"subcommand"
#define ATTR_DEVNAME_NAME	"devName"
#define ATTR_DEVTYPE_NAME	"devType"
#define ATTR_DATATYPE_NAME	"dataType"
#define ATTR_VALUE_NAME	"value"
#define ATTR_MAC_ADDR_NAME	"macAddr"
#define ATTR_INFO_NAME	"info"
#define ATTR_UUID_NAME	"uuid"

#define CMD_SEARCH	"search"
#define CMD_GET	"get"
#define CMD_UPDATE	"update"
#define CMD_SEARCHRESP	"searchResp"
#define CMD_REPORTALLINFO	"reportAllInfo"
#define CMD_REPORTINFO	"reportInfo"
#define CMD_REPORTDISCONNECT	"reportDisconnect"
#define CMD_CONTROL	"control"
#define CMD_ADD_NODE	"addNode"
#define CMD_GET_3D_PLOT_DATA	"get3dPlotData"

#define TEMP_DATATYPE_NAME	"temp"
#define HUM_DATATYPE_NAME	"hum"
#define UV_DATATYPE_NAME	"uv"
#define PRESSURE_DATATYPE_NAME	"pressure"
#define LED1_DATATYPE_NAME	"led1"

#define ROTATION_W_DATATYPE_NAME	"r_w_cor"
#define ROTATION_X_DATATYPE_NAME	"r_x_cor"
#define ROTATION_Y_DATATYPE_NAME	"r_y_cor"
#define ROTATION_Z_DATATYPE_NAME	"r_z_cor"


typedef enum Iot_Msg_Command
{
	MSG_CMD_SEARCH = 0,
	MSG_CMD_GET = 1,
	MSG_CMD_UPDATE = 2,
	MSG_CMD_SEARCHRESP = 3,
	MSG_CMD_REPORTALLINFO = 4,
	MSG_CMD_REPORTINFO = 0,
	MSG_CMD_REPORTDISCONNECT = 5,
	MSG_CMD_CONTROL = 6,
	MSG_SUBCMD_ADDNODE = 7,
	MSG_SUBCMD_GET_3D_PLOT_DATA = 8,
	MSG_CMD_UNKNOWN 
}Iot_Msg_Command;

typedef enum {
	IOT_MSG_RC_NONE_ERROR = 0,
	IOT_MSG_RC_SUCCESS = 0,
	IOT_MSG_RC_FAIL= 1,
	IOT_MSG_RC_DECODE_FAIL= 2,
}Iot_Message_RC;

int iot_message_parser(char* json_msg, char* cmd, char* mac_addr, char* info);
int iot_message_get_info_count(char* json_msg);
Iot_Message_RC iot_message_parser_info_data(char* json_msg, int array_idx, char* dataType, int* value);
Iot_Message_RC iot_message_parser_addnode_cmd_msg(char* json_msg, char* node_mac_addr);
//Iot_Message_RC iot_message_parser_control_cmd_msg(char* json_msg);
Iot_Msg_Command iot_message_parser_cmd_type(char* json_msg);
cJSON* iot_message_searchResp(char* device_type, char* mac_addr);
cJSON* iot_message_searchResp_with_temp_uv(char* device_type, char* mac_addr, int temp, int uv );
cJSON* iot_message_reportAllInfo(char* device_type, char* mac_addr, int online_dev_num, NodeInfo endnode_info[]);
cJSON* iot_message_reportInfo(char* device_type, char* mac_addr, int report_data_num, NodeInfo data_info[]);
cJSON* iot_message_reportDisconnect(int report_dev_num, NodeInfo endnode_info[]);

#endif /*__WIFI_APP_H__*/
