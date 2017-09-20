#pragma once
#include "afxcmn.h"



// CCommandView 窗体视图

class CCommandView : public CFormView
{
	DECLARE_DYNCREATE(CCommandView)

protected:
	CCommandView();           // 动态创建所使用的受保护的构造函数
	virtual ~CCommandView();

public:
	enum { IDD = IDD_DIALOG1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
public:
	void SetBeamWidth(float dx, float dy);
	afx_msg void OnBnClickedRadioYes();
	afx_msg void OnBnClickedRadioNo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CView *GetDisplayView();

private:
	CSliderCtrl m_CtrlT;
	BOOL m_ChooseBaseline;
	float m_Dx;
	float m_Dy;
};


