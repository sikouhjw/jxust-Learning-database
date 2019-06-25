/***************************************************************************
标题：	DS1302驱动程序
实验板：电子时钟套件
作者：	
客服QQ：
邮箱：	
说明：	程序仅供学习参考
******************************************************************************/
#include <reg52.h>

/********IO引脚定义***********************************************************/
sbit SCK=P1^2;		//DS1302的SCL时钟引脚
sbit SDA=P1^1;		//DS1302的IO脚
sbit RST = P1^0;  // DS1302复位脚（可以当作片选用）

/********数据定义*************************************************************/
code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; 	//写时间信息地址
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};	//读时间信息地址

/************写字节到DS1302*******************************************/
void Write_Ds1302_Byte(unsigned  char dat) 
{
	unsigned char i;
	for (i=0;i<8;i++)     		//写一个字节
	{ 
		SCK=0;
		SDA=dat&0x01;     		//获取低位数据，低位在前高位在后
		dat>>=1;  					//右移一位
		SCK=1;
	}
	SDA=1;
}   

/************根据地址写数据*************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	SCK=0;						//准备
 	RST=1;						//开始

 	Write_Ds1302_Byte(address);	//发送地址
 	Write_Ds1302_Byte(dat);		//发送数据
 	RST=0;  					//恢复
}

/***********根据地址读数据*************************************************/
unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,la=0x00;
 	SCK=0;
	SDA=1;
 	RST=1;

 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 			//读一字节
 	{
		la>>=1;							//右移一位，低位在前高位在后
		SCK=0;			  
 		if(SDA)
 			la|=0x80;					//低位在前高位在后，数据1时从高位补入
		SCK=1;						
	} 
 	RST=0;
	return (la);				//返回相应值
}

/**********读秒，分，时，日，月，星期，年*************************************/
void Read_RTC(unsigned char *ptimer,unsigned char llen)		//读取 日历，可按个数来读取顺序是秒，分，时，日，月，星期，年
{
 	unsigned char i,*p;
 	p=read_rtc_address; 				//读地址
 	for(i=0;i<llen;i++)					//
 	{
 		 ptimer[i]=Read_Ds1302(*p);
 		 p++;
 	}
}

/**********写秒，分，时，日，月，星期，年*************************************/
void Set_RTC(unsigned char *ptimer)
{
	unsigned char i,*p,tmp;
	for(i=0;i<7;i++){						//将数据转为BCD码
		tmp=ptimer[i]/10;
		ptimer[i]=ptimer[i]%10;
		ptimer[i]=ptimer[i]+tmp*16;
	}
 	Write_Ds1302(0x8E,0X00);		//打开写允许
	
 	p=write_rtc_address;				//写地址	
	for(i=0;i<7;i++)						//最多7个写入 顺序是秒，分，时，日，月，星期，年
	{
		Write_Ds1302(*p,ptimer[i]);
		p++;  
	}
	Write_Ds1302(0x8E,0x80);		//关闭写允许
}