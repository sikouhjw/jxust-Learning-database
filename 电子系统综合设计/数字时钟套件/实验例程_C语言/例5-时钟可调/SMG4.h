/***************************************************************************
标题：	四位数码管驱动
实验板：电子时钟套件
作者：	
客服QQ：
邮箱：	
说明：	程序仅供学习使用
******************************************************************************/

//头文件:
#include <reg52.h>

//引脚定义:		
sbit SMG_q = P3^2;	//定义数码管阳级控制脚（千位）
sbit SMG_b = P3^3;	//定义数码管阳级控制脚（百位）
sbit SMG_s = P3^4;	//定义数码管阳级控制脚（十位）
sbit SMG_g = P3^5;	//定义数码管阳级控制脚（个位）

//常量定义:
unsigned char table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};
											//表：共阳数码管 0～9 空

unsigned char table2[]={0xc0,0xcf,0xa4,0x86,0x8b,0x92,0x90,0xc7,0x80,0x82,0xff};
											//表2：共阳数码管 0～9 空，因第三位数码管是反过来安装的，因此与上面的段值有所不同

//变量定义:
unsigned char ly_SMGbuf[4]={0,0,0,0};	//数码管的显示缓冲
unsigned char ly_SMGcp[4]={0,0,0,0};  //数码管的小数点判断值，等于0时不显示小数点，否则显示


//延时子函数
void Sdelay(unsigned char i){
	while(i--);
}

//显示函数
void DisPlay(void)
{
		P2=table[ly_SMGbuf[0]];		//查表输出显示缓冲变量数组0中对应的数码管段值
		if(ly_SMGcp[0]) P2&=0x7f;	//是否加入小数点显示，等于0时不加小数点，否则显示
		SMG_q=0;									//选择千位数码管
		Sdelay(20);								//加入短暂延时，延时可控制刷新频率
		SMG_q=1;									//关闭千位数码管		
		Sdelay(2);								//延时防止托影
		
		P2=table[ly_SMGbuf[1]];		//同上
		if(ly_SMGcp[1]) P2&=0x7f;
		SMG_b=0;	
		Sdelay(20);
		SMG_b=1;
		Sdelay(2);
		
		P2=table2[ly_SMGbuf[2]];	//同上
		if(ly_SMGcp[2]) P2&=0x7f;
		SMG_s=0;
		Sdelay(20);
		SMG_s=1;
		Sdelay(2);
		
		P2=table[ly_SMGbuf[3]];
		if(ly_SMGcp[3]) P2&=0x7f;
		SMG_g=0;									//同上
		Sdelay(20);
		SMG_g=1;
		Sdelay(2);
}

//带时间的刷新输出
void DisPlayS(unsigned int t)
{
	while(t--)
		DisPlay();
}