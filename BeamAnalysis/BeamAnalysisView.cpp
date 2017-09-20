// BeamAnalysisView.cpp : CBeamAnalysisView ���ʵ��
//

#include "stdafx.h"
#include "BeamAnalysis.h"
#include "MainFrm.h"
#include "BeamAnalysisDoc.h"
#include "BeamAnalysisView.h"
#include "CommandView.h"
#include <math.h>

#define  aa 2.82842712

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT ThreadCalculae(LPVOID lpParam)
{
	CBeamAnalysisView* pView = (CBeamAnalysisView*)lpParam;
	pView->SetROIImg();
	pView->CalculatePara();
	pView->CalculateBeamWidth();
	return 0;
}

// CBeamAnalysisView

IMPLEMENT_DYNCREATE(CBeamAnalysisView, CScrollView)

BEGIN_MESSAGE_MAP(CBeamAnalysisView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	
END_MESSAGE_MAP()

// CBeamAnalysisView ����/����

CBeamAnalysisView::CBeamAnalysisView()
{
	// TODO: �ڴ˴���ӹ������
	m_BeamRect.bottom = 0;
	m_BeamRect.right = 0;
	m_BeamRect.top = 0;
	m_BeamRect.left = 0;

	m_ROIRect.bottom = 0;
	m_ROIRect.right = 0;
	m_ROIRect.top = 0;
	m_ROIRect.left = 0;

	m_nT = 2.0;
	m_ChooseBaseline = FALSE;
	
	m_lpConvolution = NULL;
	m_lpDataROI = NULL;
	m_lpflag = NULL;
	m_WidthROI = 0;
	m_HeightROI = 0;

	m_BeamPara.sigma_X = 0.0;
	m_BeamPara.sigma_XY = 0.0;
	m_BeamPara.sigma_Y = 0.0;
	m_BeamPara.X_means = 0.0;
	m_BeamPara.Y_means = 0.0;

	m_bHscroll = false;
}

CBeamAnalysisView::~CBeamAnalysisView()
{
	if (m_lpDataROI)
	{
		delete[] m_lpDataROI;
		m_lpDataROI = NULL;
	}
	if (m_lpflag)
	{
		delete[] m_lpflag;
		m_lpflag = NULL;
	}
	if (m_lpConvolution)
	{
		delete[] m_lpConvolution;
		m_lpConvolution = NULL;
	}
}

BOOL CBeamAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CBeamAnalysisView ����

void CBeamAnalysisView::OnDraw(CDC* pDC)
{
	CBeamAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	FIBITMAP* pDib = pDoc->GetDib();

	LPBYTE pDibBites = FreeImage_GetBits(pDib);
	LPBITMAPINFO pInfo = FreeImage_GetInfo(pDib);
	int lHeight = FreeImage_GetHeight(pDib);
	int lWidth = FreeImage_GetWidth(pDib);

	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);// ������ʾģʽ		
	::StretchDIBits(pDC->GetSafeHdc(),					// �豸�������
		0,                 //Ŀ��X����
		0,                 // Ŀ��Y����
		lWidth,            // Ŀ����
		lHeight,           // Ŀ��߶�
		0,                 // ԴX����
		0,                 // ԴY����
		lWidth,            // Դ���
		lHeight,           // Դ�߶�
		pDibBites,         // ָ��dib���ص�ָ��
		pInfo,             // ָ��λͼ��Ϣ�ṹ��ָ��
		DIB_RGB_COLORS,    // ʹ�õ���ɫ��Ŀ
		SRCCOPY);          // ��դ��������

	////////////////////////////////////////////////////////////////////////////
	int x = 0, y = 0;
	if (m_bHscroll)
	{
		x = m_ROIRect.left + m_BeamPara.X_means;
		y = m_ROIRect.bottom - m_BeamPara.Y_means;
	}	
	////////////////////////////////////////////////////////////////////////////

	CPen PenRed(PS_SOLID, 1, RGB(255, 0, 0)), PenGreen(PS_SOLID, 1, RGB(180, 100, 120)), PenXX(PS_SOLID, 1, RGB(250, 10, 220)), *Oldpen;	
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pDC->SelectObject(pBrush);
	
	if (m_bHscroll)
	{
		Oldpen = pDC->SelectObject(&PenRed);
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, lHeight);
		pDC->MoveTo(0, y);
		pDC->LineTo(lWidth, y);
	}
	Oldpen = pDC->SelectObject(&PenGreen);
	pDC->Rectangle(m_BeamRect);
	Oldpen = pDC->SelectObject(&PenXX);
	pDC->Rectangle(m_ROIRect);

	pDC->SelectObject(Oldpen);
	DeleteObject(&PenGreen);
	DeleteObject(&PenRed);
	DeleteObject(&PenXX);
}

