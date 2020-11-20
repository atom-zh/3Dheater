#include "temp.h"
#include "delay.h"
#include "usart.h"

const double Temp_Tab_10K[]=															//3950-10K阻值表
{
 179.2666, 168.4053, 158.2726, 148.8151, 139.9837, 131.7332, 124.0216, 116.8107, 110.0648, 103.7512,  //-30
 97.8396, 92.3020, 87.1124, 82.2471, 77.6837, 73.4018, 69.3823, 65.6077, 62.0616, 58.7288,            //-20
 55.5953, 52.6480, 49.8747, 47.2643, 44.8062, 42.4906, 40.3086, 38.2516, 36.3117, 34.4817,            //-10
 32.7547, 31.1243, 29.5847, 28.1301, 26.7556, 25.4562, 24.2274, 23.0650, 21.9650, 20.9239,            //0
 19.9380, 19.0041, 18.1193, 17.2807, 16.4857, 15.7317, 15.0164, 14.3376, 13.6933, 13.0816,            //10
 12.5005, 11.9485, 11.4239, 10.9252, 10.4510, 10.0000, 9.5709, 9.1626, 8.7738, 8.4037,                //20
 8.0512, 7.7154, 7.3953, 7.0903, 6.7995, 6.5221, 6.2576, 6.0051, 5.7642, 5.5342,                      //30
 5.3146, 5.1049, 4.9045, 4.7130, 4.5300, 4.3551, 4.1878, 4.0278, 3.8748, 3.7283,                      //40
 3.5882, 3.4540, 3.3255, 3.2025, 3.0846, 2.9717, 3.8635, 2.7597, 2.6603, 2.5649,                      //50
 2.4734, 2.3856, 2.3014, 2.2206, 2.1431, 2.0686, 1.9970, 1.9283, 1.8623, 1.7989,                      //60
 1.7380, 1.6794, 1.6231, 1.5689, 1.5168, 1.4667, 1.4185, 1.3722, 1.3275, 1.2845,                      //70
 1.2431, 1.2033, 1.1649, 1.1279, 1.0923, 1.0580, 1.0249, 0.9930, 0.9623, 0.9326,                      //80
 0.9040, 0.8764, 0.8498, 0.8241, 0.7994, 0.7754, 0.7523, 0.7300, 0.7085, 0.6877,                      //90
 0.6676, 0.6482, 0.6295, 0.6113, 0.5938, 0.5769, 0.5605, 0.5447, 0.5293, 0.5145,                      //100
 0.5002                                                                                               //110
};

const double Temp_Tab_50K[]={															//3950-50K阻值表
	
	911.9977,866.0101,821.9230,779.4510,738.4196,698.7468,660.4077,623.4194,587.8218,553.6648,	//-30~-21
	521.0003,489.8737,460.3197,432.3588,405.9971,381.2256,358.0213,336.3482,316.1589,297.3974,	//-20~-11
	279.9999,263.8976,249.0181,235.2865,222.6276,214.4699,203.4380,193.0172,183.1747,173.8790,	//-10~-1
	165.1000,156.8085,148.9771,141.5792,134.5901,127.9859,121.7443,115.8441,110.2652,104.9888,	//0~9
	99.9973,95.2737,90.8026,86.5690,82.5591,78.7598,75.1590,71.7452,68.5070,65.4358,	//10~19
	62.5207,59.7532,57.1250,54.6283,52.2556,50.0000,47.8551,45.8148,43.8733,42.0225,	//20~29
	40.2656,38.5896,36.9928,35.4710,34.0201,32.6366,31.3169,30.0577,28.8558,27.7084,	//30~39
	26.6126,25.5660,24.5659,23.6101,22.6965,21.8228,20.9872,20.1879,19.4230,18.6909,	//40~49
	17.9900,17.3189,16.6761,16.0604,15.4704,14.9050,14.3631,13.8434,13.3451,12.8672,	//50~59
	12.4087,11.9687,11.5465,11.1412,10.7522,10.3786,10.0197, 9.6751, 9.3440, 9.0258,	//60~69
	8.7200,8.4260,8.1434,7.8717,7.6104,7.3590,7.1172,6.8845,6.6606,6.4451,	//70~79
	6.2377,6.0379,5.8456,5.6603,5.4819,5.3100,5.1444,4.9847,4.8309,4.6826,	//80~89
	4.5396,4.4017,4.2688,4.1405,4.0168,3.8892,3.7755,3.6660,3.5603,3.4584,	//90~99
	3.3600,3.2650,3.1732,3.0844,2.9986,2.9156,2.8353,2.7575,2.6822,2.6092,	//100~109
	2.5385,2.4700,2.4036,2.3392,2.2767,2.2161,2.1573,2.1003,2.0449,1.9912,	//110~119
	1.9390,1.8884,1.8392,1.7914,1.7450,1.7000,1.6563,1.6138,1.5725,1.5324,	//120~129
	1.4934,1.4556,1.4188,1.3831,1.3484,1.3147,1.2820,1.2502,1.2193,1.1892,	//130~139
	1.1600,1.1317,1.1041,1.0774,1.0513,1.0261,1.0015,0.9776,0.9544,0.9319 	//140~149
};

