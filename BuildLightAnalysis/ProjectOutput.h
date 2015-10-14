#pragma once;

#include "Material.h"
#include "CommonData.h"
#include <string>
#include <set>

using namespace std;

void geometryOutput(string filename, set<CString>& outMats, set<Material>& antiMaterials);

void materialOutput(string filename, MaterialSet& materials, set<CString>& mats, set<Material>& antiMaterials);

void RoomOutput(string roomFile, string gridDir);
void LumOutput(string roomFile, string grid1File, string grid2File);

void RoomOutToVector(vector<OutRoom>& surfaces, set<CString>& Mats,set<Material>& antiMaterials);