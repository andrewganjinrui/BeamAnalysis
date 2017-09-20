// BeamAnalysisDoc.h : CBeamAnalysisDoc 类的接口
//


#pragma once


class CBeamAnalysisDoc : public CDocument
{
protected: // 仅从序列化创建
	CBeamAnalysisDoc();
	DECLARE_DYNCREATE(CBeamAnalysisDoc)

// 属性
public:

// 操作
public:
	inline FIBITMAP* GetDib(){return m_pDib;}
	inline CSize GetDocDimension() const//获取文档大小
	{ return m_SizeDoc; }
	inline void UpdateDib(FIBITMAP* pDib){FreeDib();m_pDib = pDib;}
	void FreeDib();
	

private:
	FIBITMAP* m_pDib;
	CSize m_SizeDoc;
	
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
// 实现
public:
	virtual ~CBeamAnalysisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
};


