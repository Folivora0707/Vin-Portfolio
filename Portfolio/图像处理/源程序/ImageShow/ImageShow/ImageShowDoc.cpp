
// ImageShowDoc.cpp: CImageShowDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageShow.h"
#endif

#include "ImageShowDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageShowDoc

IMPLEMENT_DYNCREATE(CImageShowDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageShowDoc, CDocument)
END_MESSAGE_MAP()


// CImageShowDoc 构造/析构

CImageShowDoc::CImageShowDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	m_hDIB = NULL;
	m_hProcDIB = NULL;
	HT = NULL;
}

CImageShowDoc::~CImageShowDoc()
{
	if (m_hDIB != NULL)GlobalFree(m_hDIB);
	if (m_hProcDIB != NULL)GlobalFree(m_hProcDIB);
	if (HT != NULL)free(HT);
}

BOOL CImageShowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CImageShowDoc 序列化

void CImageShowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CImageShowDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CImageShowDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CImageShowDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageShowDoc 诊断

#ifdef _DEBUG
void CImageShowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageShowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageShowDoc 命令


BOOL CImageShowDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	if (m_hDIB != NULL)
	{
		GlobalFree(m_hDIB);
		m_hDIB = NULL;
	}
	CFile file;
	if (file.Open(lpszPathName, CFile::modeRead, NULL))
	{
		m_hDIB = ReadDIBFile(file);
		m_filePathName = lpszPathName;
		file.Close();
	}
	UpdateAllViews(NULL);

	return TRUE;
}


BOOL CImageShowDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bSuccess = FALSE;
	if (m_hProcDIB != NULL)
	{
		CFile file;
		if (file.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite, NULL))
		{
			bSuccess = SaveDIB(m_hProcDIB, file);
			file.Flush();
			file.Close();
		}
	}
	return bSuccess;
	//return CDocument::OnSaveDocument(lpszPathName);
}