// CBeamAnalysisView ��ӡ

BOOL CBeamAnalysisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CBeamAnalysisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CBeamAnalysisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CBeamAnalysisView ���

#ifdef _DEBUG
void CBeamAnalysisView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CBeamAnalysisView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CBeamAnalysisDoc* CBeamAnalysisView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBeamAnalysisDoc)));
	return (CBeamAnalysisDoc*)m_pDocument;
}
#endif //_DEBUG


// CBeamAnalysisView ��Ϣ�������
void CBeamAnalysisView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocDimension());
}

/*************************************************************************
* �������ƣ�
*   OnCheck()
* ����:
*	��
* ����ֵ:
*   ��
* ����:
*   �ú������ڻ�ȡ���ο��Կ�ס��ߡ� 
************************************************************************/
void CBeamAnalysisView::OnCheck()
{
	// TODO: �ڴ���������������
	CBeamAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	FIBITMAP* pDib = pDoc->GetDib();
	FIBITMAP* pDibGray = FreeImage_ConvertToGreyscale(pDib);

	LPBYTE lpDib = FreeImage_GetBits(pDibGray);
	int lHeight = FreeImage_GetHeight(pDibGray);
	int lWidth = FreeImage_GetWidth(pDibGray);

	Threshold_OTSU(lpDib, lWidth, lHeight);
	m_BeamRect = RectMarke(lpDib, lWidth, lHeight);

	LPBYTE lpDibBites = FreeImage_GetBits(pDib);
	if (m_lpConvolution)
	{
		delete[] m_lpConvolution;
		m_lpConvolution = NULL;
	}
	m_lpConvolution = (LPBYTE)new char[lWidth * lHeight];
	memcpy(m_lpConvolution, lpDibBites, lWidth * lHeight);
	Smooth(m_lpConvolution, lWidth, lHeight, lWidth * 3 / 100, lHeight * 2 / 100);

	if (pDibGray)
	{
		FreeImage_Unload(pDibGray);
		pDibGray = NULL;
	}

	SetROIRect();
}

/*************************************************************************
* �������ƣ�
*   Threshold_OTSU()
* ����:
*	LPBYTE                   - ͼ������ָ��
*	int                      - ͼ����
*	int                      - ͼ��߶�
* ����ֵ:
*   ��
* ����:
*   �ú������ڻ�ȡ��ֵ�� 
************************************************************************/
void CBeamAnalysisView::Threshold_OTSU(LPBYTE lpDib, int lWidth, int lHeight) 
{
	// TODO: Add your control notification handler code here

	
	int nHistogram[256];                                //�Ҷ�ֱ��ͼ
	int N = lWidth * lHeight;                           //��������
	int i;
	// ��ʼ������
	for(i = 0; i < 256; i++)
		nHistogram[i] = 0;
	
	byte pixel;
	// ֱ��ͼ����ͳ��
	for (int m = 0; m < lHeight; m++)
	{
		for (int n = 0; n < lWidth; n++)
		{
			pixel = *(lpDib + m * lWidth + n);		
			nHistogram[pixel]++;
		}
	}

	int nThreshold = 0, temp = 0;
	for (i = 0; i < 255; i++)
	{
		if (nHistogram[i] >= temp)
		{
			temp = nHistogram[i];
			nThreshold = i + 1;
		}
	}

	for (int x = 0; x < lHeight; x++)
	{
		for (int y = 0; y < lWidth; y++)
		{
			pixel = *(lpDib + lWidth * x + y);
			if(pixel >= nThreshold)
				*(lpDib + lWidth * x + y) = 255;
			else
				*(lpDib + lWidth * x + y) = 0;
		}
	}
}

