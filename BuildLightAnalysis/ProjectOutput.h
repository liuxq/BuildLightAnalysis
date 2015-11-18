/**********************************************************************************************//**
 * \file	ProjectOutput.h
 *
 * \brief	���������ӿ�
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
 * \brief	Geometry.rad�ļ���������
 *
 * \author	Liuxq
 * \date	2015/11/18
 *
 * \param	filename			 	�ļ���.
 * \param [in,out]	outMats		 	������ʼ���
 * \param [in,out]	antiMaterials	��������ʲ��ʼ��ϣ�������
 **************************************************************************************************/

void geometryOutput(string filename, set<CString>& outMats, set<Material>& antiMaterials);

/**********************************************************************************************//**
 * \fn	void materialOutput(string filename, MaterialSet& materials, set<CString>& mats, set<Material>& antiMaterials);
 *
 * \brief	matierial.rad�����ļ���������
 *
 * \author	Liuxq
 * \date	2015/11/18
 *
 * \param	filename			 	�ļ���
 * \param [in,out]	materials	 	����ģ�弯��
 * \param [in,out]	mats		 	���ʵ���
 * \param [in,out]	antiMaterials	�����ʲ��ʵ�����������
 **************************************************************************************************/

void materialOutput(string filename, MaterialSet& materials, set<CString>& mats, set<Material>& antiMaterials);

/**********************************************************************************************//**
 * \fn	void RoomOutput(string roomFile, string gridDir);
 *
 * \brief	������Ϣ����(txt)
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
 * \brief	�ƾ�(gx)�����Ʒ���(ctl)����Ա(occ)�ļ�����
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
 * \brief	�����������Ϣ�����������У�����������ʹ��
 *
 * \author	Liuxq
 * \date	2015/11/18
 *
 * \param [in,out]	surfaces	 	������
 * \param [in,out]	Mats		 	��������
 * \param [in,out]	antiMaterials	���������ݣ�������
 **************************************************************************************************/

void RoomOutToVector(vector<OutRoom>& surfaces, set<CString>& Mats,set<Material>& antiMaterials);