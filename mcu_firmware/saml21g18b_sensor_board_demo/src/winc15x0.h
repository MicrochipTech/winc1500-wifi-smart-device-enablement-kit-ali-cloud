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

#ifndef __WIFI_APP_H__
#define __WIFI_APP_H__
#include "datapoint.h"
#include "socket/include/m2m_socket_host_if.h"

#define SUBSCRIBE_TOPIC "dataControl"
#define PUBLISH_TOPIC	"dataReport"
#define SUBSCRIBE_SEARCH_TOPIC "search"
#define PUBLISH_SEARCH_RESP_TOPIC "searchResp"
#define DEV_SEARCH_SUBSCRIBE_TOPIC "winc1500Iot/search"

#if USE_WEP
#define AP_WLAN_AUTH           M2M_WIFI_SEC_WEP /* < Security manner */
#define MAIN_WLAN_WEP_KEY        "6174000000"     /* < Security Key in WEP Mode - in 10 or 26 bytes hex */
#define MAIN_WLAN_WEP_KEY_OFFSET 4                /* < Attach MAC addr after this offset in WEP key */
#define MAIN_WLAN_WEP_KEY_INDEX  (1)
#else
#define AP_WLAN_AUTH           M2M_WIFI_SEC_OPEN /* < Security manner */
#endif

#define HEX2ASCII(x) (((x) >= 10) ? (((x) - 10) + 'A') : ((x) + '0'))
#define MAIN_WLAN_SSID_OFFSET    14                     /* < Attach MAC addr after this offset in SSID */
#define AP_WLAN_SSID	"iGatewayF8F005F3659B"


#define AP_TCP_SERVER_PORT				8899
#define MQTT_CLIENT_ID_LEN				13
#define MQTT_CHANNEL_LEN				80

#ifdef ECC508
char g_mqtt_client_id[129];
char g_thing_name[129];
#endif

typedef enum
{
	APP_STA,
	APP_AP,
	APP_P2P,
} wifi_mode;

typedef enum 
{
	WIFI_TASK_IDLE,
	WIFI_TASK_SWITCHING_TO_STA,
	WIFI_TASK_CONNECT_CLOUD,
	WIFI_TASK_MQTT_SUBSCRIBE,
	WIFI_TASK_MQTT_RUNNING,
	WIFI_TASK_CONNECT_CLOUD_FINISH,
	WIFI_TASK_SWITCH_TO_AP,
	WIFI_TASK_SWITCHING_TO_AP,
	WIFI_TASK_AP_CONNECTED,
	WIFI_TASK_STA_DISCONNECTED,

	WIFI_TASK_MAX
} wifi_FSM_states;


char gAwsMqttClientId[MQTT_CLIENT_ID_LEN];
char gSubscribe_Channel[MQTT_CHANNEL_LEN];
char gPublish_Channel[MQTT_CHANNEL_LEN];
char gSearch_Channel[MQTT_CHANNEL_LEN];
char gSearchResp_Channel[MQTT_CHANNEL_LEN];


int wifiInit(void);
int wifiTaskExecute(void);
void detectWiFiMode(void);
void setWiFiStates(wifi_FSM_states state);
wifi_FSM_states getWiFiStates(void);
wifi_mode getWiFiMode(void);

#endif /*__WIFI_APP_H__*/
