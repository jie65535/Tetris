/* Tetris.h -- file */
#ifndef _TETRIS_H_
#define _TETRIS_H_
#include <string>
#include <ctime>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>


const char mapWidth = 10;
const char mapHeight = 20;
const char startX    = mapWidth / 2 - 2;
const char startY    = 0;

const char shapeListMax = 7;
const uint16_t shapeList[shapeListMax][4] = {
	{0xcc00,	0xcc00,	0xcc00,	0xcc00}, // O
	{0x4444,	0xf00,	0x4444,	0xf00 }, // I
	{0x88c0,	0xe800,	0xc440,	0x2e00}, // L
	{0x44c0,	0x8e00,	0xc880,	0xe200}, // J
	{0x4c80,	0xc600,	0x4c80,	0xc600}, // Z
	{0x8c40,	0x6c00,	0x8c40,	0x6c00}, // S
	{0x4c40,	0x4e00,	0x8c80,	0xe400}  // T
};
class Box {
public:
	Box(char _index = 0, char _dir = 0, char _x = 0, char _y = 0)
	:index(index), dir(_dir), x(_x), y(_y){};
	uint16_t getShape() const {return shapeList[index][dir];};
	void randGenerateNewBox()
	{
		index = rand() % shapeListMax;
		dir = 1;                   // ��ʼ����
		x = startX;                // ��ʼx����
		y = startY;                // ��ʼy����
	}
public:
	char index;
	char dir;
	char x;
	char y;
};

// ���÷����ڵ�ͼ���Ƿ����
bool isFeasible(const Box &target);
// ������̶��ڵ�ͼ��
void fixedPos(const Box &target);
// �ƶ����鵽ָ��ƫ������
bool move(Box &A, char XOffset, char YOffset);
// ��ת����
bool rotate(Box &A);
// ������Ŀ��ķ���
void clearCompleted();
// ��ָ������������ַ���
void printXY(COORD coord, std::string str);
// ��ָ�����������ָ���ַ��ķ���
void printBox(const Box &target, std::string printChar, short x = 0, short y = 0);
// չʾ��ͼ
void showMap(COORD coord);
// ��ȡ�û����벢�ж�
void input();
// ��Ϸѭ��
void GameLoop();

#endif