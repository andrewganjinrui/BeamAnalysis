// BeamAnalysisDoc.cpp : CBeamAnalysisDoc 类的实现
//

#include "stdafx.h"
#include "BeamAnalysis.h"

#include "BeamAnalysisDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBeamAnalysisDoc

IMPLEMENT_DYNCREATE(CBeamAnalysisDoc, CDocument)

BEGIN_MESSAGE_MAP(CBeamAnalysisDoc, CDocument)
END_MESSAGE_MAP()


// CBeamAnalysisDoc 构造/析构

CBeamAnalysisDoc::CBeamAnalysisDoc()
{
	// TODO: 在此添加一次性构造代码
	m_pDib = NULL;
	m_SizeDoc = CSize(650, 650);
	
}

CBeamAnalysisDoc::~CBeamAnalysisDoc()
{
	FreeDib();
}

BOOL CBeamAnalysisDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CBeamAnalysisDoc 序列化

void CBeamAnalysisDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CBeamAnalysisDoc 诊断

#ifdef _DEBUG
void CBeamAnalysisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBeamAnalysisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBeamAnalysisDoc 命令

void CBeamAnalysisDoc::FreeDib()
{
	if (m_pDib)
	{
		FreeImage_Unload(m_pDib);
		m_pDib = NULL;
	}
}

BOOL CBeamAnalysisDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO: Add your specialized creation code here
	FREE_IMAGE_FORMAT ff = FIF_UNKNOWN;
	ff = FreeImage_GetFileType(lpszPathName);
	FreeDib();
	m_pDib = FreeImage_Load(ff, lpszPathName, 0);

	int lHeight = FreeImage_GetHeight(m_pDib);
	int lWidth = FreeImage_GetWidth(m_pDib);
	m_SizeDoc = CSize(lWidth, lHeight);

	POSITION pos;
	pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		CScrollView* pVeiw = (CScrollView*)GetNextView(pos);
		if (pVeiw != NULL)
		{
			pVeiw->SetScrollSizes(MM_TEXT, m_SizeDoc);
		}
	}

	return TRUE;
}
