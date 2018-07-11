/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		����MCU����ֲW5500������Ҫ�޸ĵ��ļ�������W5500��MAC��IP��ַ
**************************************************************************************************
*/
#include "w5500_conf.h"

uint8 dhcp_ok=0;                /*dhcp�ɹ���ȡIP*/
uint32	ms=0;                   /*�������*/
uint32	dhcp_time= 0;           /*DHCP���м���*/
vu8	ntptimer = 0;               /*NPT�����*/


/**
*@brief		W5500Ƭѡ�ź����ú���
*@param		val: Ϊ��0����ʾƬѡ�˿�Ϊ�ͣ�Ϊ��1����ʾƬѡ�˿�Ϊ��
*@return	��
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW) 
	{
		GPIO_ResetBits(W5500_NCS_GPIO_PORT, W5500_NCS_GPIO_PIN); 
	}
	else if (val == HIGH)
	{
		GPIO_SetBits(W5500_NCS_GPIO_PORT, W5500_NCS_GPIO_PIN); 
	}
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}



/**
* @brief? �����ٽ���
* @retval None
*/
void SPI_CrisEnter(void)
{
    __set_PRIMASK(1);
}

/**
* @brief? �˳��ٽ���
* @retval None
*/
void SPI_CrisExit(void)
{
    __set_PRIMASK(0);
}

/**
* @brief? Ƭѡ�ź�����͵�ƽ
* @retval None
*/
void SPI_CS_Select(void)
{
    wiz_cs(LOW);
}


/**
* @brief Ƭѡ�ź�����ߵ�ƽ
* @retval None
*/
void SPI_CS_Deselect(void)
{
    wiz_cs(HIGH);
}


/**
  * @brief  д1�ֽ����ݵ�SPI����
  * @param  TxData д�����ߵ�����
  * @retval None
  */
void SPI_WriteByte(uint8_t TxData)
{                
     SPI_FLASH_SendByte(TxData);
}


/**
  * @brief  ��SPI���߶�ȡ1�ֽ�����
  * @retval ����������
  */
uint8_t SPI_ReadByte(void)
{            
    uint8_t data = SPI_FLASH_ReadByte();
    return data;
}


/**
*@brief		W5500��λ���ú���
*@param		��
*@return	��
*/
void reset_w5500(void)
{
/* ���ݲɼ��� */
#ifdef  STM32F103RC 
    GPIO_ResetBits(W5500_RST_GPIO_PORT, W5500_RST_GPIO_PIN);
    delay_us(2);
    GPIO_SetBits(W5500_RST_GPIO_PORT, W5500_RST_GPIO_PIN);
    delay_ms(1600);
#endif
}

/**
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_FLASH_SendByte(dat));
}

///**
//*@brief		д��һ��8λ���ݵ�W5500
//*@param		addrbsb: д�����ݵĵ�ַ
//*@param   data��д���8λ����
//*@return	��
//*/
//void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
//{
//   iinchip_csoff();                              		
//   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
//   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
//   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
//   IINCHIP_SpiSendData(data);                   
//   iinchip_cson();                            
//}

///**
//*@brief		��W5500����һ��8λ����
//*@param		addrbsb: д�����ݵĵ�ַ
//*@param   data����д��ĵ�ַ����ȡ����8λ����
//*@return	��
//*/
//uint8 IINCHIP_READ(uint32 addrbsb)
//{
//   uint8 data = 0;
//   iinchip_csoff();                            
//   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
//   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
//   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
//   data = IINCHIP_SpiSendData(0x00);            
//   iinchip_cson();                               
//   return data;    
//}

/**
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                               
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   iinchip_cson();                           
   return len;  
}


/**
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  iinchip_csoff();                                
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                  
  return len;
}

