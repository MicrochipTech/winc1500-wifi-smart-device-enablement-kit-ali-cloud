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

#ifndef __CLOUD_WRAPPER_H__
#define __CLOUD_WRAPPER_H__

typedef enum {
	CLOUD_RC_NONE_ERROR = 0,
	CLOUD_RC_SUCCESS = 0,
	CLOUD_RC_NETWORK_ATTEMPTING_RECONNECT = -25,
}Cloud_RC;

Cloud_RC cloud_init(void);
Cloud_RC cloud_connect(char* hostname);
Cloud_RC cloud_mqtt_yield(int timeout);
Cloud_RC cloud_mqtt_publish(char* channel, void* message, int msg_size);
Cloud_RC cloud_mqtt_subscribe(char* channel, void*);
Cloud_RC cloud_create_topic(char* full_path, char* device_type, char* device_id, char* topic_name);
Cloud_RC cloud_create_search_topic(char* full_path, char* acct_uuid, char* topic_name);

#endif /*__WIFI_APP_H__*/
