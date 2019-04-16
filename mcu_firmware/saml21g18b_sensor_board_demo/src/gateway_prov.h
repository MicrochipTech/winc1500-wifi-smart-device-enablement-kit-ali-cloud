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


#ifndef __GATEWAY_PROV_H__
#define __GATEWAY_PROV_H__


#include "cmdset.h"
#include "socket/include/m2m_socket_host_if.h"
#include "driver/include/m2m_wifi.h"

#define PADDING_LEN     0

typedef enum _prov_state{
	PROV_WAITING,
	PROV_INIT,
	PROV_CONFIG,
	PROV_DONE,
	STA_INIT,
	STA_BCAST,
	STA_FOUND
} prov_state_t;

typedef struct _cmd_resp{
	uint16_t length;
	union {
		// The max length of cmd response: cluster length + header + checksum
		uint8_t base[MIN_CMD_LEN + PADDING_LEN];
		uint8_t discovery[DISCOVERY_RESP_PARAM_LEN + DISCOVERY_RESP_HDR_LEN + CHECKSUM_LEN + PADDING_LEN];
		uint8_t provision[PROV_RESP_PARAM_LEN + PROV_RESP_HDR_LEN + CHECKSUM_LEN + PADDING_LEN];
		uint8_t authpacket[AUTHREQ_RESP_PARAM_LEN + AUTHREQ_RESP_HDR_LEN  + CHECKSUM_LEN + PADDING_LEN];
		uint8_t rand_chal_resp[RANCHAL_RESP_PARAM_LEN + RANCHAL_RESP_HDR_LEN + CHECKSUM_LEN + PADDING_LEN];
		
	} data;
} cmd_resp_t;




prov_state_t get_prov_state(void);
void set_prov_state(prov_state_t state);
void parse_iot_cmd(SOCKET sock, uint8_t cmd, uint8_t *cmd_param, cmd_resp_t *cmd_resp);



#endif /*__WIFI_APP_H__*/
