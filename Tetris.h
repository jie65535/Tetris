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
		dir = 1;                   // 起始方向
		x = startX;                // 起始x坐标
		y = startY;                // 起始y坐标
	}
public:
	char index;
	char dir;
	char x;
	char y;
};

// 检查该方块在地图上是否可行
bool isFeasible(const Box &target);
// 将方块固定在地图上
void fixedPos(const Box &target);
// 移动方块到指定偏移量上
bool move(Box &A, char XOffset, char YOffset);
// 旋转方块
bool rotate(Box &A);
// 清除达成目标的方块
void clearCompleted();
// 在指定坐标上输出字符串
void printXY(COORD coord, std::string str);
// 在指定坐标上输出指定字符的方块
void printBox(const Box &target, std::string printChar, short x = 0, short y = 0);
// 展示地图
void showMap(COORD coord);
// 获取用户输入并判断
void input();
// 游戏循环
void GameLoop();

#endif