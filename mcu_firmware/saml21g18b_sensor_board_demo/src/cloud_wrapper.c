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
#include "driver/include/m2m_wifi.h"
#include "main.h"
#include "socket/include/socket.h"
#include "string.h"
#include "AWS_SDK/aws_iot_src/utils/aws_iot_log.h"
#include "AWS_SDK/aws_iot_src/utils/aws_iot_version.h"
#include "AWS_SDK/aws_iot_src/protocol/mqtt/aws_iot_mqtt_interface.h"
#include "winc15x0.h"
#include "cJSON.h"
#include "cloud_wrapper.h"
#include "iot_message.h"
#include "aws_iot_config.h"
#ifdef USE_ALI_CLOUD
#include "ali_iot_config.h"
#include "ali_cloud.h"
#endif

/**
 * @brief Default MQTT HOST URL is pulled from the aws_iot_config.h
 */
char HostAddress[255] = AWS_IOT_MQTT_HOST;

/**
 * @brief Default MQTT port is pulled from the aws_iot_config.h
 */
uint32_t port = AWS_IOT_MQTT_PORT;

#ifdef USE_AWS_CLOUD
MQTTConnectParams connectParams;
MQTTSubscribeParams subParams;
MQTTMessageParams Msg;
MQTTPublishParams Params;
#endif



/** Wi-Fi status variable. */
extern bool gbConnectedWifi,receivedTime;

static bool toggle = false;

extern char g_ali_product_key[];
extern char g_ali_device_name[];


#if USE_AWS_CLOUD
static void (*mqtt_subscribe_cb)(int topic_len, char* topic_name, int payload_len, char* payload);
#else if USE_ALI_CLOUD

#endif



static void disconnectCallbackHandler(void) {
	printf("MQTT Disconnect");
	IoT_Error_t rc = NONE_ERROR;
#ifdef USE_AWS_CLOUD
	if(aws_iot_is_autoreconnect_enabled()){
		printf("Auto Reconnect is enabled, Reconnecting attempt will start now");
		}else{
		printf("Auto Reconnect not enabled. Starting manual reconnect...");
		rc = aws_iot_mqtt_attempt_reconnect();
		if(RECONNECT_SUCCESSFUL == rc){
			printf("Manual Reconnect Successful");
			}else{
			printf("Manual Reconnect Failed - %d", rc);
		}
	}
#endif
}

#ifdef USE_AWS_CLOUD
static int32_t MQTTcallbackHandler(MQTTCallbackParams params) {

	printf("Subscribe callback");
	printf("%.*s\t%.*s",
	(int)params.TopicNameLen, params.pTopicName,
	(int)params.MessageParams.PayloadLen, (char*)params.MessageParams.pPayload);
	printf("\n\r");
	
	mqtt_subscribe_cb((int)params.TopicNameLen, params.pTopicName, (int)params.MessageParams.PayloadLen, (char*)params.MessageParams.pPayload);
	return 0;
}
#endif

static void jsonMessagePublish(char* channel, cJSON *message)
{
	IoT_Error_t rc = NONE_ERROR;
	char* out;
	out = cJSON_Print(message);
#ifdef USE_AWS_CLOUD
	Msg.qos = QOS_0;
	Msg.PayloadLen = strlen(out) + 1;
	Msg.pPayload = (void *)out ;
	Params.pTopic = (char*)channel;
	Params.MessageParams = Msg;

	rc = aws_iot_mqtt_publish(&Params);
	if (rc != NONE_ERROR)
	printf("aws_iot_mqtt_publish() error, rc = %d\n", rc);
#endif
	free(out);
	
}

Cloud_RC cloud_init()
{
	
#ifdef USE_ALI_CLOUD
	ali_iot_init();
	
#endif
	
	return 0;
}

