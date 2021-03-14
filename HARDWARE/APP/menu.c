#include "stm32f10x.h"
#include "lcd.h"
#include "menu.h"
#include "usart.h"
#include "key.h"
#include "delay.h"

#define Null 0
struct Dev_Info *mdev;

//结构体类型定义
struct MenuItem
{  
    char MenuCount;               //结构体数组的元素个数
    char *DisplayString;          //当前LCD显示的信息
    void (*Subs)();               //执行的函数的指针.
    struct MenuItem *Childrenms;  //指向子节点的指针
    struct MenuItem *Parentms;    //指向父节点的指针
};

//函数声明
void Nop(void);
void DoSomething(void);
void TurnBack(void);
void Locate(void);
void Run(void);
void Display_title(void);
void MaxTempSet(void);
void TempModCel(void);
void TempModFah(void);
void MaxHumSet(void);

void DoShowTempVal(void);

//====================================
//结构体声明
struct MenuItem m0_main[5];
struct MenuItem m1_dis[2];
struct MenuItem m1_mod[4];
struct MenuItem m1_sys[4];
struct MenuItem m1_rev[2];
struct MenuItem m1_help[2];

struct MenuItem m2_temp[3];
struct MenuItem m2_hum[2];
struct MenuItem m2_weight[4];

struct MenuItem m3_temp_mod[3];
struct MenuItem m3_temp_th[2];

void ShowRevInfo(void);
void ShowHelp(void);

char menuid[5]={0,0,0,0,0}; //某一级的菜单的偏移量的记录数组
char i=0; //上面数组的下标值
struct MenuItem *manyou=&m0_main[0]; //漫游,在菜单中漫游

//====================================
//结构体实现
/*    menu    layout                    
 *    m0          m1        m2        m3        m4
 *   main       Mod_Set  Sys_Set             
 *  Dis_Info    MOD1     Temp                
 *  Mod_Set     MOD2     Humtity             
 *  Sys_set     MOD3     Weight              
 *  Dev_Info    Mod4                         
 *  Help                                                                    
 */

struct MenuItem m0_main[5]=
{
    {5, "Dis Info",  DoSomething,    m1_dis,     Null},
    {5, "Mod Set",   DoSomething,    m1_mod,     Null},
    {5, "Sys Set",   DoSomething,    m1_sys,     Null},
    {5, "Rev Info",  ShowRevInfo,    Null,     Null},
    {5, "Help",      ShowHelp,       Null,    Null},
};

struct MenuItem m1_dis[2]=
{
    {2, "Dis_Info", DoSomething,    Null,    m0_main},
    {2, "Exit",     TurnBack,       Null,    m0_main},
};

struct MenuItem m1_sys[4]=
{
    {4, "Temp",     DoSomething,    m2_temp,     m0_main},
    {4, "Humitity", DoSomething,    m2_hum,      m0_main},
    {4, "Weight",   DoSomething,    m2_weight,   m0_main},
    {4, "Exit",     TurnBack,       Null,        m0_main},
};

struct MenuItem m1_mod[4]=
{
    {4, "MOD1",     DoSomething,    Null,   m0_main},
    {4, "MOD2",     DoSomething,    Null,   m0_main},
    {4, "MOD3",     DoSomething,    Null,   m0_main},
    {4, "Exit",     TurnBack,       Null,   m0_main},
};

struct MenuItem m1_help[2]=
{
    {2, "Help Info",    DoSomething,    Null,   m0_main},
    {2, "Exit",         TurnBack,       Null,   m0_main},
};

struct MenuItem m2_temp[3]=
{
    {3, "Temp Mod",         DoSomething,      m3_temp_mod,  m1_sys},
    {3, "Threshold Set",    DoSomething,      m3_temp_th,   m1_sys},
    {3, "Exit",             TurnBack,         Null,         m1_sys},
};

