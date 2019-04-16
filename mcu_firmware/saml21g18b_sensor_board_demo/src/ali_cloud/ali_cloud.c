/*
 * veml6080.c
 *
 * Created: 11/17/2015 4:21:29 PM
 *  Author: gracy.angela
 */ 

#include <asf.h>
#include "ali_cloud.h"
#include "ali_iot_config.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"
#include "MQTTClient/Wrapper/mqtt.h"
#include "ecc_provisioning_task.h"
#include "main.h"
#include "rtc.h"

/** Using IP address. */
#define IPV4_BYTE(val, index)           ((val >> (index * 8)) & 0xFF)
#define DNS_REQUEST   25
#define ALI_CONNECT_ERROR -1

static int8_t state =  -1, err = 0;
/** IP address of host. */
uint32_t gu32HostIp = 0;
/** Get host IP status variable. */
static bool gbHostIpByName = false;


static unsigned char mqtt_read_buffer[MAIN_MQTT_BUFFER_SIZE];
static unsigned char mqtt_send_buffer[MAIN_MQTT_BUFFER_SIZE];
/* Instance of MQTT service. */
static struct mqtt_module mqtt_inst;
bool g_connect_ali_cloud = false;

const char productKey[] = ALI_PRODUCT_KEY;
const char deviceName[] = ALI_DEVICE_NAME;
static char clientId[CLIENT_ID_LENGTH];
static uint8_t hashMsg[HASH_MSG_LENGTH];
char mqttUsername[50];
char mqttPassword[32 * 2 + 1];//456
char mqttCid[MQTT_CID_LENGTH];
char mqttPublishTopic[MQTT_TOPIC_LENGTH];
char mqttSubscribeTopic[MQTT_TOPIC_LENGTH];



static void (*mqtt_subscribe_cb)(int topic_len, char* topic_name, int payload_len, char* payload);

// To do: get a correct timestamp and correct JWT
static void updateJWT(uint32_t timestamp)
{

	sprintf(clientId, "%s&%s_aliyun-iot-device-sdk-js", g_ali_product_key, g_ali_device_name);
	sprintf(mqttCid, "%s|securemode=3,signmethod=hmacsha256,timestamp=%d|", clientId, timestamp);
	printf("timestamp = %d\r\n",timestamp);
	sprintf(mqttPublishTopic, "/sys/%s/%s/thing/event/property/post", g_ali_product_key, g_ali_device_name);
	sprintf(mqttSubscribeTopic, "/sys/%s/%s/thing/service/property/set", g_ali_product_key, g_ali_device_name);
	sprintf(mqttUsername, "%s&%s", g_ali_device_name, g_ali_product_key);
	sprintf(hashMsg, "clientId%sdeviceName%sproductKey%stimestamp%d", clientId, g_ali_device_name, g_ali_product_key, timestamp);
	size_t hashMsgLen = strlen(hashMsg);
	cryptoauthlib_compute_hmacsha256(hashMsg, hashMsgLen, mqttPassword);
	
	DBG_LOG("\r\nMQTT: hashMsg=%s, hashMsglength=%d\r\n", hashMsg, hashMsgLen);
	DBG_LOG("MQTT: mqttCid=%s, mqttCidlength=%d\r\n", mqttCid,strlen(mqttCid));
	DBG_LOG("MQTT: mqttPublishTopic=%s\r\n", mqttPublishTopic);
	DBG_LOG("MQTT: mqttSubscribeTopic=%s\r\n", mqttSubscribeTopic);
}

void SubscribeHandler(MessageData *msgData)
{
	/* You received publish message which you had subscribed. */
	/* Print Topic and message */
	DBG_LOG("[SubscribeHandler] In\r\n");
	DBG_LOG("\r\n %.*s",msgData->topicName->lenstring.len,msgData->topicName->lenstring.data);
	DBG_LOG(" >> ");
	DBG_LOG("%.*s",msgData->message->payloadlen,(char *)msgData->message->payload);
	
	mqtt_subscribe_cb(msgData->topicName->lenstring.len, msgData->topicName->lenstring.data, msgData->message->payloadlen, msgData->message->payload);
}


