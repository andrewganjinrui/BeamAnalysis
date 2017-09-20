// BeamAnalysisView.h : CBeamAnalysisView ��Ľӿ�
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
	int x;//λ����Ϣ
	int y;
	struct EightAreaTeam *next;
};//����ӵ����ݽṹ

class CBeamAnalysisView : public CScrollView
{
protected: // �������л�����
	CBeamAnalysisView();
	DECLARE_DYNCREATE(CBeamAnalysisView)

// ����
public:
	CBeamAnalysisDoc* GetDocument() const;

	inline void SetT(float pos){m_nT = pos;}
	inline void ChooseBaseline(BOOL nChoose){m_ChooseBaseline = nChoose;}
	CView *GetCommandView();
	inline void ChangebHscrollState(){m_bHscroll = true;}
// ����
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
// ��д
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// ʵ��
public:
	virtual ~CBeamAnalysisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	//��ֵ�ָ�
	void Threshold_OTSU(LPBYTE lpDib, int lWidth, int lHeight);
	//��ȡ���ο�
	CRect& RectMarke(LPBYTE lpDib, int lWidth, int lHeight);
	//��ȡ��������
	void SetROIRect();
	//��ȡ�����������������
	void SetROIImg();
	//������������һ�׾ࡢ���׾�
	void CalculatePara();
	//�����������Ĺ�ߴ�С
	void CalculateBeamWidth();
	//
	void Insert(struct EightAreaTeam *head,int yi,int xi);
	//
	void Delete(struct EightAreaTeam  *head);
	//����ͨ��������������
	int Eightliantong(LPBYTE data, int wth, int hth, int* flag);
	//��λ���
	void LocateRect(LPBYTE lpDib, int lWidth, int lHeight, CRect& rect);
	//��ֵ�˲�
	void Smooth(LPBYTE lpDib, int lWidth, int lHeight, int nTempWidth, int nTempHeight);
	//�����������ݵľ�ֵ��������
	void CalculateBaselinePara(LPBYTE lpDib, int n, float& Means, float& StandardDeviation);
	//��ͼ����������ȡ������������
	void SetBkData(LPBYTE lpDib, int lWidth, int lHeight, LPBYTE lpBKData);
	//�������ƫ��ֵ
	int BaseLine();
	//��ȡ���ο��Կ�ס���
	void OnCheck();
	//
	void CalDataThread();
};

#ifndef _DEBUG  // BeamAnalysisView.cpp �еĵ��԰汾
inline CBeamAnalysisDoc* CBeamAnalysisView::GetDocument() const
   { return reinterpret_cast<CBeamAnalysisDoc*>(m_pDocument); }
#endif

