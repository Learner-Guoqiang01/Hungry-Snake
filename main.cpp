#include <iostream>
#include <string>
#include "main.h"
#include <easyx.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib") 
#include <conio.h>
#include <stdlib.h>

unsigned int width = 500;
unsigned int high = 500;

char cursor_status = 0;  //欢迎界面光标位置
int task_index = 0;  //任务，0--开始，1--帮助，2--退出

struct Snake
{
	int x;
	int y;
	char status;  //0--未使用，1--使用
};

//初始化游戏界面背景
void GraphInit(void)
{
	//欢迎界面
	settextcolor(GREEN);  //颜色
	settextstyle(72, 0, 0);  //高，宽，字体
	outtextxy(width / 2 - 100, 20, L"贪吃蛇");

	settextcolor(GREEN);  //颜色
	settextstyle(50, 0, 0);  //高，宽，字体
	outtextxy(width / 2 - 70, 150, L"开始");
	outtextxy(width / 2 - 70, 250, L"帮助");
	outtextxy(width / 2 - 70, 350, L"退出");
}

//读取键盘
char GetKeyboard(void)
{
	if (_kbhit())
		return _getch();
	else
		return -1;
}

//欢迎界面控制
void WelcomePageControl(char key)
{
	int temp_y = 0;
	int enter_key = 0;

	//读取键盘
	switch (key)
	{
	case 'w':
		cursor_status--;
		break;
	case 'a':
		break;
	case 's':
		cursor_status++;
		break;
	case 'd':
		break;
	case 13:
		enter_key = 1;
		break;
	default:
		break;
	}

	//光标限幅
	if (cursor_status > 3)
		cursor_status = 3;
	if (cursor_status < 1)
		cursor_status = 1;

	//绘制光标
	switch (cursor_status)
	{
	case 1:
		temp_y = 175;
		break;
	case 2:
		temp_y = 275;
		break;
	case 3:
		temp_y = 375;
		break;
	default:
		break;
	}
	setlinecolor(GREEN);
	circle(160, temp_y, 10);

	//回车进入游戏界面
	if (enter_key)
	{
		enter_key = 0;

		switch (cursor_status)
		{
		case 1:
			task_index = 1;
			break;
		case 2:
			task_index = 2;
			break;	
		case 3:
			task_index = 3;
			break;
		default:
			task_index = 0;
			break;
		}
	}
}

//游戏结束，length：蛇长度
void GameOver(char length)
{
	wchar_t  str_2[30] = {0};
	swprintf(str_2,30,L"Game Over!  得分：%d", length);
	outtextxy(50, 200, str_2);

	while (1);
}


/*
 * 函数名：贪吃蛇
 * 输  入：键盘WASD控制方向，Enter确认，ESC退出 
 * 输  出：
 * 说  明：已添加撞墙、撞身体
 */
int main()
{
	int count = 0;
	char length = 3;
	char current_key = 0;
	Snake snake[60] = { 0 };  //注意数组大小

	//1.窗口
	initgraph(width, high);
	GraphInit();

	while (1)
	{
		current_key = GetKeyboard();

		//欢迎界面
		if (current_key > 0)
		{
			printf("%X\n", current_key);
			system("cls");  //刷新控制台
	        cleardevice();  //刷新界面
			GraphInit();
		}
		WelcomePageControl(current_key);

		//游戏界面
		if (task_index == 1)
		{
			//画桌布
			setfillcolor(WHITE);
			fillrectangle(0, 0, 500, 500);  

			//蛇头位置初始化
			unsigned int x = 50;
			unsigned int y = 50;

			int dir = 4;  //前进方向
			int step = 10;  //步进大小

			//果实变量
			int apple_x = 0;
			int apple_y = 0;
			char apple_eated = 1;

			srand(time(0));

			while (1)
			{
				char curr_key = GetKeyboard();

				if (curr_key == 0x1b)  //ESC
				{
					task_index = 2;  //退出游戏界面
					break;
				}
				
				switch (curr_key)
				{
				case 'w':
					dir = 1;
					break;
				case 'a':
					dir = 2;
					break;
				case 's':
					dir = 3;
					break;
				case 'd':
					dir = 4;
					break;
				default:
					break;
				}

				switch (dir)
				{
				case 1:
					y -= step;
					break;
				case 2:
					x -= step;
					break;
				case 3:
					y += step;
					break;
				case 4:
					x += step;
					break;
				default:
					break;
				}
				//位置限幅
				if (x < 0)
				{
					x = 0;
					GameOver(length);  //撞墙
				}	
				if (x > width)
				{
					x = width - 10;
					GameOver(length);
				}
				if (y < 0)
				{
					y = 0;
					GameOver(length);
				}
				if (y > high)
				{
					y = high - 10;
					GameOver(length);
				}
				//蛇节位置处理
				for (char k = length - 1; k > 0; k--)
				{
					snake[k].x = snake[k - 1].x;
					snake[k].y = snake[k - 1].y;

					//撞身体
					if (snake[k].x == x && snake[k].y == y && length > 4)
					{
						GameOver(length);
					}
				}
				snake[0].x = x;
				snake[0].y = y;

				//吃到果实
				if (snake[0].x == apple_x && snake[0].y == apple_y)
				{
					apple_eated = 1;
					length++;
				}

				//画蛇
				setfillcolor(BLUE);
				for (int i = 0; i < length; i++)
				{
					x = snake[i].x;
					y = snake[i].y;
					//printf("%d:%d,%d\n",i,x,y);
					fillrectangle(x, y, x + 10, y + 10);
				}

				//恢复蛇头
				x = snake[0].x;
				y = snake[0].y;

				//果实  问题：果实可能会出现在蛇身上
				if (apple_eated)
				{
					apple_eated = 0;

					apple_x = (rand()%50) * 10;
					apple_y = (rand()%50) * 10;
					if (apple_x < 0)
						apple_x = -apple_x;
					if (apple_y < 0)
						apple_y = -apple_y;

					printf("apple:%d,%d\n", apple_x,apple_y);
					setfillcolor(RED);
					fillrectangle(apple_x, apple_y, apple_x + 10, apple_y + 10);
				}
				else
				{
					setfillcolor(RED);
					fillrectangle(apple_x, apple_y, apple_x + 10, apple_y + 10);
				}
				
				//时基控制
				Sleep(90); 

				//刷新
				setfillcolor(WHITE);
				fillrectangle(0, 0, 500, 500);  
			}
		}
		else if(task_index == 2)  //退出游戏
			break;
	}

	// 关闭图形模式
	closegraph();
	while(1);

	return 0;
}


