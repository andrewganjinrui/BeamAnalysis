// BeamAnalysisDoc.h : CBeamAnalysisDoc ��Ľӿ�
//


#pragma once


class CBeamAnalysisDoc : public CDocument
{
protected: // �������л�����
	CBeamAnalysisDoc();
	DECLARE_DYNCREATE(CBeamAnalysisDoc)

// ����
public:

// ����
public:
	inline FIBITMAP* GetDib(){return m_pDib;}
	inline CSize GetDocDimension() const//��ȡ�ĵ���С
	{ return m_SizeDoc; }
	inline void UpdateDib(FIBITMAP* pDib){FreeDib();m_pDib = pDib;}
	void FreeDib();
	

private:
	FIBITMAP* m_pDib;
	CSize m_SizeDoc;
	
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
// ʵ��
public:
	virtual ~CBeamAnalysisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
};