const double Temp_Tab_100K[]=															//3950-100K阻值表
{
    179.2666, 168.4053, 158.2726, 148.8151, 139.9837, 131.7332, 124.0216, 116.8107, 110.0648, 103.7512,  //-30
    97.8396, 92.3020, 87.1124, 82.2471, 77.6837, 73.4018, 69.3823, 65.6077, 62.0616, 58.7288,            //-20
    55.5953, 52.6480, 49.8747, 47.2643, 44.8062, 42.4906, 40.3086, 38.2516, 36.3117, 34.4817,            //-10
    32.7547, 31.1243, 29.5847, 28.1301, 26.7556, 25.4562, 24.2274, 23.0650, 21.9650, 20.9239,            //0
    19.9380, 19.0041, 18.1193, 17.2807, 16.4857, 15.7317, 15.0164, 14.3376, 13.6933, 13.0816,            //10
    12.5005, 11.9485, 11.4239, 10.9252, 10.4510, 10.0000, 9.5709, 9.1626, 8.7738, 8.4037,                //20
    8.0512, 7.7154, 7.3953, 7.0903, 6.7995, 6.5221, 6.2576, 6.0051, 5.7642, 5.5342,                      //30
    5.3146, 5.1049, 4.9045, 4.7130, 4.5300, 4.3551, 4.1878, 4.0278, 3.8748, 3.7283,                      //40
    3.5882, 3.4540, 3.3255, 3.2025, 3.0846, 2.9717, 3.8635, 2.7597, 2.6603, 2.5649,                      //50
    2.4734, 2.3856, 2.3014, 2.2206, 2.1431, 2.0686, 1.9970, 1.9283, 1.8623, 1.7989,                      //60
    1.7380, 1.6794, 1.6231, 1.5689, 1.5168, 1.4667, 1.4185, 1.3722, 1.3275, 1.2845,                      //70
    1.2431, 1.2033, 1.1649, 1.1279, 1.0923, 1.0580, 1.0249, 0.9930, 0.9623, 0.9326,                      //80
    0.9040, 0.8764, 0.8498, 0.8241, 0.7994, 0.7754, 0.7523, 0.7300, 0.7085, 0.6877,                      //90
    0.6676, 0.6482, 0.6295, 0.6113, 0.5938, 0.5769, 0.5605, 0.5447, 0.5293, 0.5145,                      //100
    0.5002                                                                                               //110
};

//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Temp_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
    //ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  

//获得ADC值
//ch:通道值 0~3
static u16 Get_Vol(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

static u16 Get_Vol_Avg(u8 ch,u8 times)
{
	u32 vol=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		vol+=Get_Vol(ch);
		delay_ms(5);
	}
	return vol/times;
} 	 

/******************************************
* 函数名: Binary_Search
* 描  述: 折半查找法
* 输  入: 需要查找的数，查找的数组，数组的最大值
* 返  回: 查找数的位置。
******************************************/
static int Binary_Search(double Search_Num,double const *Temp_Tab,unsigned char High)	//折半查找法
{
	int Low = 0,Mid;
	if(Search_Num > Temp_Tab[0]) 
	{
		return 0;
	}else
	{
		if(Search_Num < Temp_Tab[High])
		{
			return High+1;
		}
	}
	while (Low < High) 
	{
		Mid = (Low + High)/2 + 0.5;
		if (Search_Num < Temp_Tab[Mid]) 
		{
			Low = Mid+1;
			if(Search_Num > Temp_Tab[Low]) 
			{
				return Mid;
			}else
			{
				if(Search_Num == Temp_Tab[Low])
				{
					return Low;
				}
			}
		}else
		{
			if(Search_Num > Temp_Tab[Mid])
			{
				High = Mid-1;
				if(Search_Num < Temp_Tab[High]) 
				{
					return Mid-1;
				}else
				{
					if(Search_Num == Temp_Tab[High])
					{
						return High;
					}
				}
			}else
			{
				return Mid;
			}		
		}
	}
	return Mid;
}

/******************************************
* 函数名: Check_Vol
* 描  述: 判断读回来的电压
* 输  入: 电压，传感器型号
* 返  回: 判断状态
******************************************/
static unsigned char Check_Vol(double Voltage,unsigned char NTC)		//判断读回来的电压
{
	if(0.0 == Voltage)
	{
		printf("Error:NTC_%dK传感器短路！\n",NTC);			            //水温读取错误次数
		return Fail;
	}else
	{
		if(Power_Vol == Voltage)
		{
			printf("Error:NTC_%dK传感器未连接或串联电阻短路！\n",NTC);			//水温读取错误次数
			return Fail;
		}
	}
	return Success;
}

