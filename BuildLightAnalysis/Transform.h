/**********************************************************************************************//**
 * \file	Transform.h
 *
 * \brief	变换类，记录了平移缩放
 **************************************************************************************************/

#pragma once;

#include "vec2.h"


class Transform
{
public:
	Transform(){}
	Transform(double s, Vec2d c, double h):scale(s), center(c), height(h)
	{

	}
	void Set(double s, Vec2d c)
	{
		scale = s;
		center = c;
	}

	/**********************************************************************************************//**
	 * \fn	Vec2d Transform::RealToScreen(Vec2d pos)
	 *
	 * \brief	实际坐标 to 屏幕坐标.
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	pos	The position.
	 *
	 * \return	A Vec2d.
	 **************************************************************************************************/

	Vec2d RealToScreen(Vec2d pos)
	{
		pos *= scale;
		pos += center;
		pos.y = height - pos.y;
		if (pos.x > 5000.0)	pos.x = 5000.0;
		if (pos.x < -5000.0)pos.x = -5000.0;
		if (pos.y > 5000.0)	pos.y = 5000.0;
		if (pos.y < -5000.0)pos.y = -5000.0;
		return pos;
	}

	/**********************************************************************************************//**
	 * \fn	Vec2d Transform::ScreenToReal(Vec2d pos)
	 *
	 * \brief	屏幕坐标 to 实际坐标.
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	pos	The position.
	 *
	 * \return	A Vec2d.
	 **************************************************************************************************/

	Vec2d ScreenToReal(Vec2d pos)
	{
		Vec2d p(pos.x, pos.y);
		p.y = height - p.y;
		p -= center;
		p /= scale;
		return p;
	}

public:

	double scale;
	Vec2d center;
	double height;
};