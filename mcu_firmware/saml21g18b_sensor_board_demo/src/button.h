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

#ifndef __BUTTON_H__
#define __BUTTON_H__


#define TIMEOUT_COUNTER_5S		(5*1000)		//5seconds
#define MAX_CB_INDEX			3
//typedef void *(button_5s_timeout_cb[MAX_CB_INDEX])();
void (*button_5s_timeout_cb[MAX_CB_INDEX]) (void);
void (*button_detect_cb[MAX_CB_INDEX]) (void);
//const button_5s_timeout_cb **button_5s_timeout_cb_ptr = NULL;
//const button_5s_timeout_cb *button_5s_timeout_cb_ptr[MAX_CB_INDEX];

void initialise_button(void);
void buttonInitCheck(void);
void buttonTaskInit(void);
void buttonTaskExecute(uint32_t tick);
int regButtonPressDetectCallback(void* cb);
int unRegButtonPressDetectCallback(int sock);
int regButtonPress5sTimeoutCallback(void* cb);
int unRegButtonPress5sTimeoutCallback(int sock);


#endif /*__BUTTON_H__*/
