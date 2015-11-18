/**********************************************************************************************//**
 * \file	Transform.h
 *
 * \brief	�任�࣬��¼��ƽ������
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
	 * \brief	ʵ������ to ��Ļ����.
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
	 * \brief	��Ļ���� to ʵ������.
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