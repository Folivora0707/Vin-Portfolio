
// ImageShowView.cpp: CImageShowView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageShow.h"
#endif

#include "ImageShowDoc.h"
#include "ImageShowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageShowView

IMPLEMENT_DYNCREATE(CImageShowView, CView)

BEGIN_MESSAGE_MAP(CImageShowView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageShowView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RGBTOGRAY, &CImageShowView::OnRgbToGray)
	ON_COMMAND(ID_VIEW_INTENSITY, &CImageShowView::OnViewIntensity)
END_MESSAGE_MAP()

// CImageShowView 构造/析构

CImageShowView::CImageShowView() noexcept
{
	// TODO: 在此处添加构造代码
	//int nNs_Y[256];
}

CImageShowView::~CImageShowView()
{
}

BOOL CImageShowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImageShowView 绘图

void CImageShowView::OnDraw(CDC* pDC)
{
	CImageShowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//LONG hheight;
	HDIB hDIB = pDoc->m_hDIB;
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR)GlobalLock(hDIB);
		LONG lWidth = DIBWidth(lpDIB);
		LONG lHeight = DIBHeight(lpDIB);
		//hheight = lHeight;
		LPSTR lpDIBBits = FindDIBBits(lpDIB);
		WORD wBitCount = DIBBitCount(lpDIB);
		CRect rectSrc, rectDst;
		rectSrc.top = rectSrc.left = 0;
		rectSrc.right = lWidth;
		rectSrc.bottom = lHeight;
		rectDst = rectSrc;
		PaintDIB(pDC->m_hDC, &rectDst, hDIB, rectSrc, NULL);
		GlobalUnlock(hDIB);
	}
	HDIB hProcDIB = pDoc->m_hProcDIB;
	if (hProcDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR)GlobalLock(hDIB);
		LONG lWidth = DIBWidth(lpDIB);
		LONG lHeight = DIBHeight(lpDIB);
		CRect rectSrc, rectDst;
		rectSrc.top = rectSrc.left = 0;
		rectSrc.right = lWidth;
		rectSrc.bottom = lHeight;
		rectDst = rectSrc;

		rectDst.left = rectDst.left + lWidth;
		rectDst.right = rectDst.right + lWidth;

		PaintDIB(pDC->m_hDC, &rectDst, hProcDIB, rectSrc, NULL);
		GlobalUnlock(hDIB);
	}
	int *Htu = pDoc->HT;
	if (Htu != NULL)
	{
		CPen *RedPen = new CPen();
		RedPen->CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		CGdiObject *RedOlderPen = pDC->SelectObject(RedPen);
		//pDC->MoveTo(0, 1000);
		for (int i = 1; i < 256; i++)
		{
			pDC->MoveTo(i, 800);
			pDC->LineTo(i, 800-(*(Htu + i)/20));
		}
		pDC->SelectObject(RedOlderPen);
		delete RedPen;
		ReleaseDC(pDC);
	}
	//free(Htu);
}


// CImageShowView 打印


void CImageShowView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageShowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageShowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImageShowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImageShowView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageShowView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageShowView 诊断

#ifdef _DEBUG
void CImageShowView::AssertValid() const
{
	CView::AssertValid();
}

void CImageShowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageShowDoc* CImageShowView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageShowDoc)));
	return (CImageShowDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageShowView 消息处理程序


void CImageShowView::OnRgbToGray()
{
	// TODO: 在此添加命令处理程序代码
	LONG i, j;
	LPSTR lpDIB;
	LPSTR lpNewDIB;
	LPSTR lpDIBBits;
	LPSTR lpNewDIBBits;
	DWORD dwWidth, dwHeight;
	LONG lLineBytes;

	CImageShowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->m_hDIB;
	if (hDIB != NULL)
	{
		lpDIB = (LPSTR)::GlobalLock((HGLOBAL)hDIB);
		dwWidth = DIBWidth(lpDIB);
		dwHeight = DIBHeight(lpDIB);
		lLineBytes = WIDTHBYTES(dwWidth * 24);
		HDIB hNewDIB = NULL;
		hNewDIB = (HDIB)CopyHandle(hDIB);
		lpNewDIB = (char*)::GlobalLock((HGLOBAL)hNewDIB);
		lpDIBBits = ::FindDIBBits(lpDIB);
		if (DIBBitCount(lpNewDIB) != 24)
		{
			MessageBox(L"请先将其转换为24位色位图，再进行处理！", L"系统提示", MB_ICONINFORMATION | MB_OK);
			::GlobalUnlock((HGLOBAL)hNewDIB);
			return;
		}
		lpNewDIBBits = ::FindDIBBits(lpNewDIB);
		for (i = 0; i < dwHeight; i++)
		{
			for (j = 0; j < lLineBytes; j++)
			{
				unsigned char B = *((unsigned char*)lpDIBBits + lLineBytes * i + j);
				j++;
				unsigned char G = *((unsigned char*)lpDIBBits + lLineBytes * i + j);
				j++;
				unsigned char R = *((unsigned char*)lpDIBBits + lLineBytes * i + j);

				unsigned char Y = (unsigned char)(0.299*R + 0.587*G + 0.114*B);

				*((unsigned char*)lpNewDIBBits + lLineBytes * i + j - 2) = Y;
				*((unsigned char*)lpNewDIBBits + lLineBytes * i + j - 1) = Y;
				*((unsigned char*)lpNewDIBBits + lLineBytes * i + j) = Y;
			}
		}
		GlobalUnlock(hDIB);
		GlobalUnlock(hNewDIB);
		if (pDoc->m_hProcDIB != NULL)
		{
			GlobalFree(pDoc->m_hProcDIB);
		}
		pDoc->m_hProcDIB = hNewDIB;
		pDoc->UpdateAllViews(NULL);
	}
}


void CImageShowView::OnViewIntensity()
{
	// TODO: 在此添加命令处理程序代码
	CImageShowDoc* pDoc = GetDocument();
	HDIB hDIB = pDoc->m_hProcDIB;
	if (hDIB != NULL)
	{
		LONG i;
		LONG j;
	    int nNs_Y[256];
		float fPs_Y[256];
		memset(nNs_Y, 0, sizeof(nNs_Y));
		LPSTR lpDIB;
		LPSTR lpDIBBits;
		lpDIB = (LPSTR)GlobalLock((HGLOBAL)hDIB);
		lpDIBBits = FindDIBBits(lpDIB);
		if (DIBBitCount(lpDIB) != 24)
		{
			MessageBox(L"请先将其转换为24位色位图，再进行处理！", L"系统提示", MB_ICONINFORMATION | MB_OK);
			::GlobalUnlock((HGLOBAL)hDIB);
			return;
		}
		LONG lWidth = DIBWidth(lpDIB);
		LONG lHeight = DIBHeight(lpDIB);
		LONG lLineBytes = WIDTHBYTES(lWidth * 24);
		for (i = 0; i < lHeight; i++)
		{
			for (j = 0; j < lLineBytes; j++)
			{
				unsigned char B = *((unsigned char*)lpDIBBits + lLineBytes * i + j);
				j++;
				unsigned char G = *((unsigned char*)lpDIBBits + lLineBytes * i + j);
				j++;
				unsigned char R = *((unsigned char*)lpDIBBits + lLineBytes * i + j);
				unsigned char Y = (unsigned char)(0.3*R + 0.59*G + 0.11*B);
				nNs_Y[Y]++;
			}
		}
		//pDoc->HT = new int[256];
		pDoc->HT = new int[256];
		memset(pDoc->HT, 0, sizeof(pDoc->HT));
		for (i = 0; i < 256; i++)
		{
			(pDoc->HT)[i] = nNs_Y[i];
		}
		for (i = 0; i < 256; i++)
			fPs_Y[i] = nNs_Y[i] / (lHeight*lWidth*1.0f);
		::GlobalUnlock((HGLOBAL)hDIB);

		pDoc->UpdateAllViews(NULL);
	}
}
