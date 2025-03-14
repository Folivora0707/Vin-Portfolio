
// ImageShowDoc.h: CImageShowDoc 类的接口
//


#pragma once
#include "dibapi.h"       //将有关DIB操作的代码头文件包含进来


class CImageShowDoc : public CDocument
{
protected: // 仅从序列化创建
	CImageShowDoc() noexcept;
	DECLARE_DYNCREATE(CImageShowDoc)

// 特性
public:
	HDIB m_hDIB;           //HDIB在DIBAPI.h中声明，定义该变量用来保存打开的图
	HDIB m_hProcDIB;       //处理后的图像
	CString m_filePathName;//保存打开文件路径
	int *HT;               //直方图
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CImageShowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
