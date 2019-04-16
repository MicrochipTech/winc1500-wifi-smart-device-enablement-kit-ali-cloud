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
#include "platform.h"
#include "at_ble_api.h"
#include "timer_hw.h"
#include "conf_serialdrv.h"
#include "conf_board.h"
#include "wearable.h"
#include "touch_api_ptc.h"
#include "touch_app.h"
#include "rtc.h"
#include "bme280\bme280_support.h"
#include "conf_sensor.h"
#include "veml60xx\veml60xx.h"
#include "bhi160.h"
#include "bhi160\bhy_uc_driver.h"
#include "i2c.h"
#include "serial_drv.h"
#include "adc_measure.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"
#include "main.h"
#include "led.h"
#include "env_sensor.h"
#include "motion_sensor.h"
#include "button.h"
#include "nvm_handle.h"
#include "winc15x0.h"
#include "usb_hid.h"
#include "ecc_provisioning_task.h"
#include "conf_console.h"

/* enum variable */
enum status_code veml60xx_sensor_status = STATUS_ERR_NOT_INITIALIZED;

/* variables */
BME280_RETURN_FUNCTION_TYPE bme280_sensor_status = ERROR;
	
/* function prototypes */
void configure_wdt(void);

void enable_gclk1(void);

/** SysTick counter to avoid busy wait delay. */
//volatile uint32_t ms_ticks = 0;



/** UART module for debug. */
static struct usart_module cdc_uart_module;

/**
 *  Configure console.
 */
static void serial_console_init(void)
{
 	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}



/* Watchdog configuration */
void configure_wdt(void)
{
	/* Create a new configuration structure for the Watchdog settings and fill
	* with the default module settings. */
	struct wdt_conf config_wdt;
	wdt_get_config_defaults(&config_wdt);
	/* Set the Watchdog configuration settings */
	config_wdt.always_on = false;
	//config_wdt.clock_source = GCLK_GENERATOR_4;
	config_wdt.timeout_period = WDT_PERIOD_2048CLK;
	/* Initialize and enable the Watchdog with the user settings */
	wdt_set_config(&config_wdt);
}



void enable_gclk1(void)
{
	struct system_gclk_gen_config gclk_conf;
	
	system_gclk_init();	
	gclk_conf.high_when_disabled = false;
	gclk_conf.source_clock       = GCLK_SOURCE_OSC16M;
	gclk_conf.division_factor = 1;
	gclk_conf.run_in_standby  = true;
	gclk_conf.output_enable   = false;
	system_gclk_gen_set_config(2, &gclk_conf);
	system_gclk_gen_enable(2);
}





/**
 * \brief SysTick handler used to measure precise delay. 
 */
//void SysTick_Handler(void)
//{
//	ms_ticks++;
//	printf("DBG log1\r\n");
//}

/* main function */
int main(void)
{

	/* Initialize RTC */
	rtc_init();
	
	/* initialize LED */
	initialise_led();
	
	led_ctrl_set_color(LED_COLOR_BLUE, LED_MODE_BLINK_NORMAL);	
		
	/* system clock initialization */
	system_init();

	//i2c configure
	configure_sensor_i2c();
	
	/* delay routine initialization */
	delay_init();

	/* configure adc for battery measurement */
	configure_adc();
 
#ifdef DEBUG_SUPPORT
	/* Initialize serial console for debugging */
	serial_console_init();
#endif

	
	DBG_LOG("Initializing Wi-Fi Sensor Board Demo Device");
	DBG_LOG("cpu_freq=%d\n",(int)system_cpu_clock_get_hz());
#ifdef USE_ALI_CLOUD
	DBG_LOG("Ali Firmware version: %s.%s.%s", ALI_FIRMWARE_MAJOR_VER, ALI_FIRMWARE_MINOR_VER, ALI_FIRMWARE_PATCH_VER);
#else
	DBG_LOG("Firmware version: %s.%s", FIRMWARE_MAJOR_VER, FIRMWARE_MINOR_VER);
#endif
	
	/* Initialize the BSP. */
	nm_bsp_init();
	
	nvm_init();
	
	initialise_button();
	
	buttonInitCheck();

	/* Hardware timer */
	//hw_timer_init();
	//Initialize bme280
	wearable_bme280_init();
	//Initialize veml60xx
	veml60xx_init();	

#ifdef ECC508
	// Initialize the USB HID interface
	usb_hid_init();

	ecc_provisioning_task();
#endif
	
#ifdef AWS_JITR
	while (1) {
		zero_touch_provisioning_task();
	}
#endif
	
	if (SysTick_Config(system_cpu_clock_get_hz() / 1000))
	{
		puts("ERR>> Systick configuration error\r\n");
		while (1);
	}

	wifiInit();

	env_sensor_data_init();
	
	while (1) {
		zero_touch_provisioning_task();
		/* Handle pending events from network controller. */
		wifiTaskExecute();

		if(tick_2second == 1)
		{
			tick_2second = 0;
			if (getWiFiMode()==APP_STA && getWiFiStates() > WIFI_TASK_MQTT_SUBSCRIBE)
				env_sensor_execute();
			
		}
		//led_ctrl_execute();
		
		
	}


}


