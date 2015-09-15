#pragma once;

#include "Material.h"
#include <string>
#include <set>

using namespace std;

void geometryOutput(string filename, set<CString>& outMats);

void materialOutput(string filename, MaterialSet& materials, set<CString>& mats);

void RoomOutput(string filename);