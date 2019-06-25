/***************************************************************************
标题：	时钟程序
实验板：电子时钟套件
作者：	芯嵌
客服QQ：
邮箱：	
说明：	时间显示+可调
操作：长按S2按钮进入时间设置，按顺序从分，时，日，月，星期，年进行调节，
      S1键确认，S2键切换，
	  S1键对值加1改变，直到到全调好再按S2进行保存退出
******************************************************************************/

//头文件:
#include <reg52.h>
#include "SMG4.h"
#include "DS1302.h"

//引脚定义:
sbit S1 = P3^1;	//定义按键1
sbit S2 = P3^0;	//定义按键2

sbit SPK = P1^5;	//蜂鸣器控制引脚

//常量定义
code unsigned char ly_Tup[]={59,59,23,31,12,7,99};		//设置调整时时做最大值判断
							//钞=59，分=59	，时=23，日=31，月=12，星期=7，年=99

//变量定义
unsigned char ly_timer[7]={0,20,12,9,7,4,15};	//0秒，20分，12时，9日，7月，星期4，15年			
																							//用于读时间信息缓冲，初值为第一次的写入时间				
unsigned char ly_key;				//保存键值
unsigned char ly_keyon=0;		//作为按钮是否松开的判断


//延时子函数
void delay(unsigned char i){
	while(i--);
}

//扫描键盘输入
void ReadKey(void)
{
	if(!(S1&S2))					//先判断是否有键按下
	{
		SPK=0;							//有键按下，发出蜂鸣提示音
		delay(100);					//这里的延时是消除干扰
		if((!(S1&S2))&&(ly_keyon==0))	//确定有按键按下 且是刚按下执行，否则不执行
		{			
			if(S1==0){				//按键1按下
				ly_key=1;				//赋值
				ly_keyon=1;			//处理完，将按键视为按下
			}
			else if(S2==0){	//按键2按下，同上
				ly_key=2;			
				ly_keyon=1;
			}
		}
	}
	else
	{
		ly_keyon=0;	//	无按钮按下
		SPK=1; 			// 关闭蜂鸣器
	}
}

