// CImageZFTDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "CImageZFTDlg.h"
#include "afxdialogex.h"


// CImageZFTDlg 对话框

IMPLEMENT_DYNAMIC(CImageZFTDlg, CDialogEx)

CImageZFTDlg::CImageZFTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ZFT, pParent)
	, m_blueBZC(_T(""))
	, m_greenBZC(_T(""))
	, m_redBZC(_T(""))
	, m_bluePJHD(_T(""))
	, m_greenPJHD(_T(""))
	, m_redPJHD(_T(""))
	, m_blueXS(_T(""))
	, m_greenXS(_T(""))
	, m_redXS(_T(""))
	, m_blueZZHD(_T(""))
	, m_greenZZHD(_T(""))
	, m_redZZHD(_T(""))
{

}

CImageZFTDlg::~CImageZFTDlg()
{
}

void CImageZFTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_BZC_BLUE, m_blueBZC);
	DDX_Text(pDX, IDC_STATIC_BZC_GREEN, m_greenBZC);
	DDX_Text(pDX, IDC_STATIC_BZC_RED, m_redBZC);
	DDX_Text(pDX, IDC_STATIC_PJHD_BLUE, m_bluePJHD);
	DDX_Text(pDX, IDC_STATIC_PJHD_GREEN, m_greenPJHD);
	DDX_Text(pDX, IDC_STATIC_PJHD_RED, m_redPJHD);
	DDX_Text(pDX, IDC_STATIC_XS_BLUE, m_blueXS);
	DDX_Text(pDX, IDC_STATIC_XS_GREEN, m_greenXS);
	DDX_Text(pDX, IDC_STATIC_XS_RED, m_redXS);
	DDX_Text(pDX, IDC_STATIC_ZZHD_BLUE, m_blueZZHD);
	DDX_Text(pDX, IDC_STATIC_ZZHD_GREEN, m_greenZZHD);
	DDX_Text(pDX, IDC_STATIC_ZZHD_RED, m_redZZHD);
}