struct MenuItem m2_hum[2]=
{
    {2, "Hum Max",          MaxHumSet,      Null,   m1_sys},
    {2, "Exit",             TurnBack,       Null,   m1_sys},
};

struct MenuItem m2_weight[4]=
{
    {4,"Weight Mod",    DoSomething,    Null,   m1_sys},
    {4,"W1 Set",        DoSomething,    Null,   m1_sys},
    {4,"W2 Set",        DoSomething,    Null,   m1_sys},
    {4,"Exit",          TurnBack,       Null,   m1_sys},
};

struct MenuItem m3_temp_mod[3]=
{
    {3, "Celsius:",      TempModCel,    Null,   m2_temp},
    {3, "Fahrenheit:",   TempModFah,    Null,   m2_temp},
    {3, "Exit",          TurnBack,      Null,   m2_temp},
};

struct MenuItem m3_temp_th[2]=
{
    {2, "Max Temp",          MaxTempSet,     Null,   m2_temp},
    {2, "Exit",              TurnBack,       Null,   m2_temp},
};


void TempModCel(void)
{
    enum KEY key_val;
    delay_ms(100);
    key_val = KEY_Scan();
    KEY_update(KEY_RELEASE);
    while(key_val != KEY_ENTER)
    {
        key_val = KEY_Scan();
        KEY_update(KEY_RELEASE);
        delay_ms(100);

        if(key_val == KEY_RIGHT)
            mdev->hum_mod = 0;
        else if(key_val == KEY_LEFT)
            mdev->hum_mod = 1;
            
        LCD_ShowString(100,26, (const unsigned char *)(mdev->hum_mod?"Y":"N"));
    }
}

void TempModFah(void)
{
    enum KEY key_val;
    delay_ms(100);
    key_val = KEY_Scan();
    KEY_update(KEY_RELEASE);
    while(key_val != KEY_ENTER)
    {
        key_val = KEY_Scan();
        KEY_update(KEY_RELEASE);
        delay_ms(100);

        if(key_val == KEY_RIGHT)
            mdev->temp_mod = 0;
        else if(key_val == KEY_LEFT)
            mdev->temp_mod = 1;
        LCD_ShowString(100,26, (const unsigned char *)(mdev->hum_mod?"Y":"N"));
    }
}

void MaxTempSet(void)
{
    enum KEY key_val;
    delay_ms(100);
    key_val = KEY_Scan();
    KEY_update(KEY_RELEASE);
    LCD_ShowString(2, 14, "Set Max Temp:");
    while(key_val != KEY_ENTER)
    {
        key_val = KEY_Scan();
        KEY_update(KEY_RELEASE);
        delay_ms(100);
        if(mdev->set.temp_th >= TEMP_MAX)
            mdev->set.temp_th = 100;
        else if(mdev->set.temp_th < 1)
            mdev->set.temp_th = 0;

        if(key_val == KEY_RIGHT)
            mdev->set.temp_th++;
        else if(key_val == KEY_LEFT)
            mdev->set.temp_th--;

        LCD_ShowNum(110,14, mdev->set.temp_th, 3, 12);
        printf("Set temp %d\n", mdev->set.temp_th);
    }
    FLASH_SaveData(&mdev->set);
}

void MaxHumSet(void)
{
    enum KEY key_val;
    delay_ms(100);
    key_val = KEY_Scan();
    KEY_update(KEY_RELEASE);
    LCD_ShowString(2, 14, "Set Hum Temp:");
    while(key_val != KEY_ENTER)
    {
        key_val = KEY_Scan();
        KEY_update(KEY_RELEASE);
        delay_ms(100);
        if(mdev->set.hum_th >= 100)
            mdev->set.hum_th = 100;
        else if(mdev->set.hum_th < HUM_MIN)
            mdev->set.hum_th = HUM_MIN;

        if(key_val == KEY_RIGHT)
            mdev->set.hum_th++;
        else if(key_val == KEY_LEFT)
            mdev->set.hum_th--;

        LCD_ShowNum(110, 14, mdev->set.hum_th, 2, 12);
        printf("Set temp %d\n", mdev->set.hum_th);
    }
    FLASH_SaveData(&mdev->set);
}


