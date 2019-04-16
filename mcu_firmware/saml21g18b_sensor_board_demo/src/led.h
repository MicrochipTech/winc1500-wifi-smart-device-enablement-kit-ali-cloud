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

#ifndef __LED_H__
#define __LED_H__

typedef enum Led_Color
{
	LED_COLOR_BLUE = 0,
	LED_COLOR_GREEN = 1,
	LED_COLOR_RED = 2,
	LED_COLOR_YELLOW = 3,
	LED_COLOR_Magneta = 4,
	LED_COLOR_Cyan = 5,
	LED_COLOR_WHTIE = 6,
}Led_Color;

typedef enum Led_Mode
{
	LED_MODE_NONE = 0,
	LED_MODE_TURN_OFF = 1,
	LED_MODE_TURN_ON = 2,
	LED_MODE_BLINK_NORMAL = 3,
	LED_MODE_BLINK_FAST = 4,
	LED_MODE_BLINK_SHORT = 5,
}Led_Mode;

void initialise_led(void);
void led_ctrl_set_color(Led_Color color, Led_Mode mode);
void led_ctrl_set_mode(Led_Mode mode);
void led_ctrl_execute(void);
void toggleLED(void);
Led_Color led_ctrl_get_color(void);
Led_Mode led_ctrl_get_mode(void);

extern Led_Mode gu8LedMode;

#endif /*__LED_H__*/
