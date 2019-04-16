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
#include "main.h"
#include "nvm_handle.h"


#define FLASH_NVDS_SIZE		1500
#define TEST_PAGE_ADDR     (FLASH_NB_OF_PAGES - 16) * NVMCTRL_PAGE_SIZE

uint8_t NVDS_Data[FLASH_NVDS_SIZE];
int nvm_init(void)
{
	struct nvm_config config;
	enum status_code status;

	/* Get the default configuration */
	nvm_get_config_defaults(&config);

	/* Set wait state to 1 */
	//config.wait_states = 1;

	/* Enable automatic page write mode */
	config.manual_page_write = false;

	/* Set the NVM configuration */
	status = nvm_set_config(&config);

	/* Validate the initialization */
	//test_assert_true(test, status == STATUS_OK,
	//"NVM Initialization error");

	DBG_LOG("status = %d", status);
	
	return status;
}

int nvds_erase(void)//sector erase
{
	uint32_t delay = 0;
	uint32_t i;
	uint32_t len_buf=FLASH_PAGE_SIZE;
	int ret;
	//uint8_t read_buf[len_buf];
	//uint8_t write_buf[len_buf];

	DBG_LOG("nvds_erase start\r\n");

	
	memset(NVDS_Data,0xff,sizeof(NVDS_Data));
	for(i=0;i<16;i++)
	{
		nvm_erase_row((unsigned long)(TEST_PAGE_ADDR+i*NVMCTRL_PAGE_SIZE));
		ret = nvm_write_buffer((unsigned long)(TEST_PAGE_ADDR+i*NVMCTRL_PAGE_SIZE),
		(uint8_t *)&NVDS_Data[i*NVMCTRL_PAGE_SIZE],
		NVMCTRL_PAGE_SIZE);
	}
}


void nvm_get_config_data(wifi_nvm_data_t* config_data)
{
	uint32_t delay = 0;
	uint32_t i;
	//uint32_t len_buf=FLASH_PAGE_SIZE;
	//uint8_t read_buf[len_buf];

	
	DBG_LOG("nvds_read start\r\n");
	//nvm_init();
	struct nvm_parameters parameters;
	nvm_get_parameters(&parameters);
	DBG_LOG("nvm page_size = %d\n", parameters.page_size);
	DBG_LOG("nvm no_of_page = %d\n", parameters.nvm_number_of_pages);
	
	memset(NVDS_Data,0,sizeof(NVDS_Data));

	for(i=0;i<16;i++)
	{
		nvm_read_buffer((unsigned long)(TEST_PAGE_ADDR+i*NVMCTRL_PAGE_SIZE),
		(uint8_t *)&NVDS_Data[i*NVMCTRL_PAGE_SIZE],
		NVMCTRL_PAGE_SIZE);
	}

	memcpy((uint8_t *)config_data,NVDS_Data,sizeof(wifi_nvm_data_t));
}


void nvm_store_config_data(wifi_nvm_data_t config_data)
{
	uint32_t delay = 0;
	uint32_t i;
	
	nvds_erase();
	//DBG_LOG("ble_device_count write:%d",ble_device_count);
	memset(NVDS_Data,0,sizeof(NVDS_Data));
	//if(len<=FLASH_NVDS_SIZE)
	//	memcpy(NVDS_Data,data,len);
	DBG_LOG("nvds data len:%d",sizeof(wifi_nvm_data_t));
	memcpy(NVDS_Data,(uint8_t *)&config_data,sizeof(wifi_nvm_data_t));
	
	DBG_LOG("ssid = %s", config_data.ssid);
	
	
	for(i=0;i<16;i++)
	{
		nvm_write_buffer((unsigned long)(TEST_PAGE_ADDR+i*NVMCTRL_PAGE_SIZE),
			(uint8_t *)&NVDS_Data[i*NVMCTRL_PAGE_SIZE], 
			NVMCTRL_PAGE_SIZE);
	}
	
#if 0	// for debug only
	memset(NVDS_Data,0,sizeof(NVDS_Data));
	
	for(i=0;i<16;i++)
	{
		nvm_read_buffer((unsigned long)(TEST_PAGE_ADDR+i*NVMCTRL_PAGE_SIZE),
		(uint8_t *)&NVDS_Data[i*NVMCTRL_PAGE_SIZE],
		NVMCTRL_PAGE_SIZE);
		DBG_LOG("Read NVDS_Data = %d",NVDS_Data[i*NVMCTRL_PAGE_SIZE]);
	}
	memset(&config_data,0,sizeof(wifi_nvm_data_t));
	memcpy((uint8_t *)&config_data,NVDS_Data,sizeof(wifi_nvm_data_t));
#endif
	
}
	
	
