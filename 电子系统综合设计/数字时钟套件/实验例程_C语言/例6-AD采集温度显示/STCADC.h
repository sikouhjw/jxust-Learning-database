/***************************************************************************
标题：	ADC采集驱动程序
实验板：电子时钟套件
作者：	
客服QQ：
邮箱：	
说明：	程序仅供学习使用
******************************************************************************/

#include <reg52.h>

sfr ADC_CONTR = 0XBC;	//转换控制寄存器
sfr ADC_RES = 0XBD;		//转换结果高8位
sfr ADC_LOW2 = 0XBE;	//转换结果低2位
sfr P1ASF = 0X9D;			//I/O口模式

#define ADC_POWER 0X80		//电源位
#define ADC_FLAG 	0X10		//标置位
#define ADC_START 0X08		//开启位

//=======延时============
void ADdelay(unsigned char i)
{
	while(i--);
}
//=======初始化=========
void InitADC()
{
	P1ASF=0xC0;    				//设P1.6和P1.7为ADC转换
	ADC_RES=0;
	ADC_CONTR=ADC_POWER; //启动ADC电源
	ADdelay(200);	
}
//=======P1AD转换启动=======
void ADCP1STA(unsigned char lch)
{
	ADC_CONTR=ADC_POWER|ADC_START|lch; //选择一路启动转换采集
}
//=======获取转换结果=======
unsigned char GetADC(void)
{
	while(!(ADC_CONTR&ADC_FLAG));
	ADC_CONTR&=~ADC_FLAG;      //停止AD转换
	return ADC_RES;
}
