// BeamAnalysisView.h : CBeamAnalysisView 类的接口
//


#pragma once
#include "BeamAnalysisDoc.h"

struct BeamPara
{
	float X_means;
	float Y_means;
	float sigma_X;
	float sigma_Y;
	float sigma_XY;
};
struct EightAreaTeam
{
	int x;//位置信息
	int y;
	struct EightAreaTeam *next;
};//定义队的数据结构

class CBeamAnalysisView : public CScrollView
{
protected: // 仅从序列化创建
	CBeamAnalysisView();
	DECLARE_DYNCREATE(CBeamAnalysisView)

// 属性
public:
	CBeamAnalysisDoc* GetDocument() const;

	inline void SetT(float pos){m_nT = pos;}
	inline void ChooseBaseline(BOOL nChoose){m_ChooseBaseline = nChoose;}
	CView *GetCommandView();
	inline void ChangebHscrollState(){m_bHscroll = true;}
// 操作
private:
	CRect m_BeamRect;
	CRect m_ROIRect;

	float m_nT;
	BOOL m_ChooseBaseline;

	int m_WidthROI;
	int m_HeightROI;
	LPBYTE m_lpDataROI;
	LPBYTE m_lpflag;
	LPBYTE m_lpConvolution;

	BeamPara m_BeamPara;
	
	bool m_bHscroll;
// 重写
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// 实现
public:
	virtual ~CBeamAnalysisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	//阈值分割
	void Threshold_OTSU(LPBYTE lpDib, int lWidth, int lHeight);
	//获取矩形框
	CRect& RectMarke(LPBYTE lpDib, int lWidth, int lHeight);
	//获取积分区域
	void SetROIRect();
	//获取积分区域的像素数据
	void SetROIImg();
	//计算积分区域的一阶距、二阶距
	void CalculatePara();
	//计算积分区域的光斑大小
	void CalculateBeamWidth();
	//
	void Insert(struct EightAreaTeam *head,int yi,int xi);
	//
	void Delete(struct EightAreaTeam  *head);
	//八连通域分析，标记区域
	int Eightliantong(LPBYTE data, int wth, int hth, int* flag);
	//定位光斑
	void LocateRect(LPBYTE lpDib, int lWidth, int lHeight, CRect& rect);
	//均值滤波
	void Smooth(LPBYTE lpDib, int lWidth, int lHeight, int nTempWidth, int nTempHeight);
	//计算样本数据的均值、均方差
	void CalculateBaselinePara(LPBYTE lpDib, int n, float& Means, float& StandardDeviation);
	//从图像数据中提取背景像素数据
	void SetBkData(LPBYTE lpDib, int lWidth, int lHeight, LPBYTE lpBKData);
	//计算基线偏移值
	int BaseLine();
	//获取矩形框以框住光斑
	void OnCheck();
	//
	void CalDataThread();
};

#ifndef _DEBUG  // BeamAnalysisView.cpp 中的调试版本
inline CBeamAnalysisDoc* CBeamAnalysisView::GetDocument() const
   { return reinterpret_cast<CBeamAnalysisDoc*>(m_pDocument); }
#endif

