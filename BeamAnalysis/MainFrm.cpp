// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
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
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	//把文档划分为左右两个客户区
	if(!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n"); 
		return FALSE;
	}
	//在左边客户区新建一个命令视图窗口
	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CCommandView), CSize(190,600), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	//在右边客户区新建一个视图窗口用于显示图形
	if(!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CBeamAnalysisView), CSize(650,650), pContext))
	{
		TRACE("Failed to create preview pane\n"); 
		return FALSE;
	}
	return true;
}


void CMainFrame::OnStartcheck()
{
	// TODO: 在此添加命令处理程序代码
	//取得displayView子视图即(1，0)句柄
	CBeamAnalysisView *pView  = (CBeamAnalysisView *)m_wndSplitter.GetPane(0,1);
	pView->OnCheck();
}

void CMainFrame::OnCalbeamwidth()
{
	// TODO: 在此添加命令处理程序代码
	//取得displayView子视图即(1，0)句柄
	CBeamAnalysisView *pView  = (CBeamAnalysisView *)m_wndSplitter.GetPane(0,1);
	pView->SetROIImg();
	pView->CalculatePara();
	pView->CalculateBeamWidth();
}
