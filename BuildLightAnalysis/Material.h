/**********************************************************************************************//**
 * \file	Material.h
 *
 * \brief	������
 **************************************************************************************************/

#pragma once

#include <vector>
#include <string>

using namespace std;

/**********************************************************************************************//**
 * \struct	Material
 *
 * \brief	���ϣ����ͣ����֣�����
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
 * \brief	���ϼ���
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
	 * \brief	��ģ���е�����ϼ���
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

	/** \brief	���ϼ��� */
	vector<Material> m_materials;
};
