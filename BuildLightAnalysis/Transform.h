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

	Vec2d RealToScreen(Vec2d pos)
	{
		pos *= scale;
		pos += center;
		pos.y = height - pos.y;
		return pos;
	}

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