/*************************************************************************
* �������ƣ�
*   RectMarke()
* ����:
*	LPBYTE                   - ͼ������ָ��
*	int                      - ͼ����
*	int                      - ͼ��߶�
* ����ֵ:
*   CRect
* ����:
*   �ú������ڻ�ȡ���ο� 
************************************************************************/
CRect& CBeamAnalysisView::RectMarke(LPBYTE lpDib, int lWidth, int lHeight)
{
	int i, j;
	int loffset;
	int pixel;

	int *flag=new int[lWidth * lHeight];
	for(i=0; i < lWidth * lHeight; i++)//��ǵĳ�ʼ��
	{
		flag[i]=0;
	}
	int lMark = Eightliantong(lpDib, lWidth, lHeight, flag);

	int* pixelnum = new int[lMark + 1];
	for (i = 0; i <= lMark; i++)
	{
		pixelnum[i] = 0;
	}

	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			loffset = lWidth * i + j;
			pixel = *(flag + loffset);
			pixelnum[pixel] ++;
		}
	}

	int temp = 0, mark = 0;
	for (i = 1; i <= lMark; i++)
	{
		if (pixelnum[i] >= temp)
		{
			temp = pixelnum[i];
			mark = i;
		}
	}

	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			loffset = lWidth * i + j;
			if (*(flag + loffset) != mark)
				*(lpDib + loffset) = 0;
			else
				*(lpDib + loffset) = 255;
		}
	}
	if (flag)
	{
		delete []flag;
		flag = NULL;
	}
	if (pixelnum)
	{
		delete []pixelnum;
		pixelnum = NULL;
	}

	CRect rect(0, 0, 0, 0);
	LocateRect(lpDib, lWidth, lHeight, rect);

	return rect;
}

/*************************************************************************
* �������ƣ�
*   LocateRect()
* ����:
*	LPBYTE                   - ͼ������ָ��
*	int                      - ͼ����
*	int                      - ͼ��߶�
*   CRect                    - ����
* ����ֵ:
*   ��
* ����:
*   �ú������ڶ�λ��ߡ� 
************************************************************************/
void CBeamAnalysisView::LocateRect(LPBYTE lpDib, int lWidth, int lHeight, CRect& rect)
{
	int i, j, loffset, k = 0;
	bool flag = true;
	for(i = 0; i < lHeight && flag; i++)
	{
		k = i;
		for (j = 0; j < lWidth; j++)
		{	
			//ƫ��
			loffset = i * lWidth + j;
			if (*(lpDib + loffset) == 255)
			{
				rect.bottom = lHeight - 1 - k;
				flag = false;
				break;
			}
		}
	}
	flag = true;
	for(i = lHeight - 1; i >= 0 && flag; i--)
	{
		k = i;
		for (j = 0; j < lWidth; j++)
		{	
			//ƫ��
			loffset = i * lWidth + j;
			if (*(lpDib + loffset) == 255)
			{
				rect.top = lHeight - 1 - k;
				flag = false;
				break;
			}
		}
	}
	flag = true;
	for (j = 0; j < lWidth && flag; j++)
	{
		k = j;
		for (i = 0; i < lHeight; i++)
		{
			loffset = i * lWidth + j;
			if (*(lpDib + loffset) == 255)
			{
				rect.left = k;
				flag = false;
				break;
			}
		}
	}
	flag = true;
	for (j = lWidth - 1; j >= 0 && flag; j--)
	{
		k = j;
		for (i = 0; i < lHeight; i++)
		{
			loffset = i * lWidth + j;
			if (*(lpDib + loffset) == 255)
			{
				rect.right = k;
				flag = false;
				break;
			}
		}
	}
}

