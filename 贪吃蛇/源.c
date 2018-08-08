#include <Windows.h>
#include "resource.h"

HBITMAP hbitmapbackgroup; 
HBITMAP hbitmapApple;
HBITMAP hbitmap_head_0;
HBITMAP hbitmap_head_1;
HBITMAP hbitmap_head_2;
HBITMAP hbitmap_head_3;
HBITMAP hbitmap_body;

typedef struct SNAKE
{
	int x;
	int y;
	struct SNAKE*pnext;
	struct SNAKE*plast;
}Snake,Apple;

Snake* snake_head=NULL;
Snake* snake_tail=NULL;
Apple apple={0,0,NULL,NULL};

/*
0==右
1==上
2==左
3==下
*/
int fx=0;

void showback(HDC hdc);
void AddNode(int x,int y);
void showSnakehead(HDC hdc);
void showApple(HDC hdc);
void Move();
BOOL IseatAppple();
BOOL Iszhuangqiang();
BOOL IseatItsef();
void makeApple();


LRESULT CALLBACK MyWNDPROC(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	//设计窗口
	WNDCLASSEX ex;
	HWND hWnd;
	MSG msg;
	ex.style=CS_HREDRAW|CS_VREDRAW;
	ex.cbSize=sizeof(ex);
	ex.cbClsExtra=0;
	ex.cbWndExtra=0;
	ex.hInstance=hInstance;
	ex.hIcon=NULL;
	ex.hCursor=NULL;
	ex.hbrBackground=CreateSolidBrush(RGB(80,255,80));
	ex.hIconSm=NULL;
	ex.lpfnWndProc=MyWNDPROC;
	ex.lpszMenuName=NULL;
	ex.lpszClassName="123";


	//注册窗口
	RegisterClassEx(&ex);

	//创建窗口
	hWnd=CreateWindow(ex.lpszClassName,"李政宇",WS_OVERLAPPEDWINDOW,100,100,615,638,NULL,NULL,hInstance,NULL);
	hbitmapbackgroup=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP1));
	hbitmapApple=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP2));
	hbitmap_body=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP3));
	hbitmap_head_0=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP4));
	hbitmap_head_1=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP5));
	hbitmap_head_2=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP6));
	hbitmap_head_3=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP7));
	//显示


	AddNode(2,4);
	AddNode(2,3);
	AddNode(2,2);

	SetTimer(hWnd,1,400,NULL);

	ShowWindow(hWnd,SW_SHOW);


	//消息循环
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
int a=0;
int x=0;
LRESULT CALLBACK MyWNDPROC(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	HDC hdc2;
	PAINTSTRUCT ps;
	switch(Msg)
	{
	case WM_KEYDOWN:
		if(a==0)
		{
			switch(wParam)
			{
			case VK_UP:
				if(fx!=3)
				{
					fx=1;
				}

				break;
			case VK_DOWN:
				if(fx!=1)
				{
					fx=3;
				}

				break;
			case VK_LEFT:
				if(fx!=0)
				{
					fx=2;
				}
				break;
			case VK_RIGHT:
				if(fx!=2)
				{
					fx=0;
				}
				break;
			}
		}
		a=1;
		break;
	case WM_LBUTTONDOWN:
		KillTimer(hWnd,1);
		x=x+50;
		SetTimer(hWnd,1,500+x,NULL);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:    //重绘消息
		hdc=BeginPaint(hWnd,&ps);
		showback(hdc);
		showApple(hdc);
		showSnakehead(hdc);

		EndPaint(hWnd,&ps);
		break;
	case WM_RBUTTONDOWN:
		KillTimer(hWnd,1);
		x=x+50;
		SetTimer(hWnd,1,500-x,NULL);
		break;
	case WM_TIMER:
		a=1;
		hdc=GetDC(hWnd);
		//1蛇移动
		Move();
		if(IseatAppple()==TRUE)
		{
			//蛇身增加
			AddNode(snake_tail->x,snake_tail->y);
			//产生新的苹果
			/*apple.x=rand()%18;
			apple.y=rand()%18;*/
			makeApple();
		}
		if(Iszhuangqiang()==TRUE||IseatItsef()==TRUE)
		{

			KillTimer(hWnd,1);
			PostQuitMessage(0);
		}

		//显示背景
		showback(hdc);
		//显示蛇
		showSnakehead(hdc);
		showApple(hdc);
		ReleaseDC (hWnd, hdc);
		a=0;
		break;

	}


	return DefWindowProc(hWnd,Msg,wParam,lParam);


}

