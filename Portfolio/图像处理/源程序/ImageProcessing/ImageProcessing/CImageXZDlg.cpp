// CImageXZDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "CImageXZDlg.h"
#include "afxdialogex.h"


// CImageXZDlg 对话框

IMPLEMENT_DYNAMIC(CImageXZDlg, CDialog)

CImageXZDlg::CImageXZDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_XZ, pParent)
	, m_xzds(0)
{

}

CImageXZDlg::~CImageXZDlg()
{
}

void CImageXZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_XZ, m_xzds);
	DDV_MinMaxInt(pDX, m_xzds, 0, 360);
}


BEGIN_MESSAGE_MAP(CImageXZDlg, CDialog)
END_MESSAGE_MAP()


// CImageXZDlg 消息处理程序
