/* Tetris.cpp -- file */
#include "Tetris.h"

bool printNextBoxVal = true;                // �Ƿ������һ������������
bool printMapVal     = true;                // �Ƿ���µ�ͼ�������
bool isGameOver      = false;               // �Ƿ���Ϸʧ��
char Map[mapWidth][mapHeight] = {0};
int  speed           = 400;

HANDLE hStdOut;
Box thisBox, nextBox;

// ���÷����ڵ�ͼ���Ƿ����
bool isFeasible(const Box &target)
{
	for(int YOffset = 0; YOffset < 4; ++YOffset)
	{
		for(int XOffset = 0; XOffset < 4; ++XOffset)
		{
			if(target.getShape() & (0x8000 >> (YOffset*4 + XOffset)))
			{
				// �����λ��Խ�� ����false
				if(target.x + XOffset < 0 || target.x + XOffset >= mapWidth
					|| target.y + YOffset < 0 || target.y + YOffset >= mapHeight)
					return false;
				
				// �����λ�����Ѿ��з���
				if(Map[target.x + XOffset][target.y + YOffset])
					return false;
			}
		}
	}
	return true;
}

// ������̶��ڵ�ͼ��
void fixedPos(const Box &target)
{
	for(int YOffset = 0; YOffset < 4; ++YOffset)
	{
		for(int XOffset = 0; XOffset < 4; ++XOffset)
		{
			// ����жϵ�λ��Խ��
			if(target.x + XOffset < 0 || target.x + XOffset >= mapWidth
				|| target.y + YOffset < 0 || target.y + YOffset >= mapHeight)
				continue;
				
			if(target.getShape() & (0x8000 >> (YOffset*4 + XOffset)))
			{
				Map[target.x + XOffset][target.y + YOffset] = 1;
				clearCompleted();
			}
		}
	}
}

bool move(Box &A, char XOffset, char YOffset)
{
	char oldX = A.x, oldY = A.y;
	printBox(A, "  ");
	A.x += XOffset, A.y += YOffset;
	if(!isFeasible(A))
	{
		A.x = oldX, A.y = oldY;
		printBox(A, "��");
		if(YOffset > 0){                 // ��������
			fixedPos(A);
			A = nextBox;
			printNextBoxVal = true;      // ֪ͨ ��һ��������ʾ�������
			if(!isFeasible(A))           // ������ɵķ����λ�����Ѿ��з��飬�ж���Ϸʧ��
			{
				isGameOver = true; 
			}
		}
		return false;
	}
	printBox(A, "��");
	return true;
}

bool rotate(Box &A)
{
	char oldDir = A.dir;
	A.dir = (A.dir + 1) % 4;
	if(!isFeasible(A))
	{
		A.dir = oldDir;
		return false;
	}
	A.dir = oldDir;
	printBox(A, "  ");
	A.dir = (A.dir + 1) % 4;
	printBox(A, "��");
	return true;
}

void clearCompleted()
{
	for(int y = mapHeight-1, x = 0; y >= 0; )
	{
		for(x = 0; x < mapWidth; ++x)
		{
			if(!Map[x][y])
				break;
		}
		// �������
		if(x == mapWidth)
		{
			// �Ѹò����ϵ����з��������ƶ�һ��
			for(int j = y; j > 0; --j)
			{
				for(int i = 0; i < mapWidth; ++i)
				{
					Map[i][j] = Map[i][j-1];
				}
			}
			// ��ն���
			for(int i = 0; i < mapWidth; ++i)
				Map[i][0] = 0;
			
			if(speed > 200)
				speed -= 10;
			printMapVal = true;   // ֪ͨ���µ�ͼ
		}else{ //δ��������ͽ������ϼ��
			--y;
		}
	}
}

void restart()
{
	for(int i = 0; i < mapWidth; ++i)
		for(int j = 0; j < mapHeight; ++j)
			Map[i][j] = 0;
	printMapVal     = true;
	thisBox.randGenerateNewBox();
	printNextBoxVal = true;
	speed = 400;
	isGameOver = false;
}

