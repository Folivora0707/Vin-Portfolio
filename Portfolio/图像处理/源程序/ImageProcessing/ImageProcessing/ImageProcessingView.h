
// ImageProcessingView.h: CImageProcessingView 类的接口
//

#pragma once


class CImageProcessingView : public CView
{
public:
	//添加成员函数
	void ShowBitmap(CDC* pDC, CString BmpName);//显示位图函数
	bool ReadBmp();                            //读取
	bool SaveBmp(LPCSTR lpFileName);           //保存

	//添加成员变量
	CString EntName;   //图像文件扩展名
	CString BmpName;   //图像文件名称
	CBitmap m_bitmap;  //创建位图对象

	CBitmap m_bitmaplin;  //创建临时位图对象进行处理
	CString BmpNameLin;   //保存图像副本文件

	int m_nWidth;
	int m_nHeight;       //实际宽高
	int m_nDrawWidth;
	int m_nDrawHeight;   //显示宽高
	DWORD m_nImage;
	DWORD m_nSize;
	int m_nLineByte;     //图像一行所占字节数
	int m_nBitCount;     //图像每个像素所占位数
	int m_nPalette;      //位图实际使用的颜色表中的颜色数

	BYTE *m_pImage;      //图片指针
	BITMAPFILEHEADER bfh;//文件头
	BITMAPINFOHEADER bih;//信息头
	RGBQUAD m_pPal;      //颜色表指针

protected: // 仅从序列化创建
	CImageProcessingView() noexcept;
	DECLARE_DYNCREATE(CImageProcessingView)

// 特性
public:
	CImageProcessingDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnShowTwo();
	afx_msg void OnShowHd();
	afx_msg void OnZftYt();
	afx_msg void OnDysFxxhs();
	afx_msg void OnDysFxxpf();
	afx_msg void OnDysJhh();
	afx_msg void OnDysXxjx();
	afx_msg void OnDysXxqb();
	afx_msg void OnDysXxyd();
	afx_msg void OnDysXxzq();
	afx_msg void OnDysYzbh();
	afx_msg void OnJhbhPy();
	afx_msg void OnJhbhFz();
	afx_msg void OnJhbhDz();
	afx_msg void OnJhbhTxxz();
	afx_msg void OnTxzqPtph1();
	afx_msg void OnTxzqZzlb();
	afx_msg void OnRhLaplacian();
	afx_msg void OnRhPrewitt();
	afx_msg void OnRhSobel();
	afx_msg void OnAddJy();
};

#ifndef _DEBUG  // ImageProcessingView.cpp 中的调试版本
inline CImageProcessingDoc* CImageProcessingView::GetDocument() const
   { return reinterpret_cast<CImageProcessingDoc*>(m_pDocument); }
#endif