/*************************************************************************
* �������ƣ�
*   SetROIRect()
* ����:
*	��
* ����ֵ:
*   ��
* ����:
*   �ú������ڻ�ȡ�������� 
************************************************************************/
void CBeamAnalysisView::SetROIRect()
{
	int loffsetx = m_BeamRect.Width() * (m_nT - 1) / 2;
	int loffsety = m_BeamRect.Height() * (m_nT - 1) / 2;
	m_ROIRect.right = m_BeamRect.right + loffsetx;
	m_ROIRect.left = m_BeamRect.left - loffsetx;
	m_ROIRect.bottom = m_BeamRect.bottom + loffsety;
	m_ROIRect.top = m_BeamRect.top - loffsety;

	m_WidthROI = m_ROIRect.Width() + 1;
	m_HeightROI = m_ROIRect.Height() + 1;

	RedrawWindow();
}

/*************************************************************************
* �������ƣ�
*   SetROIImg()
* ����:
*	��
* ����ֵ:
*   ��
* ����:
*   �ú������ڻ�ȡ����������������ݡ� 
************************************************************************/
void CBeamAnalysisView::SetROIImg()
{
	CBeamAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	FIBITMAP* pDib = pDoc->GetDib();
	LPBYTE lpDib = FreeImage_GetBits(pDib);
	int lHeight = FreeImage_GetHeight(pDib);
	int lWidth = FreeImage_GetWidth(pDib);

	if (m_lpDataROI)
	{
		delete[] m_lpDataROI;
		m_lpDataROI = NULL;
	}
	if (m_lpflag)
	{
		delete[] m_lpflag;
		m_lpflag = NULL;
	}
	m_lpDataROI = (LPBYTE)new char[m_WidthROI * m_HeightROI];
	memset(m_lpDataROI, 0, m_WidthROI * m_HeightROI);

	m_lpflag = (LPBYTE)new char[lWidth * lHeight];
	memset(m_lpflag, 0, lWidth * lHeight);

	int k = 0;
	int baselineoffset = 0;

	for (int i = m_ROIRect.bottom; i >= m_ROIRect.top; i--)
	{
		for (int j = m_ROIRect.left; j <= m_ROIRect.right; j++)
		{
			*(m_lpflag + (lHeight - 1 - i) * lWidth + j) = 1;
			m_lpDataROI[k++] = *(lpDib + (lHeight - 1 - i) * lWidth + j);
		}
	}
	if(m_ChooseBaseline)
	{
		baselineoffset = BaseLine();
		for (int i = 0; i < k; i++)
		{
			m_lpDataROI[i] -= baselineoffset;
		}
	}
}

/*************************************************************************
* �������ƣ�
*   CalculatePara()
* ����:
*	��
* ����ֵ:
*   ��
* ����:
*   �ú������ڼ�����������һ�׾ࡢ���׾ࡣ 
************************************************************************/
void CBeamAnalysisView::CalculatePara()
{ 
	int y, x;
	int sumx = 0, sumy = 0, sum = 0;
	byte pixel;
	for (y = 0; y < m_HeightROI; y++)
	{
		for (x = 0; x < m_WidthROI; x++)
		{
			pixel = *(m_lpDataROI + y * m_WidthROI + x);
			sumx += pixel * x;
			sumy += pixel * y;
			sum += pixel;
		}
	}
	float X_means = (float)sumx / sum;
	float Y_means = (float)sumy / sum;
	
	float sumxx = 0.0, sumyy = 0.0, sumxy = 0.0;
	for (y = 0; y < m_HeightROI; y++)
	{
		for (x = 0; x < m_WidthROI; x++)
		{
			pixel = *(m_lpDataROI + y * m_WidthROI + x);
			sumxx += pixel * (x - X_means) * (x - X_means);
			sumyy += pixel * (y - Y_means) * (y - Y_means);
			sumxy += pixel * (x - X_means) * (y - Y_means);
		}
	}
	float sigma_X = sumxx / sum;
	float sigma_Y = sumyy / sum;
	float sigma_XY = sumxy / sum;

	m_BeamPara.X_means = X_means;
	m_BeamPara.Y_means = Y_means;
	m_BeamPara.sigma_X = sigma_X;
	m_BeamPara.sigma_Y = sigma_Y;
	m_BeamPara.sigma_XY = sigma_XY;
}

