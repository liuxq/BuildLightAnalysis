
// BuildLightAnalysisDoc.h : interface of the CBuildLightAnalysisDoc class
//

#pragma once

#include "Material.h"
#include "CommonData.h"

class CBuildLightAnalysisDoc : public CDocument
{
protected: // create from serialization only
	CBuildLightAnalysisDoc();
	DECLARE_DYNCREATE(CBuildLightAnalysisDoc)

// Attributes
public:
	CString m_projectName;
	CString m_projectLocation;
	bool m_bIsOpen;
public:
	vector<Material>& getMaterials()
	{
		return m_material.m_materials;
	}
	vector<CString>& getCitys()
	{
		return m_citys;
	}
	vector<RoomType>& getRoomTypes()
	{
		return m_roomTypes;
	}
private:
	MaterialSet m_material;
	vector<CString> m_citys;
	vector<RoomType> m_roomTypes;
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CBuildLightAnalysisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void clear();
	void load(ifstream& in);
	void loadMaterial();
	void loadCity();
	void loadRoomType();
	void save(ofstream& out);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOutput();
};