/**
 * \brief Callback to get the MQTT status update.
 *
 * \param[in] conn_id instance id of connection which is being used.
 * \param[in] type type of MQTT notification. Possible types are:
 *  - [MQTT_CALLBACK_SOCK_CONNECTED](@ref MQTT_CALLBACK_SOCK_CONNECTED)
 *  - [MQTT_CALLBACK_CONNECTED](@ref MQTT_CALLBACK_CONNECTED)
 *  - [MQTT_CALLBACK_PUBLISHED](@ref MQTT_CALLBACK_PUBLISHED)
 *  - [MQTT_CALLBACK_SUBSCRIBED](@ref MQTT_CALLBACK_SUBSCRIBED)
 *  - [MQTT_CALLBACK_UNSUBSCRIBED](@ref MQTT_CALLBACK_UNSUBSCRIBED)
 *  - [MQTT_CALLBACK_DISCONNECTED](@ref MQTT_CALLBACK_DISCONNECTED)
 *  - [MQTT_CALLBACK_RECV_PUBLISH](@ref MQTT_CALLBACK_RECV_PUBLISH)
 * \param[in] data A structure contains notification informations. @ref mqtt_data
 */
static void ali_mqtt_callback(struct mqtt_module *module_inst, int type, union mqtt_data *data)
{
	switch (type) {
	case MQTT_CALLBACK_SOCK_CONNECTED:
	{
		/*
		 * If connecting to broker server is complete successfully, Start sending CONNECT message of MQTT.
		 * Or else retry to connect to broker server.
		 */
		if (data->sock_connected.result >= 0) {
			DBG_LOG("\r\nConnecting to Broker...");
			updateJWT(ms_ticks);
			mqtt_connect_broker(module_inst, 1, mqttUsername, mqttPassword, mqttCid, NULL, NULL, 0, 0, 0);
		} else {
			DBG_LOG("Connect fail to server(%s)! retry it automatically.\r\n", ALI_MQTT_HOST);
			mqtt_connect(module_inst, ALI_MQTT_HOST); /* Retry that. */
			
		}
	}
	break;

	case MQTT_CALLBACK_CONNECTED:
		if (data->connected.result == MQTT_CONN_RESULT_ACCEPT) {
			g_connect_ali_cloud = true;
			DBG_LOG("Preparation of the chat has been completed.\r\n");
		} else {
			/* Cannot connect for some reason. */
			g_connect_ali_cloud = false;
			DBG_LOG("MQTT broker decline your access! error code %d\r\n", data->connected.result);
		}

		break;

	case MQTT_CALLBACK_DISCONNECTED:
		/* Stop timer and USART callback. */
		DBG_LOG("MQTT disconnected\r\n");
		//usart_disable_callback(&cdc_uart_module, USART_CALLBACK_BUFFER_RECEIVED);
		break;
		
	case MQTT_CALLBACK_SUBSCRIBED:
		DBG_LOG("MQTT_CALLBACK_SUBSCRIBED\r\n");
		break;
	}
}



/**
 * \brief Callback to get the Socket event.
 *
 * \param[in] Socket descriptor.
 * \param[in] msg_type type of Socket notification. Possible types are:
 *  - [SOCKET_MSG_CONNECT](@ref SOCKET_MSG_CONNECT)
 *  - [SOCKET_MSG_BIND](@ref SOCKET_MSG_BIND)
 *  - [SOCKET_MSG_LISTEN](@ref SOCKET_MSG_LISTEN)
 *  - [SOCKET_MSG_ACCEPT](@ref SOCKET_MSG_ACCEPT)
 *  - [SOCKET_MSG_RECV](@ref SOCKET_MSG_RECV)
 *  - [SOCKET_MSG_SEND](@ref SOCKET_MSG_SEND)
 *  - [SOCKET_MSG_SENDTO](@ref SOCKET_MSG_SENDTO)
 *  - [SOCKET_MSG_RECVFROM](@ref SOCKET_MSG_RECVFROM)
 * \param[in] msg_data A structure contains notification informations.
 */