/*************************************************************************
* �������ƣ�
*   CalculateBeamWidth()
* ����:
*	��
* ����ֵ:
*   ��
* ����:
*   �ú������ڼ����������Ĺ�ߴ�С�� 
************************************************************************/
void CBeamAnalysisView::CalculateBeamWidth()
{
	CCommandView *pView = (CCommandView *)GetCommandView();	//ȡ����ʾ���ڵ�ָ��

	float sigma_X = 0.0;
	float sigma_Y = 0.0;
	float sigma_XY = 0.0;

	sigma_X = m_BeamPara.sigma_X;
	sigma_Y = m_BeamPara.sigma_Y;
	sigma_XY = m_BeamPara.sigma_XY;

	int r = (sigma_X - sigma_Y) / abs(sigma_X - sigma_Y);
	float c = r * sqrtf((sigma_X - sigma_Y) * (sigma_X - sigma_Y) + 4 * sigma_XY * sigma_XY);

	float dx, dy;
	if (sigma_X != sigma_Y)
	{
		dx = aa * sqrtf(sigma_X + sigma_Y + c);
		dy = aa * sqrtf(sigma_X + sigma_Y - c);
	}
	else
	{
		dx = aa * sqrtf(sigma_X + sigma_Y + 2 * abs(sigma_XY));
		dy = aa * sqrtf(sigma_X + sigma_Y - 2 * abs(sigma_XY));
	}
	pView->SetBeamWidth(dx, dy);
}

/*************************************************************************
* �������ƣ�
*   GetCommandView()
* ����:
*	��
* ����ֵ:
*   CView*
* ����:
*   �ú������ڻ�ȡ����ͼ�����ָ�롣 
************************************************************************/
CView *CBeamAnalysisView::GetCommandView() 
{
	CBeamAnalysisApp   *pApp   = (CBeamAnalysisApp *)AfxGetApp();		//ȡ�õ�ǰӦ�ó��������
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;	            //ȡ��Ӧ�ó��������ھ��
	//ȡ��displayView����ͼ��(0��0)���
	CView      *pView  = (CView *)pFrame->m_wndSplitter.GetPane(0, 0);
	return pView;
}

void CBeamAnalysisView::Insert(struct EightAreaTeam *head,int yi,int xi)//���
{
	struct EightAreaTeam *a,*b;
	a=(struct EightAreaTeam *)malloc(sizeof(struct EightAreaTeam));
	a->x=xi;
	a->y=yi;
	a->next=NULL;
	b=head;
	while(b->next!=NULL) b=b->next;
	b->next=a;
}

void CBeamAnalysisView::Delete(struct EightAreaTeam *head)//����
{
	struct EightAreaTeam *a;
	a=head->next;
	head->next=a->next;
	delete(a);
}

