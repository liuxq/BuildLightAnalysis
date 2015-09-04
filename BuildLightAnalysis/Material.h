#pragma once

#include <vector>
#include <string>

using namespace std;


struct Material
{
	string type;
	string name;
	vector<string> args;
};
class MaterialSet
{
public:
	MaterialSet()
	{

	}

	bool loadTemplate(CString file);

public:
	vector<Material> m_materials;
};