Cloud_RC cloud_connect(char* hostname)
{
	IoT_Error_t rc = NONE_ERROR;

#ifdef USE_ALI_CLOUD
	rc = ali_iot_mqtt_connect(hostname);
#endif

#ifdef USE_AWS_CLOUD
	
	if (strstr(hostname,"amazonaws.com") == NULL)	// hostname do not have the correct AWS IoT EndPoint
	{
		printf("Error... Cannot reade the correct AWS IoT EndPoint from ECCX08, use default AWS IoT EndPoint...\r\n");
		memset(hostname, 0, sizeof(hostname));
		strcpy(hostname,AWS_IOT_MQTT_HOST);
	}
	printf("provisioning_get_hostname, hostname = %s\r\n", hostname);
	
	
	gethostbyname((uint8_t *)hostname);
	
	connectParams = MQTTConnectParamsDefault;
	subParams = MQTTSubscribeParamsDefault;
	Msg = MQTTMessageParamsDefault;
	Params = MQTTPublishParamsDefault;
			
	connectParams.KeepAliveInterval_sec = 10;
	connectParams.isCleansession = true;
	connectParams.MQTTVersion = MQTT_3_1_1;
#ifdef ECC508
	connectParams.pClientID = g_mqtt_client_id;
#else
	connectParams.pClientID = gAwsMqttClientId;
#endif
	connectParams.pHostURL = hostname;
	connectParams.port = port;
	connectParams.isWillMsgPresent = false;
	connectParams.pRootCALocation = NULL;
	connectParams.pDeviceCertLocation = NULL;
	connectParams.pDevicePrivateKeyLocation = NULL;
	connectParams.mqttCommandTimeout_ms = 5000;
	connectParams.tlsHandshakeTimeout_ms = 5000;
	connectParams.isSSLHostnameVerify = true; // ensure this is set to true for production
	connectParams.disconnectHandler = disconnectCallbackHandler;
			
	if(gbConnectedWifi && receivedTime)
	{
		//sslEnableCertExpirationCheck(0);
		printf("Connecting...");

		rc = aws_iot_mqtt_connect(&connectParams);
		if (NONE_ERROR != rc) {
			printf("Error(%d) connecting to %s:%d", rc, connectParams.pHostURL, connectParams.port);
			return rc;
		}
		/*
		* Enable Auto Reconnect functionality. Minimum and Maximum time of Exponential backoff are set in aws_iot_config.h
		*  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
		*  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
		*/
		rc = aws_iot_mqtt_autoreconnect_set_status(true);
		if (NONE_ERROR != rc) {
			printf("Unable to set Auto Reconnect to true - %d", rc);
			return rc;
		}

		
		return rc;

	}
#endif
	return rc;
}

Cloud_RC cloud_mqtt_yield(int timeout)
{
	IoT_Error_t rc = NONE_ERROR;
#ifdef USE_AWS_CLOUD
	rc = aws_iot_mqtt_yield(timeout);
#else if USE_ALI_CLOUD
	rc = ali_iot_mqtt_yield();
#endif
	return rc;

}

Cloud_RC cloud_mqtt_publish(char* channel, void* message, int msg_size)
{
	IoT_Error_t rc = NONE_ERROR;
	
#ifdef USE_AWS_CLOUD
	jsonMessagePublish(channel, message);
#else if USE_ALI_CLOUD
	ali_iot_mqtt_publish(channel, message, msg_size);
#endif
	
	return rc;
	
}

Cloud_RC cloud_mqtt_subscribe(char* channel, void* cb)
{
	IoT_Error_t rc = NONE_ERROR;
	
#ifdef USE_AWS_CLOUD
	mqtt_subscribe_cb = cb;
	subParams.mHandler = MQTTcallbackHandler;
	subParams.pTopic = (char*)channel;
	subParams.qos = QOS_0;

	if (NONE_ERROR == rc) {
		printf("Subscribing...");
		rc = aws_iot_mqtt_subscribe(&subParams);
		if (NONE_ERROR != rc) {
			printf("Error subscribing\r\n");
		}
	}
	printf("Subscription success\r\n");
#else if USE_ALI_CLOUD
	ali_iot_mqtt_subscribe(channel, cb);
	
#endif
	return rc;
	
}

Cloud_RC cloud_create_topic(char* full_path, char* device_type, char* device_id, char* topic_name)
{
	IoT_Error_t rc = NONE_ERROR;
#ifdef USE_AWS_CLOUD	
	sprintf(full_path, "winc1500Iot/%s/%s/%s\0", device_type, device_id, topic_name);
#else if USE_ALI_CLOUD
	if (strcmp(topic_name, PUBLISH_TOPIC) == 0)
		sprintf(full_path, "/sys/%s/%s/thing/event/property/post\0", g_ali_product_key, g_ali_device_name);
	else
		sprintf(full_path, "/sys/%s/%s/thing/service/property/set\0", g_ali_product_key, g_ali_device_name);
	
#endif
	
	return rc;
	
}

Cloud_RC cloud_create_search_topic(char* full_path, char* acct_uuid, char* topic_name)
{
	IoT_Error_t rc = NONE_ERROR;
	sprintf(full_path, "winc1500Iot/%s/%s\0",acct_uuid, topic_name);
	return rc;
	
}