//调整时间设定函数
void SetRTC(void)
{
	unsigned char i=0,tmp;
	unsigned char lcc=1;		//作为调整位置的索引号，从1也就是‘分’开始设置，秒不调，归0
	unsigned char lt[7];		//时间缓存

	ReadKey();							//先做一次键盘扫描，然后将键值复制0，不让设置按键影响后面的处理
	ly_key=0;
	
	for(i=1;i<7;i++){				//将数据BCD码转为二进制
		tmp=ly_timer[i]>>4;		
		lt[i]=ly_timer[i]&0x0f;
		lt[i]=tmp*10+lt[i];	
	}
	tmp=0;
	
	while(1){
		DisPlayS(15);					//调用显示带延时的显示函数，闪烁的快慢由这个延时参数决定
		ReadKey();						//键盘扫描
		
		if(ly_key)						//有键按下处理
		{
			switch(ly_key){
			case 1:								//1键值加1，当小于设定的最大值时加1，否则回0
				if(lt[lcc]<ly_Tup[lcc])
					lt[lcc]++;
				else
					lt[lcc]=0;
				break;
			case 2:								//2键切换
				lcc++;
				if(lcc>6){					//当时间值都设置完成后保存退出
					lt[0]=0;   			 	//秒归0
					Set_RTC(lt);
					SPK=1;						//当我们最后按下切换键时，蜂鸣器是被打开的，所以退出时要将蜂鸣器关闭
					return;
				}
				break;
			}
			ly_key=0;
		}
		
		if(tmp==0)	//闪烁效果，通过这个常量判断相应位的显示与不显示的轮流，为0时显示，为180时关闭显示
		{
			switch(lcc){		//由这个变量来决定显示相应的内容
			case 1:
			case 2:
				ly_SMGcp[1]=1;			//时分中间两个点亮起
				ly_SMGcp[2]=1;
			
				ly_SMGbuf[0]=(lt[2]/10);		//时
				ly_SMGbuf[1]=(lt[2]%10);

				ly_SMGbuf[2]=(lt[1]/10);		//分
				ly_SMGbuf[3]=(lt[1]%10);
				break;
			case 3:
			case 4:
				ly_SMGcp[1]=0;
				ly_SMGcp[2]=1;							//显示‘日月’时用一个点隔开
			
				ly_SMGbuf[0]=(lt[4]/10);		//月
				ly_SMGbuf[1]=(lt[4]%10);

				ly_SMGbuf[2]=(lt[3]/10);		//日
				ly_SMGbuf[3]=(lt[3]%10);
				break;
			case 5:
				ly_SMGcp[1]=0;
				ly_SMGcp[2]=0;						//只显示一位，不显示点
				ly_SMGbuf[0]=10;					//这里的10在段值表中的值为OXFF，即关闭数码管显示
				ly_SMGbuf[1]=10;
				ly_SMGbuf[2]=10;
			
				ly_SMGbuf[3]=lt[5];				//星期
				break;
			case 6:
				ly_SMGbuf[0]=2;
				ly_SMGbuf[1]=0;			
				ly_SMGbuf[2]=(lt[6]/10);		//年
				ly_SMGbuf[3]=(lt[6]%10);
				break;	
			}
		}
		if(tmp==180)	//关闭相应位显示，以达闪烁效果
		{
			switch(lcc){				
			case 1:
			case 3:
			case 5:
				ly_SMGbuf[2]=10;	//这里的10在段值表中的值为OXFF，即关闭数码管显示
				ly_SMGbuf[3]=10;
				break;
			case 2:
			case 4:
				ly_SMGbuf[0]=10;	
				ly_SMGbuf[1]=10;
				break;
			case 6:
				ly_SMGbuf[0]=10;	
				ly_SMGbuf[1]=10;
				ly_SMGbuf[2]=10;	
				ly_SMGbuf[3]=10;
				break;
			}
		}
		tmp++;
	}
}


//主函数	
void main(void)  
{
	unsigned char oldT,DisUp=1;	//做为时间更新的显示的控制
	
	RST=0;											//DS1302的RST是低电平复位，因此将其关闭

	Set_RTC(ly_timer);					//写时间，芯片初始化需要先写入时间进行测试，非测试时此行去掉。
		
	while(1){
		if(S2==0){				//长按下2键进入调时
			DisPlayS(5000);	//调用显示函数延时为目的
			if(S1==0){			//再次确定长按进入
				SetRTC();
				DisUp=1;			//设置完成后更新显示
			}
		}
		
		if(DisUp){				//更新显示时间
			DisUp=0;
			Read_RTC(ly_timer,7);						//读时间信息
				
			ly_SMGbuf[0]=ly_timer[2]/16;		//提取小时的个位和十位，存入显示缓冲待显示
			ly_SMGbuf[1]=ly_timer[2]&0x0f;	//为什么不是除以10，这里一定注意，读出来的是BCD十进制码
			ly_SMGbuf[2]=ly_timer[1]/16;		//提取分种的个位和十位，存入显示缓冲待显示
			ly_SMGbuf[3]=ly_timer[1]&0x0f;
		}
		
		Read_RTC(ly_timer,1);					//读时间信息秒	
		if(ly_timer[0]!=oldT)  				//秒跳变更新中间点闪烁
		{
			oldT=ly_timer[0];
			ly_SMGcp[1]=oldT&0x01;			//获取秒的第0位来让中间两个小数点闪烁
			ly_SMGcp[2]=ly_SMGcp[1];
			if(oldT==0)									//秒归0时更新时间显示
				DisUp=1;
		}
		
		DisPlayS(100);	//调用显示函数
	}
}

