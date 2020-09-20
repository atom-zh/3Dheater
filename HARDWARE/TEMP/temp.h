#ifndef __TEMP_H
#define __TEMP_H	
#include "sys.h"

#define	Success			0x01
#define	Fail				0x00
#define NTC_10K			10    //NTC_10K������
#define NTC_50K			50    //NTC_50K������

#define Length_10K				140   	 //240��Temp_Tab_10K����ĳ���
#define Length_50K	      179    //179��Temp_Tab_50K����ĳ���
#define Power_Vol					3.3			//3.3V�Ǵ�����·�еĵ�Դ��ѹ���ɸ���ʵ�ʵ�·�޸�
#define RES_10K						10			//��λk�����¶ȴ�����������ѹ���裬�ɸ���ʵ�ʵ�·�޸�
#define RES_50K						51			//��λk�����¶ȴ�����������ѹ���裬�ɸ���ʵ�ʵ�·�޸�

void Temp_Init(void);
float Get_Temp(void); 
 
#endif 
