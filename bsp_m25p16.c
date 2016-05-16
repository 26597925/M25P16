#include "bsp_m25p16.h"

/*
M25P16说明：
共2M*8bit空间
扇区：0-31
每个扇区内页地址：0-255
每页包含字节：0-255
*/

/*
需要移植的代码：
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//接口函数-M25P16 10ms级延时
#define BSP_M25P16_Delay10ms_Port(ms)		CoTickDelay(ms)

//接口函数-M25P16 SPI读写一个字节
#define BSP_M25P16_ReadWrite_Port(data)		HAL_SPI2_ReadWriteByte(data)

//接口函数-M25P16 片选使能
#define BSP_M25P16_NSS_EN		GPIO_ResetBits(GPIOC, GPIO_Pin_6)

//接口函数-M25P16 片选失能
#define BSP_M25P16_NSS_DN		GPIO_SetBits(GPIOC, GPIO_Pin_6)

//接口函数-M25P16 IO初始化
__inline void BSP_M25P16_Init_Port(void)
{
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	//GPIOD5-NSS
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	BSP_M25P16_NSS_DN;		//NSS失能
	
	//SPI2引脚初始化 SCK MOSI MISO
	HAL_SPI2_Init();
}

//接口函数-M25P16 IO使能
__inline void BSP_M25P16_Enable_Port(void)
{
	//GPIOD5-NSS
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	HAL_SPI2_Enable();
}

//接口函数-M25P16 IO失能
__inline void BSP_M25P16_Default_Port(void)
{
	//GPIOD5-NSS
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	HAL_SPI2_Default();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 ************************************************************
 *  名称：	BSP_M25P16_Init()
 *  功能：	FLASH初始化
 *	输入：  无
 *	输出：  FLASH ID号
 ************************************************************
*/
u32 BSP_M25P16_Init(void)
{
	BSP_M25P16_Init_Port();
	return BSP_M25P16_ReadId();
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_Enable()
 *  功能：	FLASH 使能
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
void BSP_M25P16_Enable(void)
{
	BSP_M25P16_Enable_Port();
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_Default()
 *  功能：	FLASH 失能
 *	输入：  无
 *	输出：  FLASH ID号
 ************************************************************
*/
void BSP_M25P16_Default(void)
{
	BSP_M25P16_Default_Port();
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_ReadId()
 *  功能：	M25P16 读ID号
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
u32 BSP_M25P16_ReadId(void)
{
	u32 temp = 0;
	
	BSP_M25P16_NSS_EN;
	
	BSP_M25P16_ReadWrite_Port(RDID_FLASH);
	
	temp |= ((u32)BSP_M25P16_ReadWrite_Port(0) << 16);
  temp |= ((u32)BSP_M25P16_ReadWrite_Port(0) << 8);
  temp |= BSP_M25P16_ReadWrite_Port(0);
  
	BSP_M25P16_NSS_DN;
  
	return temp;
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_ReadState()
 *  功能：	M25P16 读状态寄存器
 *	输入：  无
 *	输出：  状态寄存器数值
 ************************************************************
*/
u8 BSP_M25P16_ReadState(void)
{
	u8 temp = 0;
	
  BSP_M25P16_NSS_EN;
  
	BSP_M25P16_ReadWrite_Port(RDSR_FLASH);
  temp = BSP_M25P16_ReadWrite_Port(0);
  
	BSP_M25P16_NSS_DN;
  
	return temp;
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_WriteEnable()
 *  功能：	M25P16 写使能
 *	输入：  无
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_M25P16_WriteEnable(void)
{
  BSP_M25P16_NSS_EN;
  BSP_M25P16_ReadWrite_Port(WREN_FLASH);
  BSP_M25P16_NSS_DN;
  
	u8 temp = BSP_M25P16_ReadState();
  
	if (temp & 0x02)
    return true;
  else
    return false;
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_WriteState()
 *  功能：	M25P16 写状态寄存器
 *	输入：  data-写入状态寄存器的值
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_M25P16_WriteState(u8 data)
{
	if(false == BSP_M25P16_WriteEnable()) return false;
	
	BSP_M25P16_NSS_EN;
  BSP_M25P16_ReadWrite_Port(WRSR_FLASH);
  BSP_M25P16_ReadWrite_Port(data);
  BSP_M25P16_NSS_DN;
  
	return true;
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_ReadBusy()
 *  功能：	M25P16 读取忙状态
 *	输入：  无
 *	输出：  0-不忙  1-忙
 ************************************************************
*/
u8 BSP_M25P16_ReadBusy(void)
{
	u8 temp = BSP_M25P16_ReadState();
  if(temp & 0x01)	return 1;
  else return 0;
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_WriteData()
 *  功能：	M25P16 页写入程序 (需要先擦除，才能写入)
 *	输入：  sector_addr-扇区地址  page_addr-页地址  byte_addr-字节地址  num-写入个数  pointer-写入数据的指针
 *	输出：  0-写入成功  1-字节数为0  2-FLASH忙  3-FLASH使能失败
 ************************************************************
*/
u8 BSP_M25P16_WriteData(u8 sector_addr, u8 page_addr, u8 byte_addr, u16 num, u8 *pointer)
{
  u16 i;
	
  if (num == 0)   return 1;
  
	i=100;
	while(1)
	{
		if (0 == BSP_M25P16_ReadBusy()) break;
		
		BSP_M25P16_Delay10ms_Port(1);	//延时10ms
		i--;
		if(i==0) return 2;
	}
	
  if (false == BSP_M25P16_WriteEnable()) return 3;
	
  BSP_M25P16_NSS_EN;
  
	BSP_M25P16_ReadWrite_Port(PP);
  BSP_M25P16_ReadWrite_Port(sector_addr);
  BSP_M25P16_ReadWrite_Port(page_addr);
  BSP_M25P16_ReadWrite_Port(byte_addr);
  for (i = 0; i < num; i++)
  {
    BSP_M25P16_ReadWrite_Port(*(pointer + i));
  }
	
  BSP_M25P16_NSS_DN;
  
//	i=100;
//	while(1)
//	{
//		if (0 == BSP_M25P16_ReadBusy()) break;
//		
//		BSP_M25P16_Delay10ms_Port(1);	//延时10ms
//		i--;
//		if(i==0) return 4;
//	}
	
  return 0;
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_ReadData()
 *  功能：	M25P16 页读取 
 *	输入：  sector_addr-扇区地址  page_addr-页地址  num-读取个数  p_back-读取缓冲区的首地址
 *	输出：  0-读取成功  1-字节数为0  2-FLASH忙
 ************************************************************
*/
u8 BSP_M25P16_ReadData(u8 sector_addr, u8 page_addr, u8 byte_addr ,u16 num, u8 *p_back)
{
  u16 i;
	
  if (num == 0)   return 1;
	
  i=100;
	while(1)
	{
		if (0 == BSP_M25P16_ReadBusy()) break;
		
		BSP_M25P16_Delay10ms_Port(1);	//延时10ms
		i--;
		if(i==0) return 2;
	}
  
  BSP_M25P16_NSS_EN;
  
	BSP_M25P16_ReadWrite_Port(READ_FLASH);
  BSP_M25P16_ReadWrite_Port(sector_addr);
  BSP_M25P16_ReadWrite_Port(page_addr);
  BSP_M25P16_ReadWrite_Port(byte_addr);
  for (i = 0; i < num; i++)
  {
    *(p_back + i) = BSP_M25P16_ReadWrite_Port(0);
  }
	
  BSP_M25P16_NSS_DN;
	
//	i=100;
//	while(1)
//	{
//		if (0 == BSP_M25P16_ReadBusy()) break;
//		
//		BSP_M25P16_Delay10ms_Port(1);	//延时10ms
//		i--;
//		if(i==0) return 3;
//	}
  
  return 0;
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_ErasureSector()
 *  功能：	M25P16 扇区擦除 
 *	输入：  sector_addr-扇区地址
 *	输出：  0-擦除成功  1-FLASH忙  2-FLASH使能失败
 ************************************************************
*/
u8 BSP_M25P16_ErasureSector(u8 sector_addr)
{
  u8 i=100;
	while(1)
	{
		if (0 == BSP_M25P16_ReadBusy()) break;
		
		BSP_M25P16_Delay10ms_Port(1);	//延时100ms
		i--;
		if(i==0) return 1;
	}
	
  if (false == BSP_M25P16_WriteEnable()) return 2;
    
  BSP_M25P16_NSS_EN;
  
	BSP_M25P16_ReadWrite_Port(SE);
  BSP_M25P16_ReadWrite_Port(sector_addr);
  BSP_M25P16_ReadWrite_Port(0);
  BSP_M25P16_ReadWrite_Port(0);
  
	BSP_M25P16_NSS_DN;
	
	i=40;		//最高擦除时间3S
	while(1)
	{
		if (0 == BSP_M25P16_ReadBusy()) break;
		
		BSP_M25P16_Delay10ms_Port(10);	//延时100ms
		i--;
		if(i==0) return 3;
	}

  return 0;
}

/*
 ************************************************************
 *  名称：	BSP_M25P16_ErasureAll()
 *  功能：	M25P16 擦除所有数据
 *	输入：  sector_addr-扇区地址  page_addr-页地址  byte_addr-字节地址
 *	输出：  0-格式化成功  1-FLASH忙  2-FLASH使能失败
 ************************************************************
*/
u8 BSP_M25P16_ErasureAll(void)
{
	u8 i=100;
	while(1)
	{
		if (0 == BSP_M25P16_ReadBusy()) break;
		
		BSP_M25P16_Delay10ms_Port(1);	//延时10ms
		i--;
		if(i==0) return 1;
	}
	
  if (false == BSP_M25P16_WriteEnable()) return 2;

  BSP_M25P16_NSS_EN;
	
  BSP_M25P16_ReadWrite_Port(BE);
	
  BSP_M25P16_NSS_DN;
	
//	i=100;
//	while(1)
//	{
//		if (0 == BSP_M25P16_ReadBusy()) break;
//		
//		BSP_M25P16_Delay10ms_Port(1);	//延时10ms
//		i--;
//		if(i==0) return 3;
//	}
  
  return 0;
}
