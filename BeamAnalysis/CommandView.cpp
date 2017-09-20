// CommandView.cpp : ʵ���ļ�
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


// CCommandView ���

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


// CCommandView ��Ϣ�������
void CCommandView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	//���������ѡ���ֵ��Χ����ʼֵ

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
	CBeamAnalysisApp   *pApp   = (CBeamAnalysisApp *)AfxGetApp();		//ȡ�õ�ǰӦ�ó��������
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;	//ȡ��Ӧ�ó��������ھ��
	//ȡ��displayView����ͼ��(1��0)���
	CView      *pView  = (CView *)pFrame->m_wndSplitter.GetPane(0,1);
	return pView;
}

void CCommandView::OnBnClickedRadioYes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBeamAnalysisView *pView = (CBeamAnalysisView *)GetDisplayView();	//ȡ����ʾ���ڵ�ָ��
	pView->ChooseBaseline(FALSE);
}

void CCommandView::OnBnClickedRadioNo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBeamAnalysisView *pView = (CBeamAnalysisView *)GetDisplayView();	//ȡ����ʾ���ڵ�ָ��
	pView->ChooseBaseline(TRUE);
}

void CCommandView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBeamAnalysisView *pView = (CBeamAnalysisView *)GetDisplayView();	//ȡ����ʾ���ڵ�ָ��
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