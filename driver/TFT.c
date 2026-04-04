#include "tft.h"

void TFT_Init(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_StructInit(&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin  = TFT_BL_PIN| TFT_RST_PIN| TFT_DC_PIN| TFT_CS_PIN;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
		
		SPI_InitTypeDef SPI_InitStruct;
		SPI_StructInit(&SPI_InitStruct);
		SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
		SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
		SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
		SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_Init(SPI1,&SPI_InitStruct);
		SPI_Cmd(SPI1,ENABLE);
		
		
		TFT_RST_LOW();
		delay_ms(10);
		TFT_RST_HIGH();
		delay_ms(120);
		
		TFT_Command(TFTcmd_SLEEP_OUT,0);
    delay_ms(120);
		
		
		TFT_Command(TFTcmd_RGB_SELECT,1,0x05);
		
		TFT_Command(TFTcmd_MADCTL,1,0x00);
		
		TFT_Command(TFTcmd_INV_OFF,0);
		
		TFT_Command(TFTcmd_DISP_ON,0);
		
    TFT_BL_ON();
}

void TFT_SendByte(uint8_t Data)
{
		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
		SPI_I2S_SendData(SPI1, Data);
}

void TFT_SendColor(uint16_t Data)
{
		TFT_SendByte((Data >> 8) & 0xFF);
		TFT_SendByte(Data & 0xFF);
}

void TFT_SetBlock(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1)
{
		TFT_Command(0x2A,4,(x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF);
		TFT_Command(0x2B,4,(y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF);
}


void TFT_Command(uint8_t Cmd,uint8_t num_params,...)
{
		TFT_CS_LOW();
		
		TFT_DC_LOW();
		TFT_SendByte(Cmd);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
		
		TFT_DC_HIGH();
		va_list args;
		va_start(args,num_params);
		for(uint8_t i=0;i<num_params;i++)
		{
				TFT_SendByte((uint8_t)va_arg(args,int));
		}
		va_end(args);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
		
		TFT_CS_HIGH();
}

void TFT_FillBlock(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1,uint16_t color)
{
		uint32_t total=(uint32_t)(x1-x0+1)*(y1-y0+1);
		TFT_SetBlock(x0, x1, y0, y1);
		
		TFT_CS_LOW();
	
    TFT_DC_LOW();
    TFT_SendByte(0x2C);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
		
		TFT_DC_HIGH();
		for(uint32_t i=0;i<total;i++) TFT_SendColor(color);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
		                                                                                            
		TFT_CS_HIGH();
}

void TFT_ClearScreen(uint16_t color)
{
		TFT_FillBlock(0, 239, 0, 319, color);
}

void TFT_DrawChar_16x16(uint16_t x, uint16_t y,const uint8_t *Char16x16, uint16_t fg_color, uint16_t bg_color)
{
		TFT_SetBlock(x, x+15, y, y+15);
		
		TFT_CS_LOW();
	
    TFT_DC_LOW();
    TFT_SendByte(0x2C);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
		
		TFT_DC_HIGH();
		for(uint16_t row_rol=0;row_rol<16;row_rol++)
		{
				uint16_t row_data=(Char16x16[row_rol*2]<<8)|Char16x16[row_rol*2+1];
				for(uint16_t col_jud=0;col_jud<16;col_jud++)
				{
					uint16_t jud=(0x8000>>col_jud)&row_data?fg_color:bg_color;
					TFT_SendColor(jud);
				}
		}
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
		                                                                                            
		TFT_CS_HIGH();
}

//void TFT_PrintChar_16x16()
//{
//		
//		
//}