//====================================
//函数实现
void MenuFresh(void)
{
    char a = 0;
    printf("Now we at %s\n" ,(manyou+menuid[i])->DisplayString);

    LCD_Clear();
    //Display title
    if((manyou+menuid[i])->Parentms == 0)
        LCD_ShowString(28,0, (const unsigned char *)"Main");
    else
        LCD_ShowString(28,0, (const unsigned char *)(manyou->Parentms+menuid[i-1])->DisplayString);

    LCD_DrawLine(0,13,1,128);
    //display body context
    for(a=0; a<(manyou+menuid[i])->MenuCount; a++)
    {
        LCD_ShowString(10,12*(a+1)+2, (const unsigned char *)((manyou+((menuid[i]>3)?(menuid[i]-3+a):a))->DisplayString));
    }

    //display select
    LCD_ShowString(1,12*(((menuid[i]>3)?3:menuid[i])+1)+2, (const unsigned char *)">");
}

void Run(void)
{
    (*((manyou+menuid[i])->Subs))(manyou+menuid[i]);
}

void Nop(void)
{}

void DoSomething(void)
{
    printf("we have done %s\n" , (manyou+menuid[i])->DisplayString);
}

void DoShowTempVal(void)
{
    LCD_ShowNum(90,14,mdev->set.temp_th,2,12);
}

void ShowRevInfo(void)
{
    LCD_Clear();
    LCD_ShowString(10,14, (const unsigned char *)"SW: V1.0");
    delay_ms(1000);
}

void ShowHelp(void)
{
    LCD_Clear();
    LCD_ShowString(10,14, (const unsigned char *)"3D Printers");
    LCD_ShowString(10,26, (const unsigned char *)"PH:133xxxxxx");
    delay_ms(1000);
}

void TurnBack(void)
{
    printf("Now Turn Back \r\n");
    
    if((manyou+menuid[i])->Parentms != Null)
    {
		manyou=(manyou+menuid[i])->Parentms;
		i--;
    }
    else
	{
		printf("You are at the top of menu");
    }

    MenuFresh();
}

void dump_array(void)
{
    int a;
    printf("===========");
    for(a=0;a<5;a++)
    {
        printf("%d \t", menuid[a]);
    }
    printf("=============\n");
}

int menu_update(struct Dev_Info *dev)
{
    mdev = dev;

    if(dev->status == MENU_ENTRY)
    {
        MenuFresh();
        return 0;
    }

	switch(dev->key_val) //根据字符跳转
	{
        case KEY_LEFT: //到同级菜单的后一项
	    {
	         menuid[i]++;
	         if(menuid[i] > manyou->MenuCount-1)
                 menuid[i]=0;//可以看到MenuCount项是必要的
		     break;
	    }
        case KEY_RIGHT: //到同级菜单的前一项
	    {
		    if(menuid[i]==0)
                menuid[i]=manyou->MenuCount-1;//可以看到MenuCount项是必要的
		    else
                menuid[i]--;
		    break;
	    }
        case KEY_ENTER: //到下一级菜单,无下级时执行某功能
	    {
            if(!(strncmp(((manyou+menuid[i])->DisplayString), "Dis", 3)))
            {
                menuid[0]=0;
                i=0;
                manyou=&m0_main[0];
                return 1;
            }

		    if((manyou+menuid[i])->Childrenms != Null)
		    {
		        manyou=(manyou+menuid[i])->Childrenms;
		        i++;
		        menuid[i]=0;
                
		    }
		    else
		    {
		        Run();
		    }
		    break;
	    }
        case KEY_RELEASE:
            return 0;
		default:  break;
	}

    MenuFresh();
    //dump_array();
    return 0;
}
