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
#include <stdio.h>

#include "aws_status.h"
#include "hex_dump.h"

// Global variables
static struct aws_iot_status g_aws_iot_status;

/**
 * \brief Clears the global AWS IoT status information
 */
void aws_iot_clear_status(void)
{
    g_aws_iot_status.aws_state  = AWS_STATE_UNKNOWN;
    g_aws_iot_status.aws_status = AWS_STATUS_SUCCESS;

    memset(&g_aws_iot_status.aws_message[0], 0, sizeof(g_aws_iot_status.aws_message));
}

/**
 * \brief Gets the global AWS IoT status information
 *
 * \note Do not free the pointer returned from this function
 */
struct aws_iot_status * aws_iot_get_status(void)
{
    return &g_aws_iot_status;
}

/**
 * \brief Sets the global AWS IoT status information
 *
 * param[in] state                  The AWS IoT error state
 * param[in] status                 The AWS IoT error status
 * param[in] message                The AWS IoT error message
 */
void aws_iot_set_status(uint32_t state, uint32_t status, const char *message)
{
    g_aws_iot_status.aws_state  = state;
    g_aws_iot_status.aws_status = status;

    memset(&g_aws_iot_status.aws_message[0], 0, 
           sizeof(g_aws_iot_status.aws_message));
    strncpy(&g_aws_iot_status.aws_message[0], &message[0], 
            sizeof(g_aws_iot_status.aws_message));
}


/**
 * \brief Prints the current AWS IoT status information to the console EDBG USART
 *
 * \param[in] message               The message to be printed
 * \param[in] error                 The AWS IoT status information to be printed
 */
void console_print_aws_status(const char *message, const struct aws_iot_status *status)
{
    // Obtain the console mutex

    printf("\r\nSTATUS: %s\r\n", message);
    
    // Print the AWS status information
    printf("Current AWS IoT State:   %lu\r\n", status->aws_state);
    printf("Current AWS IoT Status:  %lu\r\n", status->aws_status);
    printf("Current AWS IoT Message: %s\r\n", status->aws_message);

    printf("\r\n");
    
}

/**
 * \brief Prints the AWS IoT message information to the console EDBG USART
 *
 * \param[in] message               The message to be printed
 * \param[in] buffer                The data buffer containing the information
 *                                  to be printed
 * \param[in] length                The length, in bytes, of the data buffer
 */
void console_print_aws_message(const char *message, const void *buffer, size_t length)
{

    printf("\r\n%s\r\n", message);

    // Print the hex dump of the information in the data buffer
    print_hex_dump(buffer, length, true, true, 16);

    printf("\r\n");
    
}