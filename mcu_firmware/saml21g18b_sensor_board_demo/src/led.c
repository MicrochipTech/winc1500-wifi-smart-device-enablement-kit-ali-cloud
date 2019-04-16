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
#include "conf_board.h"
#include "led.h"
#include "rtc.h"

unsigned char gu8Blue;
unsigned char gu8Red;
unsigned char gu8Green;
unsigned char gu8OnOffState;
Led_Mode gu8LedMode;
Led_Color gu8Color;


void toggleLED()
{
	if (gu8OnOffState == 1)
	{
		
		gu8OnOffState = 0;
		port_pin_set_output_level(BLUE_LED, 1);
		port_pin_set_output_level(GREEN_LED, 1);
		port_pin_set_output_level(RED_LED, 1);
	}
	else
	{
		gu8OnOffState = 1;
		port_pin_set_output_level(BLUE_LED, gu8Blue);
		port_pin_set_output_level(GREEN_LED, gu8Green);
		port_pin_set_output_level(RED_LED, gu8Red);
	}
}

static inline void turnOnLED(void)
{
	printf("turnOnLED\r\n");
	gu8OnOffState = 1;
	port_pin_set_output_level(BLUE_LED, gu8Blue);
	port_pin_set_output_level(GREEN_LED, gu8Green);
	port_pin_set_output_level(RED_LED, gu8Red);
}

static inline void turnOffLED(void)
{
	printf("turnOffLED\r\n");
	gu8OnOffState = 0;
	port_pin_set_output_level(BLUE_LED, 1);
	port_pin_set_output_level(GREEN_LED, 1);
	port_pin_set_output_level(RED_LED, 1);
}

void initialise_led(void)
{
	gu8Blue = 0;
	gu8Green = 1;
	gu8Red = 1;
	gu8Color = LED_COLOR_BLUE;
	
	/* led port pin initialization */
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(BLUE_LED, &config_port_pin);
	port_pin_set_config(GREEN_LED, &config_port_pin);
	port_pin_set_config(RED_LED, &config_port_pin);
	port_pin_set_output_level(BLUE_LED, gu8Blue);
	port_pin_set_output_level(GREEN_LED, gu8Green);
	port_pin_set_output_level(RED_LED, gu8Red);
}


Led_Color led_ctrl_get_color()
{
	return gu8Color;
}


Led_Mode led_ctrl_get_mode()
{
	return gu8LedMode;
}

void led_ctrl_set_color(Led_Color color, Led_Mode mode)
{
	switch(color)
	{
		case LED_COLOR_BLUE:
			gu8Blue = 0;
			gu8Green = 1;
			gu8Red = 1;
			gu8Color = LED_COLOR_BLUE;
			break;
		case LED_COLOR_GREEN:
			gu8Blue = 1;
			gu8Green = 0;
			gu8Red = 1;
			gu8Color = LED_COLOR_GREEN;
			break;
		case LED_COLOR_RED:
			gu8Blue = 1;
			gu8Green = 1;
			gu8Red = 0;
			gu8Color = LED_COLOR_RED;
			break;
		case LED_COLOR_YELLOW:
			gu8Blue = 1;
			gu8Green = 0;
			gu8Red = 0;
			gu8Color = LED_COLOR_YELLOW;
			break;
		case LED_COLOR_Magneta:
			gu8Blue = 0;
			gu8Green = 1;
			gu8Red = 0;
			gu8Color = LED_COLOR_Magneta;
			break;
		case LED_COLOR_Cyan:
			gu8Blue = 0;
			gu8Green = 0;
			gu8Red = 1;
			gu8Color = LED_COLOR_Cyan;
			break;
		case LED_COLOR_WHTIE:
			gu8Blue = 1;
			gu8Green = 1;
			gu8Red = 1;
			gu8Color = LED_COLOR_WHTIE;
			break;
			
		default:
			break;
		
	}
	
	if (mode == LED_MODE_NONE)
	{
		if (gu8OnOffState == 1)
			turnOnLED();	// color change take effect
		return;
	}
	
	gu8LedMode = mode;
	
}

void led_ctrl_set_mode(Led_Mode mode)
{
	gu8LedMode = mode;
	switch (mode)
	{
		case LED_MODE_TURN_OFF:
			turnOffLED();
			break;
		case LED_MODE_TURN_ON:
			turnOnLED();
			break;
		
		default:
			break;
	}

}

void led_ctrl_execute()
{
	switch (gu8LedMode)
	{
		case LED_MODE_TURN_OFF:
			turnOffLED();
			break;
		case LED_MODE_TURN_ON:
			turnOnLED();
			break;
		
		case LED_MODE_BLINK_NORMAL:
			if (tick_500ms)
			{
				tick_500ms = 0;
				if (gu8OnOffState == 1)
					turnOffLED();
				else
					turnOnLED();
			}
			break;
		case LED_MODE_BLINK_FAST:
			if (tick_100ms)
			{
				tick_100ms = 0;
				if (gu8OnOffState == 1)
					turnOffLED();
				else
					turnOnLED();
			}
			break;
		
		default:
			break;
	}
}