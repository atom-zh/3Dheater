#include "stm32f10x.h"
#include "lcd.h"
#include "menu.h"
#include "usart.h"
#include <key.h>
 
#define Null 0
char over=0; //结束标志
 
//结构体类型定义
struct MenuItem
{  
    char MenuCount; //结构体数组的元素个数
    char *DisplayString;  //当前LCD显示的信息
    void (*Subs)();  //执行的函数的指针.
    struct MenuItem *Childrenms;  //指向子节点的指针
    struct MenuItem *Parentms;  //指向父节点的指针
};
 
//函数声明
void Nop(void);
void DoSomething(struct MenuItem *manyou);
void GameOver(void);
void Locate(struct MenuItem *manyou);
void Run(struct MenuItem *manyou);

//====================================
//结构体声明
struct MenuItem m0_main[4];
struct MenuItem m1_file[5];
struct MenuItem m1_edit[4];
struct MenuItem m1_view[2];
struct MenuItem m2_font[3];
struct MenuItem m1_help[3];

//====================================
//结构体实现
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
//函数实现
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

//主函数
int aaaaa(void)
{
    char charin; //输入的字符
    char menuid[3]={0,0,0}; //某一级的菜单的偏移量的记录数组
    char i=0; //上面数组的下标值

    struct MenuItem *manyou; //漫游,在菜单中漫游

    manyou=&m0_main[0]; //开始的位置
    Locate(manyou); //输出

    while(!over) //如果还没完
    {
	charin=KEY_Scan(); //读取键盘字符
	switch(charin) //根据字符跳转
	{
            case '0': //到同级菜单的后一项
	    {
		menuid[i]++;
		if (menuid[i]>manyou->MenuCount-1) menuid[i]=0;//可以看到MenuCount项是必要的
		    Locate(manyou+menuid[i]);
		break;
	    }
            case '1': //到同级菜单的前一项
	    {
		if (menuid[i]==0) menuid[i]=manyou->MenuCount-1;//可以看到MenuCount项是必要的
		else menuid[i]--;
		Locate(manyou+menuid[i]);
		break;
	    }
            case '+': //到下一级菜单,无下级时执行某功能
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
            case '-': //到上一级菜单,无上级时给出些提示
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

