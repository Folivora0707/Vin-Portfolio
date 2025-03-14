// CImagePYDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "CImagePYDlg.h"
#include "afxdialogex.h"


// CImagePYDlg 对话框

IMPLEMENT_DYNAMIC(CImagePYDlg, CDialog)

CImagePYDlg::CImagePYDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_PY, pParent)
	, m_xPY(0)
	, m_yPY(0)
{

}

CImagePYDlg::~CImagePYDlg()
{
}

void CImagePYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PYX, m_xPY);
	DDX_Text(pDX, IDC_EDIT_PYY, m_yPY);
}


BEGIN_MESSAGE_MAP(CImagePYDlg, CDialog)
END_MESSAGE_MAP()


// CImagePYDlg 消息处理程序
