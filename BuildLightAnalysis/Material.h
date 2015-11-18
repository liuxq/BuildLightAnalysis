/**********************************************************************************************//**
 * \file	Material.h
 *
 * \brief	材质类
 **************************************************************************************************/

#pragma once

#include <vector>
#include <string>

using namespace std;

/**********************************************************************************************//**
 * \struct	Material
 *
 * \brief	材料：类型，名字，参数
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

struct Material
{
	string type;
	string name;
	vector<string> args;
};
bool operator<(const Material& _Left, const Material& _Right);

/**********************************************************************************************//**
 * \class	MaterialSet
 *
 * \brief	材料集合
 *
 * \author	Liuxq
 * \date	2015/11/18
 **************************************************************************************************/

class MaterialSet
{
public:
	MaterialSet()
	{

	}

	/**********************************************************************************************//**
	 * \fn	bool MaterialSet::loadTemplate(CString file);
	 *
	 * \brief	从模板中导入材料集合
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	file	The file.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool loadTemplate(CString file);

public:

	/** \brief	材料集合 */
	vector<Material> m_materials;
};
