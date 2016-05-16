#ifndef __BSP_M25P16_H__
#define __BSP_M25P16_H__

/*----添加需要包含的头文件----*/
//////////////////////////////////////////////////////////////////
#include "hal_spi2.h"
#include <CoOS.h>
//////////////////////////////////////////////////////////////////

#define M25P16_MAXBYTE	2*1024*1024

#define		WREN_FLASH					0x06
#define 	WRDI_FLASH					0x04
#define		RDID_FLASH					0x9F
#define		RDSR_FLASH					0x05
#define		WRSR_FLASH					0X01
#define		READ_FLASH					0X03
#define 	FAST_READ_FLASH			0x0B
#define 	PP		        			0x02
#define 	SE									0xD8
#define		BE									0xC7
#define 	DP									0xB9
#define 	RES									0xAB


void BSP_M25P16_Init_Port(void);
void BSP_M25P16_Default_Port(void);

u32 BSP_M25P16_Init(void);
void BSP_M25P16_Enable(void);
void BSP_M25P16_Default(void);
	
u32 BSP_M25P16_ReadId(void);
u8 BSP_M25P16_WriteData(u8 sector_addr, u8 page_addr, u8 byte_addr, u16 num, u8 *pointer);
u8 BSP_M25P16_ReadData(u8 sector_addr, u8 page_addr, u8 byte_addr ,u16 num, u8 *p_back);
u8 BSP_M25P16_ErasureSector(u8 sector_addr);
u8 BSP_M25P16_ErasureAll(void);












#endif