void showback(HDC hdc)
{
	HDC hdc2=CreateCompatibleDC(hdc);
	SelectObject(hdc2,hbitmapbackgroup);
	BitBlt(hdc,0,0,618,638,hdc2,0,0,SRCCOPY);
	DeleteDC(hdc2);
}

void AddNode(int x,int y)
{
	Snake*snake=(Snake*)malloc(sizeof(Snake));
	snake->x=x;
	snake->y=y;
	snake->plast=NULL;
	snake->pnext=NULL;
	if(NULL==snake_head)
	{
		snake_head=snake;
	}
	else
	{
		snake_tail->pnext=snake;
		snake->plast=snake_tail;
	}
	snake_tail=snake;
}

void showSnakehead(HDC hdc)
{
	Snake*bj=snake_head->pnext;
	HDC hdc2 =CreateCompatibleDC(hdc);
	switch(fx)
	{
	case 0:
		SelectObject(hdc2,hbitmap_head_0);
		break;
	case 1:
		SelectObject(hdc2,hbitmap_head_1);
		break;
	case 2:
		SelectObject(hdc2,hbitmap_head_2);
		break;
	case 3:
		SelectObject(hdc2,hbitmap_head_3);
		break;
	}
	BitBlt(hdc,snake_head->y*30+30,snake_head->x*30+30,30,30,hdc2,0,0,SRCCOPY);
	DeleteDC(hdc2);

	//显示蛇身
	hdc2=CreateCompatibleDC(hdc);
	SelectObject(hdc2,hbitmap_body);
	while(bj)
	{
		BitBlt(hdc,bj->y*30+30,bj->x*30+30,30,30,hdc2,0,0,SRCCOPY);
		bj=bj->pnext;
	}
	DeleteDC(hdc2);
}

void showApple(HDC hdc)
{
	HDC hdc2=CreateCompatibleDC(hdc);
	SelectObject(hdc2,hbitmapApple);
	BitBlt(hdc,apple.y*30+30,apple.x*30+30,30,30,hdc2,0,0,SRCCOPY);
	DeleteDC(hdc2);
}
void Move()
{
	Snake*bj=snake_tail;
	while(bj!=snake_head)
	{
		bj->x=bj->plast->x;
		bj->y=bj->plast->y;
		bj=bj->plast;
	}
	switch(fx)
	{
	case 0:
		snake_head->y++;
		break;
	case 1:
		snake_head->x--;
		break;
	case 2:
		snake_head->y--;
		break;
	case 3:
		snake_head->x++;
		break;
	}
}
BOOL eatItsef();

BOOL IseatAppple()
{
	if(apple.x==snake_head->x&&apple.y==snake_head->y)
	{
		return TRUE;

	}
	else
	{
		return FALSE;
	}
}

void makeApple()
{
	int x;
	int y;
	Snake*bj;
	while(1)
	{
		x=rand()%18;
		y=rand()%18;
		bj=snake_head;
		while(bj->x!=x&&bj->y!=y)
		{
			if(bj==snake_tail)
			{
				apple.x=x;
				apple.y=y;
				return;
			}
			bj=bj->pnext;
		}
	}
}

BOOL IseatItsef()
{

	Snake*bj=snake_head->pnext;
	while(bj)
	{
		if(bj->x==snake_head->x&&bj->y==snake_head->y)
		{
			return TRUE;
		}
		bj=bj->pnext;
	}
	return FALSE;
}

BOOL Iszhuangqiang()
{
	if(snake_head->x<0|snake_head->x>17|snake_head->y<0|snake_head->y>17)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}