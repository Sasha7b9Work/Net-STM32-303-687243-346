#include "defines.h"
#include "Hardware/CDC/CDC.h"
#include <usbd_cdc.h>
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#define APP_RX_DATA_SIZE  512
#define APP_TX_DATA_SIZE  512



PCD_HandleTypeDef _handlePCD;

void *handlePCD = &_handlePCD;


USBD_CDC_LineCodingTypeDef LineCoding =
{
  115200, /* baud rate*/
  0x00,   /* stop bits-1*/
  0x00,   /* parity - none*/
  0x08    /* nb. of bits 8*/
};

uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */
uint8_t UserTxBuffer[APP_TX_DATA_SIZE];/* Received Data over UART (CDC interface) are stored in this buffer */
uint32_t BuffLength;
uint32_t UserTxBufPtrIn = 0;/* Increment this pointer or roll it back to
                               start address when data are received over USART */
uint32_t UserTxBufPtrOut = 0; /* Increment this pointer or roll it back to
                                 start address when data are sent over USB */

                                 /* USB handler declaration */
extern USBD_HandleTypeDef  hUSBDDevice;


static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t *pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_CDC_fops =
{
  CDC_Itf_Init,
  CDC_Itf_DeInit,
  CDC_Itf_Control,
  CDC_Itf_Receive
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  CDC_Itf_Init
  *         Initializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_Init(void)
{
    USBD_CDC_SetTxBuffer(&hUSBDDevice, UserTxBuffer, 0);
    USBD_CDC_SetRxBuffer(&hUSBDDevice, UserRxBuffer);

    return (USBD_OK);
}

/**
  * @brief  CDC_Itf_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_DeInit(void)
{
    return (USBD_OK);
}

/**
  * @brief  CDC_Itf_Control
  *         Manage the CDC class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
    UNUSED(length);

    switch (cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        /* Add your code here */
        break;

    case CDC_GET_ENCAPSULATED_RESPONSE:
        /* Add your code here */
        break;

    case CDC_SET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_GET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_CLEAR_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_SET_LINE_CODING:
        LineCoding.bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | \
            (pbuf[2] << 16) | (pbuf[3] << 24));
        LineCoding.format = pbuf[4];
        LineCoding.paritytype = pbuf[5];
        LineCoding.datatype = pbuf[6];

        break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = (uint8_t)(LineCoding.bitrate);
        pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
        pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
        pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
        pbuf[4] = LineCoding.format;
        pbuf[5] = LineCoding.paritytype;
        pbuf[6] = LineCoding.datatype;

        /* Add your code here */
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        /* Add your code here */
        break;

    case CDC_SEND_BREAK:
        /* Add your code here */
        break;

    default:
        break;
    }

    return (USBD_OK);
}


static int8_t CDC_Itf_Receive(uint8_t *Buf, uint32_t *Len)
{
    UNUSED(Buf);
    UNUSED(Len);

    return (USBD_OK);
}


void HCDC::Transmit(const void *buffer, int size)
{
    if (!buffer)
    {
        return;
    }

    USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)hUSBDDevice.pClassData;

    if (hcdc->TxState != 0)
    {
        return;
    }

    USBD_CDC_SetTxBuffer(&hUSBDDevice, (uint8_t *)buffer, (uint16_t)size);
    USBD_CDC_TransmitPacket(&hUSBDDevice);
}


void HCDC::TransmitF(pchar format, ...)
{
    char buffer[128];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);

    Transmit((const void *)buffer, (int)std::strlen(buffer) + 1);
}