/*************************************************************************
* �������ƣ�
*   Eightliantong()
* ����:
*	LPBYTE                   - ͼ������ָ��
*	int                      - ͼ����
*	int                      - ͼ��߶�
*   int*                     - ���λָ��
* ����ֵ:
*   int                      - ������Ŀ
* ����:
*   �ú������ڰ���ͨ�������������� 
************************************************************************/
int CBeamAnalysisView::Eightliantong(LPBYTE data, int wth, int hth, int* flag)
{
	int pixel = 255;
	int num = 0;//��������󷵻�ֵ
	int a = 0, b = 0;
	int i, j;

	struct EightAreaTeam *head;
	head=(struct EightAreaTeam *)malloc(sizeof(struct EightAreaTeam));
	head->next=NULL;

	for(i=0;i<hth;i++)
	{
		for(j=0;j<wth;j++)
		{
			if(flag[i*wth+j]==0 && *(data+i*wth+j)==pixel)//�Ƿ��Ѿ����ڱ��8��ͨ&&�Ƿ�Ҷ���pixelzhi
			{
				Insert(head,i,j);
				num++;
				while(head->next!=NULL)
				{
					a=head->next->x;
					b=head->next->y;
					if(b!=0)//������y�����ϱ߽���
					{
						if(flag[(b-1)*wth+a]==0 && *(data+(b-1)*wth+a)==pixel)
						{
							Insert(head,b-1,a);
							flag[(b-1)*wth+a] = num;//���
						}
						if(a!=0)//������x������߽���
						{
							if(flag[(b-1)*wth+a-1]==0 && *(data+(b-1)*wth+a-1)==pixel)
							{
								Insert(head,b-1,a-1);
								flag[(b-1)*wth+a-1] = num;
							}
							if(flag[b*wth+a-1]==0 && *(data+b*wth+a-1)==pixel)
							{
								Insert(head,b,a-1);
								flag[b*wth+a-1] = num;
							}

						}

						if(a!=wth-1)//�ұ߽�
						{
							if(flag[(b-1)*wth+a+1]==0 && *(data+(b-1)*wth+a+1)==pixel)
							{
								Insert(head,b-1,a+1);
								flag[(b-1)*wth+a+1] = num;
							}
							if(flag[b*wth+a+1]==0 && *(data+b*wth+a+1)==pixel)
							{
								Insert(head,b,a+1);
								flag[b*wth+a+1] = num;
							}
						}
						
					}

					if(b!=hth-1)//y������±߽�
					{
						if(flag[(b+1)*wth+a]==0 && *(data+(b+1)*wth+a)==pixel)
						{
							Insert(head,b+1,a);
							flag[(b+1)*wth+a] = num;
						}
						if(a!=0)
						{
							if(flag[(b+1)*wth+a-1]==0 && *(data+(b+1)*wth+a-1)==pixel)
							{
								Insert(head,b+1,a-1);
								flag[(b+1)*wth+a-1] = num;
							}
							if(flag[b*wth+a-1]==0 && *(data+b*wth+a-1)==pixel)
							{
								Insert(head,b,a-1);
								flag[b*wth+a-1] = num;
							}
						}
						
						if(a!=wth-1)
						{
							if(flag[(b+1)*wth+a+1]==0 && *(data+(b+1)*wth+a+1)==pixel)
							{
								Insert(head,b+1,a+1);
								flag[(b+1)*wth+a+1] = num;
							}
							if(flag[b*wth+a+1]==0 && *(data+b*wth+a+1)==pixel)
							{
								Insert(head,b,a+1);
								flag[b*wth+a+1] = num;
							}
						}
						
					}
					Delete(head);
				}
			}
		}
	}
	return num;
}

/*************************************************************************
* �������ƣ�
*   Smooth()
* ����:
*	LPBYTE                   - ͼ������ָ��
*	int                      - ͼ����
*	int                      - ͼ��߶�
*   int                      - ģ���
*	int                      - ģ���
* ����ֵ:
*   ��
* ����:
*   �ú������ھ�ֵ�˲��� 
************************************************************************/
void CBeamAnalysisView::Smooth(LPBYTE lpDib, int lWidth, int lHeight, int nTempWidth, int nTempHeight)
{
	int i, j, x, y;
	int loffset;
	float sum = 0.0;;
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			loffset = i * lWidth + j;
			sum = 0;

			for (y = - nTempHeight / 2; y <= nTempHeight / 2; y++)
			{				
				for (x = - nTempWidth / 2; x <= nTempWidth / 2; x++)
				{
					if ((i + y) >= 0 && (i + y) < lHeight)
					{
						if ((j + x) >= 0 && (j + x) < lWidth)
						{
							sum += *(lpDib + (i + y) * lWidth + (j + x));
						}
					}
				}
			}
			*(lpDib + loffset) = sum / (nTempWidth * nTempHeight);
		}
	}
}

