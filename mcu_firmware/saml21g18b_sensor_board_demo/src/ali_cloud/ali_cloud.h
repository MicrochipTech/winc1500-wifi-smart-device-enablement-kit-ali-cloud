
/*
 * veml6080.h
 *
 * Created: 11/17/2015 4:21:40 PM
 *  Author: gracy.angela
 */ 


#ifndef ALI_CLOUD_H_
#define ALI_CLOUD_H_


char g_ali_product_key[15];
char g_ali_device_name[25];


typedef enum {
	ALI_CLOUD_RC_NONE_ERROR = 0,
	ALI_CLOUD_RC_SUCCESS = 0,
	ALI_CLOUD_RC_FAIL= 1,
}Ali_Cloud_RC;

Ali_Cloud_RC ali_iot_init(void);
Ali_Cloud_RC ali_iot_mqtt_connect(char* hostname);
Ali_Cloud_RC ali_iot_mqtt_publish(char* channel, void* message, int msg_size);
Ali_Cloud_RC ali_iot_mqtt_subscribe(char* channel, void* cb);
Ali_Cloud_RC ali_iot_mqtt_yield(void);


#endif /* ALI_CLOUD_H_ */