/******************************************
* 函数名: Read_ADC_Data_10K
* 描  述: 读NTC_10K传感器温度
* 输  入: 无
* 返  回: 无
******************************************/
static float Read_ADC_Data_10K(void)			//读取ADC值
{
	float Temp_10K;
	double Voltage,Resistance;		//电压,电阻
	unsigned char Binaty;
	unsigned int ADC_Value;
    //unsigned char List_Length;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADC通道3,规则采样顺序值为1,采样时间为239.5周期         
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);        //启动上面设置好的一个通道，进行转换       
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//等待EOC置位
	ADC_Value = ADC_GetConversionValue(ADC1);  		//把数据寄存器的值读走
	
	DEBUG("ADC_Value_10K：%d\n",ADC_Value);
	Voltage = Power_Vol*ADC_Value/4095;					//3.3V是串联电路中的电源电压，可根据实际电路修改
	DEBUG("Voltage_10K：%f\n",Voltage);
	if(Fail == Check_Vol(Voltage,NTC_10K))			//判断读回来的电压
	{
		return -1;
	}
	Resistance = (RES_10K*Voltage/(Power_Vol-Voltage));		//RES_50K单位kΩ；温度传感器串联分压电阻，可根据实际电路修改
	DEBUG("Resistance_10K：%f\n",Resistance);
	Binaty = Binary_Search(Resistance,Temp_Tab_10K,Length_10K);			//140是Temp_Tab_50K数组的长度
	DEBUG("Binary_Search_10K：%d\n",Binaty);
	Temp_10K = Binaty - 30;										//数组起始是-30度，这里需要减30；
	printf("NTC_10K温度：%0.1f℃。\n", Temp_10K);
	return Temp_10K;
}

/******************************************
* 函数名: Read_ADC_Data_50K
* 描  述: 读NTC_50K传感器温度
* 输  入: 无
* 返  回: 无
******************************************/
static float Read_ADC_Data_50K(void)			//读取ADC值
{
	char Temp;
	double Voltage,Resistance;			//电压,电阻
	unsigned char Binaty;
	unsigned int ADC_Value;
    //unsigned char List_Length;

	ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADC通道3,规则采样顺序值为1,采样时间为239.5周期         
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);        //启动上面设置好的一个通道，进行转换       
	while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);//等待EOC置位
	ADC_Value = ADC_GetConversionValue(ADC2);   	 //把数据寄存器的值读走
	
	DEBUG("ADC_Value_50K：%d\n",ADC_Value);
	Voltage = Power_Vol*ADC_Value/4095;					//3.3V是串联电路中的电源电压，可根据实际电路修改
	DEBUG("Voltage_50K：%f\n",Voltage);
	if(Fail == Check_Vol(Voltage,NTC_50K))			//判断读回来的电压
	{
		return -1;
	}
	Resistance = (RES_50K*Voltage/(Power_Vol-Voltage));		//RES_50K单位kΩ；温度传感器串联分压电阻，可根据实际电路修改
	DEBUG("Resistance_50K：%f\n",Resistance);
    //List_Length=(sizeof(Temp_Tab_50K)/sizeof(Temp_Tab_50K[0]))-1;  //方法用于获取数组长度,对于指针不适用,数组占内存总空间，除以单个元素占内存空间大小
    //DEBUG("Temp_Tab_50K数组长度：%d。\n",List_Length);	
	Binaty = Binary_Search(Resistance,Temp_Tab_50K,Length_50K);			//179是Temp_Tab_50K数组的长度
	DEBUG("Binary_Search_50K：%d\n",Binaty);
	Temp = Binaty - 30;										//数组起始是-30度，这里需要减30；
	printf("NTC_50K温度：%d℃。\n",Temp);
	return Temp;
}

/******************************************
* 函数名: Read_ADC_Data_100K
* 描  述: 读NTC_100K传感器温度
* 输  入: 无
* 返  回: 无
******************************************/
static float Read_ADC_Data_100K(void)			//读取ADC值
{
	float Temp_100K;
	double Voltage,Resistance;		            //电压,电阻
	unsigned char Binaty;
	unsigned int ADC_Value;
    //unsigned char List_Length;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADC通道3,规则采样顺序值为1,采样时间为239.5周期         
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);        //启动上面设置好的一个通道，进行转换       
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//等待EOC置位
	ADC_Value = ADC_GetConversionValue(ADC1);  		//把数据寄存器的值读走
	
	DEBUG("ADC_Value_100K：%d\n",ADC_Value);
	Voltage = Power_Vol*ADC_Value/4095;					//3.3V是串联电路中的电源电压，可根据实际电路修改
	DEBUG("Voltage_100K：%f\n",Voltage);
	if(Fail == Check_Vol(Voltage,NTC_100K))			//判断读回来的电压
	{
		return -1;
	}
	Resistance = (RES_100K*Voltage/(Power_Vol-Voltage));		//RES_50K单位kΩ；温度传感器串联分压电阻，可根据实际电路修改
	DEBUG("Resistance_100K：%f\n",Resistance);
	Binaty = Binary_Search(Resistance,Temp_Tab_100K,Length_100K);			//140是Temp_Tab_50K数组的长度
	DEBUG("Binary_Search_100K：%d\n",Binaty);
	Temp_100K = Binaty - 30;										//数组起始是-30度，这里需要减30；
	printf("NTC_100K 温度：%0.1f℃。\n", Temp_100K);
	return Temp_100K;
}

float Get_Temp(void)
{
  return Read_ADC_Data_100K();
} 	 