static void socket_event_handler(SOCKET sock, uint8_t msg_type, void *msg_data)
{
	mqtt_socket_event_handler(sock, msg_type, msg_data);
}

/**
 * \brief Callback of gethostbyname function.
 *
 * \param[in] doamin_name Domain name.
 * \param[in] server_ip IP of server.
 */
static void socket_resolve_handler(uint8_t *doamin_name, uint32_t server_ip)
{
	mqtt_socket_resolve_handler(doamin_name, server_ip);
}


static void read_ali_credentials(void)
{
	uint32_t product_key_length = sizeof(g_ali_product_key);
	uint32_t device_name_length = sizeof(g_ali_device_name);
	
	
	
#ifdef EVB_TEST_MODE
	strcpy(g_ali_product_key, ALI_PRODUCT_KEY);
	strcpy(g_ali_device_name, ALI_DEVICE_NAME);
#else 
	provisioning_get_ali_credentials(g_ali_product_key, &product_key_length, g_ali_device_name, &device_name_length);
#endif

}

Ali_Cloud_RC ali_iot_init(void) {
	
	
	struct mqtt_config mqtt_conf;
	int result;
	
	read_ali_credentials();

	mqtt_get_config_defaults(&mqtt_conf);
	/* To use the MQTT service, it is necessary to always set the buffer and the timer. */
	mqtt_conf.read_buffer = mqtt_read_buffer;
	mqtt_conf.read_buffer_size = MAIN_MQTT_BUFFER_SIZE;
	mqtt_conf.send_buffer = mqtt_send_buffer;
	mqtt_conf.send_buffer_size = MAIN_MQTT_BUFFER_SIZE;
	mqtt_conf.keep_alive = 20;
	
	result = mqtt_init(&mqtt_inst, &mqtt_conf);
	if (result < 0) {
		printf("MQTT initialization failed. Error code is (%d)\r\n", result);
		while (1) {
		}
	}

	result = mqtt_register_callback(&mqtt_inst, ali_mqtt_callback);
	if (result < 0) {
		printf("MQTT register callback failed. Error code is (%d)\r\n", result);
		while (1) {
		}
	}
	
	/* Initialize socket interface. */
	socketInit();
	registerSocketCallback(socket_event_handler, socket_resolve_handler);

	return ALI_CLOUD_RC_SUCCESS;
}

Ali_Cloud_RC ali_iot_mqtt_connect(char* hostname)
{
	struct sockaddr_in addr;
	SOCKET ali_socket = -1;
	
	mqtt_connect(&mqtt_inst, hostname);
	
	if (g_connect_ali_cloud == false)
		return ALI_CLOUD_RC_FAIL;
	else
		return ALI_CLOUD_RC_SUCCESS;
}

Ali_Cloud_RC ali_iot_mqtt_publish(char* channel, void* message, int msg_size)
{
	mqtt_publish(&mqtt_inst, channel, message, msg_size, 0, 0);
	
	return ALI_CLOUD_RC_SUCCESS;
}

Ali_Cloud_RC ali_iot_mqtt_subscribe(char* channel, void* cb)
{
	mqtt_subscribe_cb = cb;
	
	mqtt_subscribe(&mqtt_inst, channel, 0, SubscribeHandler);
	return ALI_CLOUD_RC_SUCCESS;
}

Ali_Cloud_RC ali_iot_mqtt_yield(void)
{
	if(mqtt_inst.isConnected)
	{
		mqtt_yield(&mqtt_inst, 0);
	}
	return ALI_CLOUD_RC_SUCCESS;
}



