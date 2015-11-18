/**********************************************************************************************//**
 * \file	ProjectOutput.h
 *
 * \brief	导出方法接口
 **************************************************************************************************/

#pragma once;

#include "Material.h"
#include "CommonData.h"
#include <string>
#include <set>

using namespace std;

/**********************************************************************************************//**
 * \fn	void geometryOutput(string filename, set<CString>& outMats, set<Material>& antiMaterials);
 *
 * \brief	Geometry.rad文件导出方法
 *
 * \author	Liuxq
 * \date	2015/11/18
 *
 * \param	filename			 	文件名.
 * \param [in,out]	outMats		 	输出材质集合
 * \param [in,out]	antiMaterials	输出反物质材质集合（废弃）
 **************************************************************************************************/

void geometryOutput(string filename, set<CString>& outMats, set<Material>& antiMaterials);

/**********************************************************************************************//**
 * \fn	void materialOutput(string filename, MaterialSet& materials, set<CString>& mats, set<Material>& antiMaterials);
 *
 * \brief	matierial.rad材质文件导出方法
 *
 * \author	Liuxq
 * \date	2015/11/18
 *
 * \param	filename			 	文件名
 * \param [in,out]	materials	 	材质模板集合
 * \param [in,out]	mats		 	材质导出
 * \param [in,out]	antiMaterials	反物质材质导出（废弃）
 **************************************************************************************************/

void materialOutput(string filename, MaterialSet& materials, set<CString>& mats, set<Material>& antiMaterials);

/**********************************************************************************************//**
 * \fn	void RoomOutput(string roomFile, string gridDir);
 *
 * \brief	房间信息导出(txt)
 *
 * \author	Liuxq
 * \date	2015/11/18
 *
 * \param	roomFile	The room file.
 * \param	gridDir 	The grid dir.
 **************************************************************************************************/

void RoomOutput(string roomFile, string gridDir);

/**********************************************************************************************//**
 * \fn	void LumOutput(string lumFilePre, string controlFilePre, string personFilePre);
 *
 * \brief	灯具(gx)、控制分组(ctl)、人员(occ)文件导出
 *
 * \author	Liuxq
 * \date	2015/11/18
 *
 * \param	lumFilePre	  	The lum file pre.
 * \param	controlFilePre	The control file pre.
 * \param	personFilePre 	The person file pre.
 **************************************************************************************************/

void LumOutput(string lumFilePre, string controlFilePre, string personFilePre);

/**********************************************************************************************//**
 * \fn	void RoomOutToVector(vector<OutRoom>& surfaces, set<CString>& Mats,set<Material>& antiMaterials);
 *
 * \brief	房间内相关信息导出到集合中，供其他方法使用
 *
 * \author	Liuxq
 * \date	2015/11/18
 *
 * \param [in,out]	surfaces	 	面数据
 * \param [in,out]	Mats		 	材质数据
 * \param [in,out]	antiMaterials	反物质数据（废弃）
 **************************************************************************************************/

void RoomOutToVector(vector<OutRoom>& surfaces, set<CString>& Mats,set<Material>& antiMaterials);