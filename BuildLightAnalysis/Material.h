#pragma once

#include <vector>
#include <string>

using namespace std;


struct Material
{
	string type;
	string name;
	vector<string> args;

// 	bool operator<(const Material& _Left, const Material& _Right) const
// 	{
// 
// 	}
// 	bool operator < (Material& b) const
// 	{
// 		if (name != b.name)
// 		{
// 			return true;
// 		}
// 		return false;
// 	}
};
bool operator<(const Material& _Left, const Material& _Right);

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
