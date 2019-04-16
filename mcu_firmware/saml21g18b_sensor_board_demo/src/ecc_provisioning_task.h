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

#ifndef __PROVISIONING_TASK_H__
#define __PROVISIONING_TASK_H__

#include <stdint.h>

#include "cryptoauthlib.h"
#include "aws_status.h"

// Set the packing alignment for the structure members
#pragma pack(push, 1)

// AWS IoT ATECC508A Configuration
#define DEVICE_KEY_SLOT            (0)
#define METADATA_SLOT              (8)
#define SIGNER_CA_PUBLIC_KEY_SLOT  (15)


// ATECC508A Slot 8 Metadata
#define SLOT8_SIZE                (416)
#define SLOT8_SSID_SIZE           (32)
#define SLOT8_WIFI_PASSWORD_SIZE  (64)
#define SLOT8_HOSTNAME_SIZE       (128)
#define SLOT8_ALI_PRODUCT_KEY_SIZE       (15)
#define SLOT8_ALI_DEVICE_NAME_SIZE       (25)
#define SLOT8_ALI_DEVICE_KEY_SIZE       (50)

#define SLOT8_WIFI_PROVISIONED_VALUE   (0x72B0)  //! Value to determine if the ATECC508A is provisioned with wifi credentials
#define SLOT8_AWS_PROVISIONED_VALUE    (0xF309)  //! Value to determine if the ATECC508A is provisioned with AWS credentials

#define ECCx08A_DEFAULT_ADDRESS  (uint8_t)(0xC0)  //! Default I2C address for unconfigured ECCx08A crypto devices
#define AWS_ECCx08A_I2C_ADDRESS  (uint8_t)(0xB0)  //! AWS ECCx08A device I2C address
#define AWS_KIT_DEVICES_MAX      (1)              //! Maximum number of AWS Kit CryptoAuth devices


//ATECC508A Slot 8 Metadata structure
struct Eccx08A_Slot8_Metadata
{
	uint32_t provision_flag;             //! Flag to tell if the ATECCx08A is provisioned
	uint32_t ssid_size;
	uint8_t  ssid[SLOT8_SSID_SIZE];
	uint32_t wifi_password_size;
	uint8_t  wifi_password[SLOT8_WIFI_PASSWORD_SIZE];
	uint32_t hostname_size;
	uint8_t  hostname[SLOT8_HOSTNAME_SIZE];
	uint32_t ali_product_key_size;
	uint8_t  ali_product_key[SLOT8_ALI_PRODUCT_KEY_SIZE];
	uint32_t ali_device_name_size;
	uint8_t  ali_device_name[SLOT8_ALI_DEVICE_NAME_SIZE];
};

enum aws_iot_state provisioning_get_state(void);


ATCA_STATUS provisioning_get_serial_number(uint32_t *serial_number_length,
											uint8_t *serial_number);
ATCA_STATUS provisioning_get_signer_ca_public_key(uint32_t *public_key_length,
													uint8_t *public_key);
ATCA_STATUS provisioning_get_ssid(uint32_t *ssid_length, char *ssid);
ATCA_STATUS provisioning_get_wifi_password(uint32_t *password_length, char *password);
ATCA_STATUS provisioning_get_hostname(uint32_t *hostname_length, char *hostname);
//ATCA_STATUS provisioning_get_ali_credentials(char *product_key, uint32_t *product_key_length, char* device_name, uint32_t * device_name_length, char* device_secret, uint32_t *device_secret_length);
ATCA_STATUS provisioning_get_ali_credentials(char *product_key, uint32_t *product_key_length, char* device_name, uint32_t * device_name_length);

enum kit_protocol_status kit_board_get_version(uint8_t *message,
												uint16_t *message_length);
enum kit_protocol_status kit_board_get_firmware(uint8_t *message,
												uint16_t *message_length);
enum kit_protocol_status kit_board_get_device(uint32_t device_handle,
												uint8_t *message,
												uint16_t *message_length);
enum kit_protocol_status kit_board_application(uint32_t device_handle,
												uint8_t *message,
												uint16_t *message_length);

enum kit_protocol_status kit_device_idle(uint32_t device_handle);
enum kit_protocol_status kit_device_sleep(uint32_t device_handle);
enum kit_protocol_status kit_device_wake(uint32_t device_handle);
enum kit_protocol_status kit_device_talk(uint32_t device_handle,
											uint8_t *message,
											uint16_t *message_length);
											
uint8_t cryptoauthlib_compute_hmacsha256(const uint8_t * data, size_t data_size, char *s);

void ecc_provisioning_task();
void zero_touch_provisioning_task();

// Unset the packing alignment for the structure members
#pragma pack(pop)

#endif /*__LED_H__*/
