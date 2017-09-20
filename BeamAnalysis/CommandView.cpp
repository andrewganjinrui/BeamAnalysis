// CommandView.cpp : 实现文件
//

#include "stdafx.h"
#include "BeamAnalysis.h"
#include "CommandView.h"
#include "BeamAnalysisDoc.h"
#include "BeamAnalysisView.h"
#include "MainFrm.h"

// CCommandView

IMPLEMENT_DYNCREATE(CCommandView, CFormView)

CCommandView::CCommandView()
	: CFormView(CCommandView::IDD)
	, m_ChooseBaseline(FALSE)
{
	
}

CCommandView::~CCommandView()
{
}

void CCommandView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_T, m_CtrlT);
	DDX_Radio(pDX, IDC_RADIO_YES, m_ChooseBaseline);
}

BEGIN_MESSAGE_MAP(CCommandView, CFormView)
	ON_BN_CLICKED(IDC_RADIO_YES, &CCommandView::OnBnClickedRadioYes)
	ON_BN_CLICKED(IDC_RADIO_NO, &CCommandView::OnBnClickedRadioNo)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CCommandView 诊断

#ifdef _DEBUG
void CCommandView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCommandView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCommandView 消息处理程序
void CCommandView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	//设置左操作选择的值范围及初始值

	m_CtrlT.SetRange(15, 25);
	m_CtrlT.SetPos(20);
	m_ChooseBaseline = FALSE;
	CString str;
	str.Format(_T("%.2f"), 2.0);
	SetDlgItemText(IDC_SHOW_T, str);

	UpdateData(FALSE);
}

CView *CCommandView::GetDisplayView() 
{
	CBeamAnalysisApp   *pApp   = (CBeamAnalysisApp *)AfxGetApp();		//取得当前应用程序对象句柄
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;	//取得应用程序主窗口句柄
	//取得displayView子视图即(1，0)句柄
	CView      *pView  = (CView *)pFrame->m_wndSplitter.GetPane(0,1);
	return pView;
}

void CCommandView::OnBnClickedRadioYes()
{
	// TODO: 在此添加控件通知处理程序代码
	CBeamAnalysisView *pView = (CBeamAnalysisView *)GetDisplayView();	//取得演示窗口的指针
	pView->ChooseBaseline(FALSE);
}

void CCommandView::OnBnClickedRadioNo()
{
	// TODO: 在此添加控件通知处理程序代码
	CBeamAnalysisView *pView = (CBeamAnalysisView *)GetDisplayView();	//取得演示窗口的指针
	pView->ChooseBaseline(TRUE);
}

void CCommandView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBeamAnalysisView *pView = (CBeamAnalysisView *)GetDisplayView();	//取得演示窗口的指针
	UpdateData();

	float T = (float)m_CtrlT.GetPos() / 10.0;
	pView->SetT(T);
	pView->ChangebHscrollState();

	CString str;
	str.Format(_T("%.2f"), T);
	SetDlgItemText(IDC_SHOW_T, str);
	
	pView->SetROIRect();
//	pView->CalDataThread();
	pView->SetROIImg();
	pView->CalculatePara();
	pView->CalculateBeamWidth();

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCommandView::SetBeamWidth(float dx, float dy)
{
	CString str;
	str.Format(_T("%.2f"), dx);
	SetDlgItemText(IDC_EDIT_DX, str);
	str.Format(_T("%.2f"), dy);
	SetDlgItemText(IDC_EDIT_DY, str);
}