BEGIN_MESSAGE_MAP(CImageZFTDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CImageZFTDlg 消息处理程序


void CImageZFTDlg::OnPaint()
{
	CPaintDC dc(this); 

	extern int Red[256], Green[256], Blue[256];

	CRect rectpic;
	GetDlgItem(IDC_STATIC_KJ)->GetWindowRect(&rectpic);

	int x, y;
	x = rectpic.Width();
	y = rectpic.Height();

	CWnd *pWnd = GetDlgItem(IDC_STATIC_KJ);
	CDC *pDC = pWnd->GetDC();

	CPen *RedPen = new CPen();                              
	RedPen->CreatePen(PS_SOLID, 1, RGB(255, 0, 0));           
	CGdiObject *RedOlderPen = pDC->SelectObject(RedPen);    

	pDC->Rectangle(9, 16, 312, 147);      
	pDC->MoveTo(15, 20);               
	pDC->LineTo(15, 128);               
	pDC->LineTo(305, 128);              

	pDC->MoveTo(305, 128);             
	pDC->LineTo(300, 123);             
	pDC->MoveTo(305, 128);
	pDC->LineTo(300, 133);              

	pDC->MoveTo(15, 20);              
	pDC->LineTo(10, 25);               
	pDC->MoveTo(15, 20);
	pDC->LineTo(20, 25);                


	CString str;
	int i;
	for (i = 0; i <= 5; i++)                   
	{
		str.Format("%d", i * 50);              
		pDC->SetTextColor(RGB(255, 0, 255));   
		pDC->TextOut(15 + 48 * i, 130, str);       
		pDC->MoveTo(15 + 48 * i, 128);            
		pDC->LineTo(15 + 48 * i, 125);
	}
	for (i = 0; i <= 5; i++)                        
	{
		pDC->MoveTo(15, 128 - 20 * i);            
		pDC->LineTo(18, 128 - 20 * i);
	}

	for (i = 1; i < 256; i++)
	{
		pDC->MoveTo(15 + i, 128);
		if ((128 - 16) > (Red[i] / 40))
			pDC->LineTo(15 + i, 128 - (Red[i] / 40));
		else
			pDC->LineTo(15 + i, 16);            
	}


	//画直方图 绿色
	CPen *GreenPen = new CPen();                             
	GreenPen->CreatePen(PS_SOLID, 1, RGB(0, 255, 0));          
	CGdiObject *GreenOlderPen = pDC->SelectObject(GreenPen);

	pDC->Rectangle(9, 167, 312, 308);     
	pDC->MoveTo(15, 171);               
	pDC->LineTo(15, 288);               
	pDC->LineTo(305, 288);              

	pDC->MoveTo(305, 288);              
	pDC->LineTo(300, 283);              
	pDC->MoveTo(305, 288);
	pDC->LineTo(300, 293);              

	pDC->MoveTo(15, 171);                
	pDC->LineTo(10, 176);                
	pDC->MoveTo(15, 171);
	pDC->LineTo(20, 176);                

	for (i = 0; i <= 5; i++)                   
	{
		str.Format("%d", i * 50);               
		pDC->SetTextColor(RGB(255, 0, 255));   
		pDC->TextOut(15 + 48 * i, 290, str);       

		pDC->MoveTo(15 + 48 * i, 288);            
		pDC->LineTo(15 + 48 * i, 285);
	}
	for (i = 0; i <= 5; i++)                       
	{
		pDC->MoveTo(15, 288 - 20 * i);           
		pDC->LineTo(18, 288 - 20 * i);
	}

	for (i = 1; i < 256; i++)
	{
		pDC->MoveTo(15 + i, 288);
		if ((288 - 167) > (Green[i] / 40))
			pDC->LineTo(15 + i, 288 - (Green[i] / 40));
		else
			pDC->LineTo(15 + i, 167);
	}


	
	//画直方图 蓝
	CPen *BluePen = new CPen();                            
	BluePen->CreatePen(PS_SOLID, 1, RGB(0, 0, 255));         
	CGdiObject *BlueOlderPen = pDC->SelectObject(BluePen);

	pDC->Rectangle(9, 327, 312, 468);      
	pDC->MoveTo(15, 331);                
	pDC->LineTo(15, 448);                
	pDC->LineTo(305, 448);               

	pDC->MoveTo(305, 448);               
	pDC->LineTo(300, 443);               
	pDC->MoveTo(305, 448);
	pDC->LineTo(300, 453);               

	pDC->MoveTo(15, 331);                
	pDC->LineTo(10, 336);                
	pDC->MoveTo(15, 331);
	pDC->LineTo(20, 336);                


	for (i = 0; i <= 5; i++)                   
	{
		str.Format("%d", i * 50);               
		pDC->SetTextColor(RGB(255, 0, 255));   
		pDC->TextOut(15 + 48 * i, 450, str);       

		pDC->MoveTo(15 + 48 * i, 448);            
		pDC->LineTo(15 + 48 * i, 445);
	}
	for (i = 0; i <= 5; i++)                        
	{
		pDC->MoveTo(15, 448 - 20 * i);            
		pDC->LineTo(18, 448 - 20 * i);
	}

	for (i = 1; i < 256; i++)
	{
		pDC->MoveTo(15 + i, 448);
		if ((448 - 327) > (Blue[i] / 40))
			pDC->LineTo(15 + i, 448 - (Blue[i] / 40));
		else
			pDC->LineTo(15 + i, 327);
	}


	//恢复以前的画笔
	pDC->SelectObject(RedOlderPen);
	pDC->SelectObject(GreenOlderPen);
	pDC->SelectObject(BlueOlderPen);
	delete RedPen;
	delete GreenPen;
	delete BluePen;
	ReleaseDC(pDC);
	return;

}
