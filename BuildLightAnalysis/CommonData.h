/**********************************************************************************************//**
 * \file	CommonData.h
 *
 * \brief	Declares the common data class.
 **************************************************************************************************/

#pragma once

#include <vector>
#include <string>
#include "vec2.h"
#include "vec3.h"

using namespace std;

/**********************************************************************************************//**
 * \def	GAME_VERSION
 *
 * \brief	A macro that defines game version.
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

#define GAME_VERSION 113 

/**********************************************************************************************//**
 * \struct	sLine
 *
 * \brief	表示内外墙的几何信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct sLine
{
	enum W_TYPE
	{
		OUT_WALL = 1,
		IN_WALL = 2
		
	};
	Vec2d s;
	Vec2d e;
	W_TYPE type;
	sLine(Vec2d s, Vec2d e, W_TYPE itype = IN_WALL)
	{
		this->s = s;
		this->e = e;
		type = itype;
	}
	sLine()
	{
		this->s = Vec2d::ZERO;
		this->e = Vec2d::ZERO;
		type = IN_WALL;
	}
};


#pragma pack(1)

/**********************************************************************************************//**
 * \struct	sOpWall
 *
 * \brief	内外墙的信息（附带材质）
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct sOpWall
{
	sLine line;
	WCHAR mat[80];
};
#pragma pack()

/**********************************************************************************************//**
 * \struct	WallIndex
 *
 * \brief	内外墙和窗户的索引信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct WallIndex
{
	WallIndex(int t, int i)
	{
		this->type = t;
		this->index = i;
	}
	WallIndex()
	{
		this->type = 1;
		this->index = -1;
	}
	int type;// 1 2 3 : 外墙， 内墙， 窗户
	int index;
};

/**********************************************************************************************//**
 * \struct	Wall
 *
 * \brief	内外墙的全部信息，包含几何信息，索引信息，以及顺序信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct Wall
{
	sLine line;
	WallIndex wallInfo;
	bool isOrder;
};

#pragma pack(1)

/**********************************************************************************************//**
 * \struct	stWindow
 *
 * \brief	窗户信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct stWindow
{
	WCHAR wallType[80];
	int wallIndex;
	double pos;
	double WinUpHeight;
	double WinDownHeight;
	double WinWidth;
	WCHAR WinMaterial[80];

	bool operator < (stWindow& b)
	{
		return pos < b.pos;
	}
};
#pragma pack()

/**********************************************************************************************//**
 * \struct	GridPoint
 *
 * \brief	计算网格点信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct GridPoint
{
	Vec3d p;
	bool isKey;
};

/**********************************************************************************************//**
 * \struct	Grid
 *
 * \brief	计算网格信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct Grid
{
	double offset;
	double meshLen;
	double height;
	vector<GridPoint> points;
};

/**********************************************************************************************//**
 * \struct	RoomType
 *
 * \brief	房间类型信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct RoomType
{
	WCHAR name[80];
	double e;
};

/**********************************************************************************************//**
 * \struct	LuminaireTem
 *
 * \brief	灯具信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct LuminaireTem
{
	WCHAR type[80];
	double lm;
	double w;
};

/**********************************************************************************************//**
 * \struct	ControlSetTem
 *
 * \brief	控制分组信息，类型，参数
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct ControlSetTem
{
	enum
	{
		manual_on_off,
		mannual_on_auto_off,
		light_sensor_on_off,
		auto_on_off,
		mannual_on_off_dimming,
		mannual_on_auto_off_dimming,
		time_table,
	};
	WCHAR type[80];
	vector<double> args;
};

/**********************************************************************************************//**
 * \struct	Room
 *
 * \brief	房间信息，类型，高度，内外墙窗户索引，计算网格
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct Room
{
	RoomType type;
	double height;
	vector<long> outWalls;
	vector<long> inWalls;
	vector<long> windows;
	//计算网格
	Grid grid;
};

//以下结构体用于导出-------------------------------------------------------------------------------------------------------------

/**********************************************************************************************//**
 * \struct	OutSurface
 *
 * \brief	导出文件中的面信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct OutSurface
{
	string mat;
	string type;
	string name;
	vector<Vec3d> points;
	int args[3];
};

/**********************************************************************************************//**
 * \struct	OutRoom
 *
 * \brief	导出文件中的房间几何和基本信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct OutRoom
{
	vector<OutSurface> surfaces;
	double girth;
	double area;
	double height;
};

/**********************************************************************************************//**
 * \struct	OutLumSingle
 *
 * \brief	导出文件中的单个灯具信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct OutLumSingle
{
	WCHAR type[80];
	double lm;
	double w;
	Vec3d p;
	Vec3d np;
};

/**********************************************************************************************//**
 * \struct	OutLumSet
 *
 * \brief	导出文件中的组灯具信息，并实现了生成灯具的逻辑
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct OutLumSet
{
	WCHAR type[80];
	double lm;
	double w;
	double z;
	Vec3d np;
	Vec2d originP;
	int rowN;
	int colN;
	double rowL;
	double colL;

	void outputLums(vector<Vec2d>& outLums)
	{
		for (int i = 0; i < rowN; i++)
		{
			for (int j = 0; j < colN; j++)
			{
				outLums.push_back(Vec2d(i*colL + originP.x, j*rowL + originP.y));				
			}
		}
	}
};

/**********************************************************************************************//**
 * \struct	OutControlSet
 *
 * \brief	导出文件中控制分组信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct OutControlSet
{
	vector<long> lumSingles;
	vector<long> lumSets;
	WCHAR type[80];
	int keyGrid;
};

/**********************************************************************************************//**
 * \struct	OutPerson
 *
 * \brief	导出文件中的人员信息
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct OutPerson
{
	WCHAR schedule_type[80];
	WCHAR behavior_type[80];
	vector<long> controlIds;
};