/*************************************************************************
* �������ƣ�
*   CalculateBaselinePara()
* ����:
*	LPBYTE                   - ��������ָ��
*	int                      - ��������
*	float&                   - �����ֵ
*   float&                   - ���������
* ����ֵ:
*   ��
* ����:
*   �ú������ڼ����������ݵľ�ֵ������� 
************************************************************************/
void CBeamAnalysisView::CalculateBaselinePara(LPBYTE lpDib, int n, float& Means, float& StandardDeviation)
{
	int i;
	float sum = 0.0;

	for (i = 0; i < n; i++)
	{
		sum += lpDib[i];	
	}
	Means = sum / n;
	sum = 0.0;
	for (i = 0; i < n; i++)
	{
		sum += (lpDib[i] - Means) * (lpDib[i] - Means);
	}
	StandardDeviation = sqrtf(sum / n);
}

/*************************************************************************
* �������ƣ�
*   SetBkData()
* ����:
*	LPBYTE                   - ͼ������ָ��
*	int                      - ͼ����
*	int                      - ͼ��߶�
*	LPBYTE                   - ͼ�񱳾�����ָ��
* ����ֵ:
*   ��
* ����:
*   �ú������ڴ�ͼ����������ȡ�����������ݡ� 
************************************************************************/
void CBeamAnalysisView::SetBkData(LPBYTE lpDib, int lWidth, int lHeight, LPBYTE lpBKData)
{
	int i, j;
	int loffset;
	int pixel = 0;
	int k = 0;
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			loffset = i * lWidth + j;
			pixel = *(lpDib + loffset);
			if (*(m_lpflag + loffset) == 0)
			{
				lpBKData[k++] = pixel;
			}
		}
	}
}

/*************************************************************************
* �������ƣ�
*   BaseLine()
* ����:
*	��
* ����ֵ:
*   int                     - ����ƫ��ֵ
* ����:
*   �ú������ڼ������ƫ��ֵ�� 
************************************************************************/
int CBeamAnalysisView::BaseLine()
{
	CBeamAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	FIBITMAP* pDib = pDoc->GetDib();
	LPBYTE lpDib = FreeImage_GetBits(pDib);
	int lHeight = FreeImage_GetHeight(pDib);
	int lWidth = FreeImage_GetWidth(pDib);

	float Means = 0.0, StandardDeviation1 = 0.0;
	float Means1 = 0.0, StandardDeviation = 0.0;
	float threshold = 0.0;
	int baselineoffset;

	int N = lWidth * lHeight - m_WidthROI * m_HeightROI;
	LPBYTE lpBKData = (LPBYTE)new char[N];
	memset(lpBKData, 0, N);

	SetBkData(lpDib, lWidth, lHeight, lpBKData);
	CalculateBaselinePara(lpBKData, N, Means, StandardDeviation1);
	
	memset(lpBKData, 0, N);
	SetBkData(m_lpConvolution, lWidth, lHeight, lpBKData);
	CalculateBaselinePara(lpBKData, N, Means1, StandardDeviation);

	threshold = Means + 3.0 * StandardDeviation / sqrtf((lWidth * 3 / 100) * (lHeight * 2 / 100));

	if (lpBKData)
	{
		delete[] lpBKData;
		lpBKData = NULL;
	}
	
	float sum = 0.0;
	int loffset;
	int pixel = 0;
	int k = 0;
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			loffset = i * lWidth + j;
			pixel = *(lpDib + loffset);
			if (pixel <= threshold)
			{
				k++;
				sum += pixel;
			}
		}
	}
	baselineoffset = sum / k;
	return baselineoffset;
}

void CBeamAnalysisView::CalDataThread()
{
	AfxBeginThread(ThreadCalculae, this);
}