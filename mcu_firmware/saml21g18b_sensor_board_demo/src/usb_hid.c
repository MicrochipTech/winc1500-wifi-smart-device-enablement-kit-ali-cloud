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

#include <string.h>

#include "asf.h"
#include "usb_hid.h"
#include "kit_protocol_utilities.h"

// Use the KIT PROTOCOL message delimiter as the USB message completed delimiter
#define USB_MESSAGE_DELIMITER  KIT_MESSAGE_DELIMITER

#define USB_DELAY              (50)

// Global variables
uint8_t  g_usb_buffer[KIT_MESSAGE_SIZE_MAX];  //! The USB message buffer
uint16_t g_usb_buffer_length = 0;             //! The USB message buffer length
uint8_t     g_usb_message_received;      //! Whether the USB message was received

/**
 * \brief Initializes the USB HID interface.
 */
void usb_hid_init(void)
{
    // Start the USB device stack
    udc_start();
    // Enter sleep mode
    sleepmgr_enter_sleep();
}

bool usb_send_response_message(uint8_t *response, uint16_t response_length)
{
    bool usb_report_sent = false;
    uint16_t current_response_location = 0;
    uint8_t usb_report[UDI_HID_REPORT_OUT_SIZE];
    uint8_t usb_report_length = 0;
    uint8_t send_retries = 5;
    
    if (response == NULL)
    {
        return false;
    }
    
    // Send the USB response message
    while (response_length > 0)
    {
        // Create the USB report
        usb_report_length = min(UDI_HID_REPORT_OUT_SIZE, response_length);
        
        memset(&usb_report[0], 0, sizeof(usb_report));
        memcpy(&usb_report[0], &response[current_response_location], 
               usb_report_length);
        
        usb_report_sent = udi_hid_generic_send_report_in(usb_report);
        if (usb_report_sent == true)
        {
            current_response_location += usb_report_length;
            response_length -= usb_report_length;            

            // Delay for 50ms
            delay_ms(USB_DELAY);
        }
        else
        {
            if (send_retries > 0)
            {
                // Delay for 50ms
                delay_ms(USB_DELAY);
                
                // Decrement the send retries
                send_retries--; 
            }
            else
            {
                // Break the while loop
                break;
            }
        }  
    }
    
    // Clear the USB message buffer
    memset(&g_usb_buffer[0], 0, sizeof(g_usb_buffer));
    g_usb_buffer_length = 0;
	printf("g_usb_message_received = false\r\n");
    g_usb_message_received--;
    
    return usb_report_sent;
}

/**
 * \brief Callback called when the USB host enables the USB interface.
 *
 * \return    Whether the USB interface should be enabled
 *                TRUE  - The USB interface should be enabled
 *                FALSE - The USB interface should not be enabled
 */
bool usb_hid_enable_callback(void)
{
    return true;
}

/**
 * \brief Callback called when the USB host disables the USB interface.
 */
void usb_hid_disable_callback(void)
{
}

/**
 * \brief Callback called when the USB endpoint wakeup event occurs.
 */
void usb_hid_wakeup_callback(void)
{
}

/**
 * \brief Handles the incoming USB report.
 *
 * \param[in] report                The incoming USB report
 */
void usb_hid_report_out_callback(uint8_t *report)
{
    // Handle incoming USB report
    
    for (uint32_t index = 0; index < UDI_HID_REPORT_OUT_SIZE; index++)
    {
        // Save the incoming USB packet
        g_usb_buffer[g_usb_buffer_length] = report[index];
        g_usb_buffer_length++;
        
        // Check if the USB message was received
        if (report[index] == USB_MESSAGE_DELIMITER)
        {
			printf("g_usb_message_received = true\r\n");
            g_usb_message_received++;
            break;
        }
    }
}

/**
 * \brief Handles the incoming USB feature request.
 *
 * \param[in] report                The incoming USB report
 */
void usb_hid_set_feature_callback(uint8_t *report)
{
    // Do nothing with the incoming feature request
}
