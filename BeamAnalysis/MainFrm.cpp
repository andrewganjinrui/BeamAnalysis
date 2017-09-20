// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "BeamAnalysis.h"
#include "CommandView.h"
#include "BeamAnalysisView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_STARTCHECK, &CMainFrame::OnStartcheck)
	ON_COMMAND(ID_CALBEAMWIDTH, &CMainFrame::OnCalbeamwidth)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	//���ĵ�����Ϊ���������ͻ���
	if(!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n"); 
		return FALSE;
	}
	//����߿ͻ����½�һ��������ͼ����
	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CCommandView), CSize(190,600), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	//���ұ߿ͻ����½�һ����ͼ����������ʾͼ��
	if(!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CBeamAnalysisView), CSize(650,650), pContext))
	{
		TRACE("Failed to create preview pane\n"); 
		return FALSE;
	}
	return true;
}


void CMainFrame::OnStartcheck()
{
	// TODO: �ڴ���������������
	//ȡ��displayView����ͼ��(1��0)���
	CBeamAnalysisView *pView  = (CBeamAnalysisView *)m_wndSplitter.GetPane(0,1);
	pView->OnCheck();
}

void CMainFrame::OnCalbeamwidth()
{
	// TODO: �ڴ���������������
	//ȡ��displayView����ͼ��(1��0)���
	CBeamAnalysisView *pView  = (CBeamAnalysisView *)m_wndSplitter.GetPane(0,1);
	pView->SetROIImg();
	pView->CalculatePara();
	pView->CalculateBeamWidth();
}
