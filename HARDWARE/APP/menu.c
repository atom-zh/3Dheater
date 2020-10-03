#include "stm32f10x.h"
#include "lcd.h"
#include "menu.h"
#include "usart.h"
#include <key.h>
 
#define Null 0
char over=0; //������־
 
//�ṹ�����Ͷ���
struct MenuItem
{  
    char MenuCount; //�ṹ�������Ԫ�ظ���
    char *DisplayString;  //��ǰLCD��ʾ����Ϣ
    void (*Subs)();  //ִ�еĺ�����ָ��.
    struct MenuItem *Childrenms;  //ָ���ӽڵ��ָ��
    struct MenuItem *Parentms;  //ָ�򸸽ڵ��ָ��
};
 
//��������
void Nop(void);
void DoSomething(struct MenuItem *manyou);
void GameOver(void);
void Locate(struct MenuItem *manyou);
void Run(struct MenuItem *manyou);

//====================================
//�ṹ������
struct MenuItem m0_main[4];
struct MenuItem m1_file[5];
struct MenuItem m1_edit[4];
struct MenuItem m1_view[2];
struct MenuItem m2_font[3];
struct MenuItem m1_help[3];

//====================================
//�ṹ��ʵ��
struct MenuItem m0_main[4]=
{
    {4,"file",Nop,m1_file,Null},
    {4,"edit",Nop,m1_edit,Null},
    {4,"view",Nop,m1_view,Null},
    {4,"help",Nop,m1_help,Null},
};

struct MenuItem m1_file[5]=
{
    {5,"open",DoSomething,Null,m0_main},
    {5,"save",DoSomething,Null,m0_main},
    {5,"save as",DoSomething,Null,m0_main},
    {5,"print",DoSomething,Null,m0_main},
    {5,"exit",GameOver,Null,m0_main},
};

struct MenuItem m1_edit[4]=
{
    {4,"undo",DoSomething,Null,m0_main},
    {4,"redo",DoSomething,Null,m0_main},
    {4,"copy",DoSomething,Null,m0_main},
    {4,"paste",DoSomething,Null,m0_main},
};

struct MenuItem m1_view[2]=
{
    {2,"font",Nop,m2_font,m0_main},
    {2,"color",DoSomething,Null,m0_main},
};

struct MenuItem m2_font[3]=
{
    {3,"songti",DoSomething,Null,m1_view},
    {3,"heiti",DoSomething,Null,m1_view},
    {3,"youyuan",DoSomething,Null,m1_view},
};

struct MenuItem m1_help[3]=
{
    {3,"index",DoSomething,Null,m0_main},
    {3,"online",DoSomething,Null,m0_main},
    {3,"about",DoSomething,Null,m0_main},
};

//====================================
//����ʵ��
void Locate(struct MenuItem *manyou)
{
    printf("Now we at %s\n" ,manyou->DisplayString);
}

void Run(struct MenuItem *manyou)
{
    (*(manyou->Subs))(manyou);
}

void Nop(void)
{}

void DoSomething(struct MenuItem *manyou)
{
    printf("we have done %s\n" ,manyou->DisplayString);
}

void GameOver(void)
{
    over=1;
    printf("Now Game is Over ");
    KEY_Scan();
}

//������
int aaaaa(void)
{
    char charin; //������ַ�
    char menuid[3]={0,0,0}; //ĳһ���Ĳ˵���ƫ�����ļ�¼����
    char i=0; //����������±�ֵ

    struct MenuItem *manyou; //����,�ڲ˵�������

    manyou=&m0_main[0]; //��ʼ��λ��
    Locate(manyou); //���

    while(!over) //�����û��
    {
	charin=KEY_Scan(); //��ȡ�����ַ�
	switch(charin) //�����ַ���ת
	{
            case '0': //��ͬ���˵��ĺ�һ��
	    {
		menuid[i]++;
		if (menuid[i]>manyou->MenuCount-1) menuid[i]=0;//���Կ���MenuCount���Ǳ�Ҫ��
		    Locate(manyou+menuid[i]);
		break;
	    }
            case '1': //��ͬ���˵���ǰһ��
	    {
		if (menuid[i]==0) menuid[i]=manyou->MenuCount-1;//���Կ���MenuCount���Ǳ�Ҫ��
		else menuid[i]--;
		Locate(manyou+menuid[i]);
		break;
	    }
            case '+': //����һ���˵�,���¼�ʱִ��ĳ����
	    {
		if ((manyou+menuid[i])->Childrenms !=Null)
		{
		    manyou=(manyou+menuid[i])->Childrenms;
		    i++;
		    menuid[i]=0;
		    Locate(manyou+menuid[i]);
		}
		else
		{
		    Run(manyou+menuid[i]);
		}
		break;
	    }
            case '-': //����һ���˵�,���ϼ�ʱ����Щ��ʾ
	    {
		if ((manyou+menuid[i])->Parentms !=Null)
		{
		    manyou=(manyou+menuid[i])->Parentms;
		    i--;
		    Locate(manyou+menuid[i]);
		}
		else
		{
		    printf("You are at the top of menu");
		}
	        break;
	    } 
		default:  break;
	}
    }
    return 0;
}

