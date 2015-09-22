
#include "stdafx.h"
#include "Material.h"
#include "MathUtility.h"

#include <fstream>
#include <string>
#include <sstream>
using namespace std;

bool MaterialSet::loadTemplate(CString file)
{
	ifstream inputFile(CStringToString(file));
	if (!inputFile.is_open())
	{
		AfxMessageBox(_T("缺少材质模板，请将Template_material.rad文件放入工程文件夹下"));
		return false;
	}
	string strLine;
	
	string first, type, name, arg;
	
	while(getline(inputFile, strLine))
	{
		stringstream sStrem;
		sStrem << strLine;
		if (strLine.empty() || sStrem.peek() == '#')
		{
			continue;
		}

		sStrem >> first;
		if (first == "void")
		{
			Material material;
			m_materials.push_back(material);
			sStrem >> type >> name;
			m_materials.back().type = type;
			m_materials.back().name = name;
		}
		else
		{
			m_materials.back().args.push_back(first);
			while(sStrem >> arg)
			{
				m_materials.back().args.push_back(arg);
			}
		}
	}

	return true;
}

bool operator<(const Material& _Left, const Material& _Right)
{
	if (_Left.name < _Right.name || (_Left.name == _Right.name && _Left.type < _Right.type))
	{
		return true;
	}
	return false;
}