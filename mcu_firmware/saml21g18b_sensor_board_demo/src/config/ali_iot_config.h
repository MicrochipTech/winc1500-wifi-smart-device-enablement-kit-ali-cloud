/**
 * \file
 *
 * \brief MAIN configuration.
 *
 * Copyright (c) 2017 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * @file ali_iot_config.h
 * @brief ALI IoT specific configuration file
 */

#ifndef ALI_IOT_CLOUD_CONFIG_H_
#define ALI_IOT_CLOUD_CONFIG_H_

//#define ALI_DEVICE_NAME "AVRIOTA5F0"
//#define ALI_PRODUCT_KEY "a11v7xKKAWn"
#define ALI_DEVICE_NAME "vj7i5asSgTA06a0gSB6o"
#define ALI_PRODUCT_KEY "a1GUxQfLrai"
#define ALI_DEVICE_SECRET_KEY "PfrItKRyTax2ItKcWLE2UqFe7aTiD0od"
#define ALI_MQTT_HOST_SUFFIX ".iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define ALI_MQTT_HOST ALI_PRODUCT_KEY ALI_MQTT_HOST_SUFFIX

/* Max size of MQTT buffer. */
#define MAIN_MQTT_BUFFER_SIZE 512

#define CLIENT_ID_LENGTH 64 + 1 //the client id is not more than 64 bytes in aliyun document  +  '\0' (1)byte = 65bytes
#define HASH_MSG_LENGTH 72 + 40 + 21 + 22 + 1 //clientId (8+64=72)bytes  +  deviceName (10+30=40)bytes  +  productKey (10+11=21)bytes  +  timestamp (9+13=22)bytes  +  '\0' (1)byte = 156bytes
#define MQTT_CID_LENGTH 64 + 60 + 1//clientId(64 bytes)  +  |securemode=3,signmethod=hmacsha256,timestamp=1539939251799|(60 bytes)  +  '\0'(1byte)
#define MQTT_TOPIC_LENGTH 80 //38

#endif /* ALI_IOT_CLOUD_CONFIG_H_ */
