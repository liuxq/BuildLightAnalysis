
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