void printXY(COORD coord, std::string str)
{
	SetConsoleCursorPosition(hStdOut, coord);
	WriteConsole(hStdOut, str.c_str(), str.length(), NULL, NULL);
}

// ��ָ��λ��������飬x,yȱʡΪ0�����Ϊ0����Box�������
void printBox(const Box &target, std::string printChar, short x, short y)
{
	short posX = x?x:target.x * 2 + 2; // +2����Ϊ�߿�ռ��2���
	short posY = y?y:target.y;
	for(int YOffset = 0; YOffset < 4; ++YOffset)
	{
		for(int XOffset = 0; XOffset < 4; ++XOffset)
		{
			// ��������λ�ò��Ϸ�
			if(posX + XOffset < 0 || posY + YOffset < 0)
				continue;
				
			if(target.getShape() & (0x8000 >> (YOffset*4 + XOffset)))
				printXY({posX + XOffset*2, posY + YOffset}, printChar);
		}
	}
}

void showMap(short _x, short _y)
{
	for(short x = 0; x <= mapWidth+1; ++x)
	{
		printXY({_x + x*2, _y + mapHeight}, "��");
	}
	for(short y = 0; y < mapHeight; ++y)
	{
		printXY({_x, _y + y}, "��");
		printXY({_x + mapWidth * 2 + 2, _y + y}, "��");
		for(short x = 0; x < mapWidth; ++x)
		{
			printXY({_x + 2 + x*2, _y + y}, (Map[x][y] ? "��" : "  "));
		}
	}
}

void input()
{
	char tempPos = 0;
	switch(getch())
	{
	case 'w':
	case 'W':
		rotate(thisBox);
		break;
	case 'a':
	case 'A':
		move(thisBox, -1, 0);
		break;
	case 'd':
	case 'D':
		move(thisBox, 1, 0);
		break;
	case 's':
	case 'S':
		move(thisBox, 0, 1);
		break;
	case ' ':
		while(move(thisBox, 0, 1));
		break;
	case 'z':
	case 'Z':
		for(int i = 0; i < mapWidth; ++i)
			Map[i][mapHeight-1] = 1;
		clearCompleted();
		break;
	case 'r':
	case 'R':
		restart();
		break;
	case '\n':
	case '\r':
		printXY({30, 14}, "��ͣ��...");
		printXY({30, 15}, "�����������");
		getch();
		printXY({30, 14}, "            ");
		printXY({30, 15}, "            ");
		break;
	case 'q':
	case 'Q':
		exit(1);
		break;
	}
}

void GameLoop()
{
	thisBox.randGenerateNewBox();
	nextBox.randGenerateNewBox();
	system("cls");
	printXY({30, 5}, "w ��ת");
	printXY({30, 6}, "s ����һ��");
	printXY({30, 7}, "a ���� d ����");
	printXY({30, 8}, "�ո� ֱ������");
	printXY({30, 9}, "r ���¿�ʼ");
	printXY({30, 10}, "�س�����ͣ");
	printXY({30, 11}, "q �˳�");
	while(!isGameOver)
	{
		if(printMapVal)
		{
			showMap(0, 0);
			printMapVal = false;
		}
		if(printNextBoxVal)
		{
			printXY({30, 0}, "next:");
			printBox(nextBox, "  ", 30, 1);
			nextBox.randGenerateNewBox();
			printBox(nextBox, "��", 30, 1);
			printNextBoxVal = false;
		}
		printBox(thisBox, "��");
		
		move(thisBox, 0, 1);
		
		while(kbhit())
			input();
		
		if(isGameOver)
		{
			printXY({6, 10}, "��Ϸ������");
			printXY({4, 11}, "���¿�ʼ�밴R");
			char ch = getch();
			if(ch == 'r' || ch == 'R')
				restart();
		}
	
		Sleep(speed);
	}
}

int main()
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(std::time(NULL));
	GameLoop();
	
	return 0;
}