
// ImageProcessingView.cpp: CImageProcessingView 类的实现
//

#include "stdafx.h"
#include "CImageZFTDlg.h"
#include "math.h"
#include <time.h>

#include "CImagePYDlg.h"
#include "CImageXZDlg.h"

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageProcessing.h"
#endif

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CImageProcessingView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CImageProcessingView::OnFileSave)
	ON_COMMAND(ID_SHOW_TWO, &CImageProcessingView::OnShowTwo)
	ON_COMMAND(ID_SHOW_HD, &CImageProcessingView::OnShowHd)
	ON_COMMAND(ID_ZFT_YT, &CImageProcessingView::OnZftYt)
	ON_COMMAND(ID_DYS_FXXHS, &CImageProcessingView::OnDysFxxhs)
	ON_COMMAND(ID_DYS_FXXPF, &CImageProcessingView::OnDysFxxpf)
	ON_COMMAND(ID_DYS_JHH, &CImageProcessingView::OnDysJhh)
	ON_COMMAND(ID_DYS_XXJX, &CImageProcessingView::OnDysXxjx)
	ON_COMMAND(ID_DYS_XXQB, &CImageProcessingView::OnDysXxqb)
	ON_COMMAND(ID_DYS_XXYD, &CImageProcessingView::OnDysXxyd)
	ON_COMMAND(ID_DYS_XXZQ, &CImageProcessingView::OnDysXxzq)
	ON_COMMAND(ID_DYS_YZBH, &CImageProcessingView::OnDysYzbh)
	ON_COMMAND(ID_JHBH_PY, &CImageProcessingView::OnJhbhPy)
	ON_COMMAND(ID_JHBH_FZ, &CImageProcessingView::OnJhbhFz)
	ON_COMMAND(ID_JHBH_DZ, &CImageProcessingView::OnJhbhDz)
	ON_COMMAND(ID_JHBH_TXXZ, &CImageProcessingView::OnJhbhTxxz)
	ON_COMMAND(ID_TXZQ_PTPH1, &CImageProcessingView::OnTxzqPtph1)
	ON_COMMAND(ID_TXZQ_ZZLB, &CImageProcessingView::OnTxzqZzlb)
	ON_COMMAND(ID_RH_Laplacian, &CImageProcessingView::OnRhLaplacian)
	ON_COMMAND(ID_RH_Prewitt, &CImageProcessingView::OnRhPrewitt)
	ON_COMMAND(ID_RH_Sobel, &CImageProcessingView::OnRhSobel)
	ON_COMMAND(ID_ADD_JY, &CImageProcessingView::OnAddJy)
END_MESSAGE_MAP()

// CImageProcessingView 构造/析构

CImageProcessingView::CImageProcessingView() noexcept
{
	// TODO: 在此处添加构造代码

}

CImageProcessingView::~CImageProcessingView()
{
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImageProcessingView 绘图

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (EntName.Compare(_T("bmp")) == 0)
	{
		ReadBmp();
		ShowBitmap(pDC, BmpName);
	}
}


// CImageProcessingView 打印


void CImageProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImageProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessingView 诊断

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

int numPicture = 0;
int level = 0;

// CImageProcessingView 消息处理程序
void CImageProcessingView::ShowBitmap(CDC *pDC, CString BmpName)
{
	HBITMAP m_hBitmap;

	m_hBitmap = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();
	}
	m_bitmap.Attach(m_hBitmap);

	CRect rect;
	GetClientRect(&rect);

	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;
	int m_nWindowHeight = rect.bottom - rect.top;

	CDC dcBmp;
	if (!dcBmp.CreateCompatibleDC(pDC))
		return;
	BITMAP m_bmp;
	m_bitmap.GetBitmap(&m_bmp);
	CBitmap *pbmpOld = NULL;
	dcBmp.SelectObject(&m_bitmap);

	pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);

	dcBmp.SelectObject(pbmpOld);
	DeleteObject(&m_bitmap);
	dcBmp.DeleteDC();

	if (numPicture == 2)
	{
		HBITMAP m_hBitmapChange;
		if (level == 0)
		{
			m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		}
		else if (level == 1)
		{
			m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpNameLin, IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		}
		else if (level = 101)
		{
			m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpNameLin, IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		}
		else if (level = 200)
		{
			m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpNameLin, IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		}
		else if (level = 400)
		{
			m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpNameLin, IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		}

		if (m_bitmap.m_hObject) {
			m_bitmap.Detach();
		}
		m_bitmap.Attach(m_hBitmapChange);

		CDC dcBmp;
		if (!dcBmp.CreateCompatibleDC(pDC))
			return;
		BITMAP m_bmp;
		m_bitmap.GetBitmap(&m_bmp);
		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);

		if (m_nDrawWidth < 650 && m_nDrawHeight < 650)
			pDC->StretchBlt(m_nWindowWidth - m_nDrawWidth, 0,
				m_nDrawWidth, m_nDrawHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
		else
			pDC->StretchBlt(m_nWindowWidth - 640, 0, 640, 640, &dcBmp, 0, 0,
				m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
		dcBmp.SelectObject(pbmpOld);
	}
}

void CImageProcessingView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString filter;
	filter = "所有文件(*.bmp,*.jpg,*.gif)|*.bmp;*.jpg| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();

		BmpNameLin = "picture.bmp";
		numPicture = 1;

		EntName = dlg.GetFileExt();
		EntName.MakeLower();
		Invalidate();
	}
}

bool CImageProcessingView::ReadBmp()
{
	FILE *fp = fopen(BmpName, "rb");
	if (fp == 0) {
		AfxMessageBox("无法打开文件！", MB_OK, 0);
		return 0;
	}

	fread(&bfh.bfType, sizeof(WORD), 1, fp);
	fread(&bfh.bfSize, sizeof(DWORD), 1, fp);
	fread(&bfh.bfReserved1, sizeof(WORD), 1, fp);
	fread(&bfh.bfReserved2, sizeof(WORD), 1, fp);
	fread(&bfh.bfOffBits, sizeof(DWORD), 1, fp);

	m_nSize = bfh.bfSize;

	if (bfh.bfType != 0x4d42) {
		AfxMessageBox("不是BMP格式图片！", MB_OK, 0);
		return 0;
	}

	fread(&bih.biSize, sizeof(DWORD), 1, fp);
	fread(&bih.biWidth, sizeof(LONG), 1, fp);
	fread(&bih.biHeight, sizeof(LONG), 1, fp);
	fread(&bih.biPlanes, sizeof(WORD), 1, fp);
	fread(&bih.biBitCount, sizeof(WORD), 1, fp);
	fread(&bih.biCompression, sizeof(DWORD), 1, fp);
	fread(&bih.biSizeImage, sizeof(DWORD), 1, fp);
	fread(&bih.biXPelsPerMeter, sizeof(LONG), 1, fp);
	fread(&bih.biYPelsPerMeter, sizeof(LONG), 1, fp);
	fread(&bih.biClrUsed, sizeof(DWORD), 1, fp);
	fread(&bih.biClrImportant, sizeof(DWORD), 1, fp);
	if (bih.biSize != sizeof(bih)) {
		AfxMessageBox("本结构所占用字节数出现错误");
		return 0;
	}

	if (bih.biCompression == BI_RLE8 || bih.biCompression == BI_RLE4)
	{
		AfxMessageBox("位图被压缩！");
		return 0;
	}

	m_nHeight = bih.biHeight;
	m_nWidth = bih.biWidth;
	m_nDrawHeight = bih.biHeight;
	m_nDrawWidth = bih.biWidth;
	m_nBitCount = bih.biBitCount;

	m_nLineByte = (m_nWidth*m_nBitCount + 31) / 32 * 4;

	m_nImage = m_nLineByte * m_nHeight;

	m_nPalette = 0;
	if (bih.biClrUsed)
		m_nPalette = bih.biClrUsed;

	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fp);
	fclose(fp);
	return true;
}

bool CImageProcessingView::SaveBmp(LPCSTR lpFileName)
{
	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(lpFileName, "wb");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);

	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fpo);
	fwrite(m_pImage, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	return true;
}


void CImageProcessingView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	CString filter;
	filter = "所有文件(*.bmp,*.jpg)|*.bmp;*.jpg| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg||";
	CFileDialog dlg(0, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString str;
		CString strName;
		CString filename;
		str = dlg.GetPathName();
		filename = dlg.GetPathName();
		int nFilterIndex = dlg.m_ofn.nFilterIndex;
		if (nFilterIndex == 2)
		{
			str = str + ".bmp";
			SaveBmp(str);
			AfxMessageBox("图片保存成功", MB_OK, 0);
		}
	}
}


void CImageProcessingView::OnShowTwo()
{
	// TODO: 在此添加命令处理程序代码
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能显示2张图片！");
		return;
	}
	AfxMessageBox("显示两张图片！", MB_OK, 0);
	numPicture = 2;
	level = 0;
	Invalidate();
}


void CImageProcessingView::OnShowHd()
{
	// TODO: 在此添加命令处理程序代码
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能灰度图片！", MB_OK, 0);
		return;
	}
	AfxMessageBox("灰度图像！", MB_OK, 0);

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");

	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);

	unsigned char color;
	unsigned char red, green, blue;

	for (int i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		color = (red + green + blue) / 3;
		red = color;
		green = color;
		blue = color;

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 1;
	Invalidate();
}


int Red[256], Green[256], Blue[256];

void CImageProcessingView::OnZftYt()
{
	// TODO: 在此添加命令处理程序代码
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能显示原图直方图！", MB_OK, 0);
		return;
	}
	AfxMessageBox("显示原图直方图！", MB_OK, 0);

	CImageZFTDlg dlg;

	FILE *fpo = fopen(BmpName, "rb");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);

	int i, j;
	for (j = 0; j < 256; j++)
	{
		Red[j] = 0;
		Green[j] = 0;
		Blue[j] = 0;
	}

	unsigned char red, green, blue;
	int IntRed, IntGreen, IntBlue;
	double sumRedHD = 0, sumGreenHD = 0, sumBlueHD = 0;
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		IntRed = int(red);
		sumRedHD = sumRedHD + IntRed;
		if (IntRed >= 0 && IntRed < 256) Red[IntRed]++;

		fread(&green, sizeof(char), 1, fpo);
		IntGreen = int(green);
		sumGreenHD = sumGreenHD + IntGreen;
		if (IntGreen >= 0 && IntGreen < 256) Green[IntGreen]++;

		fread(&blue, sizeof(char), 1, fpo);
		IntBlue = int(blue);
		sumBlueHD = sumBlueHD + IntBlue;
		if (IntBlue >= 0 && IntBlue < 256) Blue[IntBlue]++;
	}
	fclose(fpo);

	dlg.m_redXS.Format("%d", m_nImage);
	dlg.m_greenXS.Format("%d", m_nImage);
	dlg.m_blueXS.Format("%d", m_nImage);
	
	//平均灰度 = 总灰度/总像素
	float pinRedHD, pinGreenHD, pinBlueHD;
	pinRedHD = sumRedHD * 3 / m_nImage;
	pinGreenHD = sumGreenHD * 3 / m_nImage;
	pinBlueHD = sumBlueHD * 3 / m_nImage;

	dlg.m_redPJHD.Format("%.2f", pinRedHD);
	dlg.m_greenPJHD.Format("%.2f", pinGreenHD);
	dlg.m_bluePJHD.Format("%.2f", pinBlueHD);

	//中值灰度
	int sumRedZZHD = 0, sumGreenZZHD = 0, sumBlueZZHD = 0;
	int redZZHD = 0, greenZZHD = 0, blueZZHD = 0;
	for (i = 0; i < 256; i++)
	{
		sumRedZZHD = sumRedZZHD + Red[i];
		if (sumRedZZHD >= m_nImage / 6)
		{
			redZZHD = i;
			break;
		}
	}
	for (i = 0; i < 256; i++)
	{
		sumGreenZZHD = sumGreenZZHD + Green[i];
		if (sumGreenZZHD >= m_nImage / 6)
		{
			greenZZHD = i;
			break;
		}
	}
	for (i = 0; i < 256; i++)
	{
		sumBlueZZHD = sumBlueZZHD + Blue[i];
		if (sumBlueZZHD >= m_nImage / 6)
		{
			blueZZHD = i;
			break;
		}
	}

	dlg.m_redZZHD.Format("%d", redZZHD);
	dlg.m_greenZZHD.Format("%d", greenZZHD);
	dlg.m_blueZZHD.Format("%d", blueZZHD);

	//标准差

	float redBZC, greenBZC, blueBZC;       //标准差
	double redFC = 0, greenFC = 0, blueFC = 0;    //方差
	for (i = 0; i < 256; i++)
	{
		redFC = redFC + (pinRedHD - i)*(pinRedHD - i)*Red[i];
		greenFC = greenFC + (pinGreenHD - i)*(pinGreenHD - i)*Green[i];
		blueFC = blueFC + (pinBlueHD - i)*(pinBlueHD - i)*Blue[i];
	}

	redBZC = sqrt(redFC * 3 / m_nImage);
	greenBZC = sqrt(greenFC * 3 / m_nImage);
	blueBZC = sqrt(blueFC * 3 / m_nImage);

	dlg.m_redBZC.Format("%.2lf", redBZC);
	dlg.m_greenBZC.Format("%.2lf", greenBZC);
	dlg.m_blueBZC.Format("%.2lf", blueBZC);

	//弹出对话框
	if (dlg.DoModal() == IDOK)
	{

	}
}

//非线性函数
void CImageProcessingView::OnDysFxxhs()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能非线性灰度处理!", MB_OK, 0);
		return;
	}
	AfxMessageBox("线性灰度直方图-灰度变化增强 D(B)=D(A)+0.8*D(A)*(255-D(A))/255!", MB_OK, 0);
	int i;

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);

	unsigned char color;
	unsigned char red, green, blue;
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		if (((int)red + 0.8*(int)red*(255 - (int)red) / 255) > 255)
			red = 255;
		else
			red = (int)red + 0.8*(int)red*(255 - (int)red) / 255;

		if (((int)green + 0.8*(int)green*(255 - (int)green) / 255) > 255)
			green = 255;
		else
			green = (int)green + 0.8*(int)green*(255 - (int)green) / 255;

		if (((int)blue + 0.8*(int)blue*(255 - (int)blue) / 255) > 255)
			blue = 255;
		else
			blue = (int)blue + 0.8*(int)blue*(255 - (int)blue) / 255;

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 101;
	Invalidate();
}

//非线性平方
void CImageProcessingView::OnDysFxxpf()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能非线性灰度处理!", MB_OK, 0);
		return;
	}
	AfxMessageBox("非线性灰度变化 D(B)=D(A)*D(A)/255!", MB_OK, 0);
	int i;
	
	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	
	unsigned char color;
	unsigned char red, green, blue;
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		red = (int)red*(int)red / 255;
		green = (int)green*(int)green / 255;
		blue = (int)blue*(int)blue / 255;

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 101;
	Invalidate();
}

//灰度均衡化
void CImageProcessingView::OnDysJhh()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能图像均衡化!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图像均衡化!", MB_OK, 0);

	FILE *fpo = fopen(BmpName, "rb");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);

	int i, j, k;
	for (j = 0; j < 256; j++) { 
		Red[j] = 0;
		Green[j] = 0;
		Blue[j] = 0;
	}

	unsigned char red, green, blue;
	int IntRed, IntGreen, IntBlue;                  
	double sumRedHD = 0, sumGreenHD = 0, sumBlueHD = 0;  
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		IntRed = int(red);
		sumRedHD = sumRedHD + IntRed;
		if (IntRed >= 0 && IntRed < 256) Red[IntRed]++;

		fread(&green, sizeof(char), 1, fpo);
		IntGreen = int(green);
		sumGreenHD = sumGreenHD + IntGreen;
		if (IntGreen >= 0 && IntGreen < 256) Green[IntGreen]++;

		fread(&blue, sizeof(char), 1, fpo);
		IntBlue = int(blue);
		sumBlueHD = sumBlueHD + IntBlue;
		if (IntBlue >= 0 && IntBlue < 256) Blue[IntBlue]++;
	}
	fclose(fpo);

	//均衡化处理
	float CountRed[256], CountGreen[256], CountBlue[256];
	float CountRedLin[256], CountGreenLin[256], CountBlueLin[256];

	for (k = 0; k < 256; k++)
	{
		CountRed[k] = (float)(Red[k]) * 3 / m_nImage;
		CountRedLin[k] = CountRed[k];
		CountGreen[k] = (float)(Green[k]) * 3 / m_nImage;
		CountGreenLin[k] = CountGreen[k];
		CountBlue[k] = (float)(Blue[k]) * 3 / m_nImage;
		CountBlueLin[k] = CountBlue[k];
	}

	for (k = 1; k < 256; k++)
	{
		CountRed[k] = CountRed[k] + CountRed[k - 1];
		CountGreen[k] = CountGreen[k] + CountGreen[k - 1];
		CountBlue[k] = CountBlue[k] + CountBlue[k - 1];
	}

	int LRed[256], LGreen[256], LBlue[256];  
	for (k = 0; k < 256; k++)
	{
		LRed[k] = (int)(CountRed[k] * (256 - 1) + 0.5);
		LGreen[k] = (int)(CountGreen[k] * (256 - 1) + 0.5);
		LBlue[k] = (int)(CountBlue[k] * (256 - 1) + 0.5);
	}

	//写入新图像
	fpo = fopen(BmpName, "rb");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);

	FILE *fpw = fopen(BmpNameLin, "wb+");
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);

	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		red = LRed[int(red)];
		green = LGreen[int(green)];
		blue = LBlue[int(blue)];

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpw);
	numPicture = 2;
	level = 101;
	Invalidate();
}

//线性减小
void CImageProcessingView::OnDysXxjx()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能线性灰度处理!", MB_OK, 0);
		return;
	}
	AfxMessageBox("线性灰度直方图-灰度减小 D(B)=0.8*D(A)!", MB_OK, 0);
	int i;

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);

	unsigned char color;
	unsigned char red, green, blue;
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		red = (int)red*0.8;
		green = (int)green*0.8;
		blue = (int)blue*0.8;

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 101;
	Invalidate();
}

//线性求补
void CImageProcessingView::OnDysXxqb()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能线性灰度处理!", MB_OK, 0);
		return;
	}
	AfxMessageBox("线性灰度直方图-灰度求补 D(B)=-1*D(A)+255!", MB_OK, 0);
	int i;
	
	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	
	unsigned char color;
	unsigned char red, green, blue;
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		red = (int)red*(-1) + 255;
		green = (int)green*(-1) + 255;
		blue = (int)blue*(-1) + 255;

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 101;
	Invalidate(); 
}

//线性移动
void CImageProcessingView::OnDysXxyd()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能线性灰度运算!", MB_OK, 0);
		return;
	}
	AfxMessageBox("线性灰度直方图-灰度变化移动 D(B)=D(A)+50!", MB_OK, 0);
	int i;

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");

	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);

	unsigned char color;
	unsigned char red, green, blue;
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		if ((int)red + 50 > 255)
			red = 255;
		else
			red = (int)red + 50;

		if ((int)green + 50 > 255)
			green = 255;
		else
			green = (int)green + 50;

		if ((int)blue + 50 > 255)
			blue = 255;
		else
			blue = (int)blue + 50;

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 101;      
	Invalidate();
}

//线性增强
void CImageProcessingView::OnDysXxzq()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能线性灰度运算!", MB_OK, 0);
		return;
	}
	AfxMessageBox("线性灰度直方图-灰度变化增强 D(B)=1.5*D(A)!", MB_OK, 0);
	int i;

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");

	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);

	unsigned char color;
	unsigned char red, green, blue;
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		if ((int)red*1.5 > 255)
			red = 255;
		else
			red = (int)red*1.5;

		if ((int)green*1.5 > 255)
			green = 255;
		else
			green = (int)green*1.5;

		if ((int)blue*1.5 > 255)
			blue = 255;
		else
			blue = (int)blue*1.5;

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 101;
	Invalidate();
}

//阈值变换
void CImageProcessingView::OnDysYzbh()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能点运算阈值化处理!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图像点运算阈值化处理!", MB_OK, 0);
	
	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	
	unsigned char color;
	unsigned char red, green, blue;
	for (int i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		if ((int)red > 128)
			red = 255;
		else
			red = 0;

		if ((int)green > 128)
			green = 255;
		else
			green = 0;

		if ((int)blue > 128)
			blue = 255;
		else
			blue = 0;

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 101;
	Invalidate();
}

//平移
void CImageProcessingView::OnJhbhPy()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能空间平移!", MB_OK, 0);
		return;
	}

	CImagePYDlg dlg;
	if (dlg.DoModal() == IDOK) 
	{
		
		if (dlg.m_xPY > m_nWidth || dlg.m_yPY > m_nHeight) {
			AfxMessageBox("图片平移不能为超过原图长宽!", MB_OK, 0);
			return;
		}
		AfxMessageBox("图片空间变换-平移!", MB_OK, 0);

		
		FILE *fpo = fopen(BmpName, "rb");
		FILE *fpw = fopen(BmpNameLin, "wb+");
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
		fread(m_pImage, m_nImage, 1, fpo);

		unsigned char *ImageSize;
		ImageSize = new unsigned char[m_nImage]; 

		int Place;                    
		int m_pImagePlace;            
		unsigned char black;          

		Place = dlg.m_yPY*m_nWidth * 3;            
		black = 0;                       
		m_pImagePlace = 0;                
		int countWidth = 0;              
		int number = 0;                 

		for (int i = 0; i < m_nImage; i++)
		{
			if (countWidth == m_nWidth * 3) {
				countWidth = 0;
			}

			if (i < Place) {
				ImageSize[i] = black;     
				continue;
			}

			else if (i >= Place && countWidth < dlg.m_xPY * 3) { 
				ImageSize[i] = black;     
				countWidth++;
				continue;
			}

			else if (i >= Place && countWidth >= dlg.m_xPY * 3)
			{
				ImageSize[i] = m_pImage[m_pImagePlace];
				m_pImagePlace++;
				countWidth++;
				if (countWidth == m_nWidth * 3)
				{
					number++;
					m_pImagePlace = number * m_nWidth * 3;
				}
			}
		}

		fwrite(ImageSize, m_nImage, 1, fpw);
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level = 200;        
		Invalidate();
	}
}


void CImageProcessingView::OnJhbhFz()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能空间反转!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图片空间变换-反转图像!", MB_OK, 0);

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	fread(m_pImage, m_nImage, 1, fpo);

	unsigned char *ImageSize;
	ImageSize = new unsigned char[m_nImage];
	int countWidth = 0;//列数        
	int Place;               
	int number = 0;//行数            
	Place = m_nWidth * 3 - 1;

	for (int i = 0; i < m_nImage; i++)
	{
		if (countWidth == m_nWidth * 3)
		{
			countWidth = 0;
		}
		ImageSize[i] = m_pImage[Place]; 
		Place--;
		countWidth++;
		if (countWidth == m_nWidth * 3)
		{
			number++;
			Place = number * m_nWidth * 3 - 1;
		}
	}

	fwrite(ImageSize, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 200;
	Invalidate();
}

//倒转
void CImageProcessingView::OnJhbhDz()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能空间反转!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图片空间变换-反转图像!", MB_OK, 0);

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	fread(m_pImage, m_nImage, 1, fpo);

	unsigned char *ImageSize;
	ImageSize = new unsigned char[m_nImage];
	int countWidth = 0;                  
	int Place;                         
	int number = 0;                      
	Place = (m_nWidth * 3)*(m_nHeight - 1 - 1); 

	for (int i = 0; i < m_nImage; i++)
	{
		ImageSize[i] = m_pImage[Place]; 
		Place++;
		countWidth++;
		if (countWidth == m_nWidth * 3)
		{
			countWidth = 0;
			number++;
			Place = (m_nWidth * 3)*(m_nHeight - number - 1);
		}
	}

	fwrite(ImageSize, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 200;
	Invalidate();
}


void CImageProcessingView::OnJhbhTxxz()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能空间旋转!", MB_OK, 0);
		return;
	}

	CImageXZDlg dlg;
	if (dlg.DoModal() == IDOK) //显示对话框
	{
		AfxMessageBox("图片空间变换-旋转图像!", MB_OK, 0);

		FILE *fpo = fopen(BmpName, "rb");
		FILE *fpw = fopen(BmpNameLin, "wb+");
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
		fread(m_pImage, m_nImage, 1, fpo);

		unsigned char *ImageSize;
		ImageSize = new unsigned char[m_nImage];
		int Place;         

		double PA;
		PA = asin(0.5) * 6;

		double degree;
		degree = PA * dlg.m_xzds / 180; 

		int X, Y;            
		int XPlace, YPlace;

		CString str;
		str.Format("转换后的角度=%f", degree);
		AfxMessageBox(str);

		for (int i = 0; i < m_nImage; i++)
		{
			X = (i / 3) % m_nWidth;
			Y = (i / 3) / m_nWidth;

			XPlace = (int)(X*cos(degree) - Y * sin(degree));
			YPlace = (int)(X*sin(degree) + Y * cos(degree));

			if ((XPlace >= 0 && XPlace <= m_nWidth) && (YPlace >= 0 && YPlace <= m_nHeight))
			{
				Place = YPlace * m_nWidth * 3 + XPlace * 3;
				if (Place + 2 < m_nImage)
				{
					ImageSize[i] = m_pImage[Place];
					i++;
					ImageSize[i] = m_pImage[Place + 1];
					i++;
					ImageSize[i] = m_pImage[Place + 2];
				}
				else
				{
					ImageSize[i] = 255;
					i++;
					ImageSize[i] = 255;
					i++;
					ImageSize[i] = 255;
				}
			}
			else
			{
				ImageSize[i] = 255;
				i++;
				ImageSize[i] = 255;
				i++;
				ImageSize[i] = 255;
			}
		}

		fwrite(ImageSize, m_nImage, 1, fpw);
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level = 200;            
		Invalidate();
	}
}

//均值滤波
void CImageProcessingView::OnTxzqPtph1()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能图像增强(平滑)!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图像增强(平滑)!选取的模板为:均值滤波", MB_OK, 0);

	int HWS = 3;                                
	float H1[3][3] = { {1.0 / 9,1.0 / 9,1.0 / 9},   
					{1.0 / 9,1.0 / 9,1.0 / 9},
					{1.0 / 9,1.0 / 9,1.0 / 9} };

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	fread(m_pImage, m_nImage, 1, fpo);

	unsigned char *ImageSize;
	ImageSize = new unsigned char[m_nImage];
	float red, green, blue;
	int X, Y;               
	int TR, TG, TB;         

	for (int i = 0; i < m_nImage; i = i + 3)
	{
		X = (i / 3) % m_nWidth;    
		Y = (i / 3) / m_nWidth;    

		red = green = blue = 0;

		for (int j = Y - HWS / 2; j < Y + HWS / 2 + 1; j++)                     
		{
			for (int k = X - HWS / 2; k < X + HWS / 2 + 1; k++)                  
			{
				if (j >= 0 && k >= 0 && k < m_nWidth && j < m_nHeight)      
				{
					TR = j * m_nWidth * 3 + k * 3;
					red += H1[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (float)(m_pImage[TR]);
					TG = j * m_nWidth * 3 + k * 3 + 1;
					green += H1[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (float)(m_pImage[TG]);
					TB = j * m_nWidth * 3 + k * 3 + 2;
					blue += H1[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (float)(m_pImage[TB]);
				}
			}
		}

		ImageSize[i] = (unsigned char)(red);
		ImageSize[i + 1] = (unsigned char)(green);
		ImageSize[i + 2] = (unsigned char)(blue);
	}

	fwrite(ImageSize, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 400;
	Invalidate();
}


void CImageProcessingView::OnTxzqZzlb()
{
	if (numPicture == 0) {
		AfxMessageBox("载入图片后才能图像增强(平滑)!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图像增强(平滑)!选取的模板为:中值滤波", MB_OK, 0);

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	fread(m_pImage, m_nImage, 1, fpo);

	unsigned char *ImageSize;
	ImageSize = new unsigned char[m_nImage];
	int X, Y;               
	int TR, TG, TB;        

	int H[9] = { 0,0,0,0,0,0,0,0,0 };
	int HWS = 3;            

	for (int i = 0; i < m_nImage; i = i + 3)
	{
		X = (i / 3) % m_nWidth;    
		Y = (i / 3) / m_nWidth;   

		//边缘使用原值
		if (X == 0 || Y == 0 || X == m_nWidth * 3 || Y == m_nHeight)
		{
			if (i + 2 > m_nImage) break;
			ImageSize[i] = m_pImage[i];
			ImageSize[i + 1] = m_pImage[i + 1];
			ImageSize[i + 2] = m_pImage[i + 2];
			continue;
		}

		//将九个数放进数组
		int num = 0;
		for (int j = Y - HWS / 2; j < Y + HWS / 2 + 1; j++)                      
		{
			for (int k = X - HWS / 2; k < X + HWS / 2 + 1; k++)                  
			{
				if (j >= 0 && k >= 0 && k < m_nWidth && j < m_nHeight)      
				{
					TR = j * m_nWidth * 3 + k * 3;
					H[num] = m_pImage[TR];
					num++;
				}
			}
		}

		//获取中位数
		int temp = 0;
		for (int x = 0; x < 9; x++)
		{
			for (int y = x; y < 9; y++)
			{
				if (H[x] >= H[y])
				{
					temp = H[x];
					H[x] = H[y];
					H[y] = temp;
				}
			}
		}

		//灰度图像RGB相同
		ImageSize[i] = H[4];
		ImageSize[i + 1] = H[4];
		ImageSize[i + 2] = H[4];
	}

	fwrite(ImageSize, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 400;
	Invalidate();
}


void CImageProcessingView::OnRhLaplacian()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能图像增强(锐化)!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图像增强(锐化):采用拉普拉斯(Laplacian)算子!");

	int HWS = 3;
	int H[3][3] = { {0,-1,0}, 
				{-1,4,-1},
				{0,-1,0} };

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	fread(m_pImage, m_nImage, 1, fpo);

	unsigned char *ImageSize;
	ImageSize = new unsigned char[m_nImage];
	int red, green, blue;
	int X, Y;       
	int TR, TG, TB;  

	for (int i = 0; i < m_nImage; i = i + 3)
	{
		X = (i / 3) % m_nWidth;   
		Y = (i / 3) / m_nWidth;   
		red = green = blue = 0;

		for (int j = Y - HWS / 2; j < Y + HWS / 2 + 1; j++)                      
		{
			for (int k = X - HWS / 2; k < X + HWS / 2 + 1; k++)                 
			{
				if (j >= 0 && k >= 0 && k < m_nWidth && j < m_nHeight)
				{

					TR = j * m_nWidth * 3 + k * 3;
					red += H[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TR]);
					TG = j * m_nWidth * 3 + k * 3 + 1;
					green += H[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TG]);
					TB = j * m_nWidth * 3 + k * 3 + 2;
					blue += H[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TB]);
				}
			}
		}

		if (red >= 0 && red < 256) ImageSize[i] = red;
		else if (red < 0) ImageSize[i] = 0;      
		else ImageSize[i] = 0;

		if (green >= 0 && green < 256) ImageSize[i + 1] = green;
		else if (green < 0) ImageSize[i + 1] = 0;  
		else ImageSize[i + 1] = 0;

		if (blue >= 0 && blue < 256) ImageSize[i + 2] = blue;
		else if (blue < 0) ImageSize[i + 2] = 0;   
		else ImageSize[i + 2] = 0;
	}

	fwrite(ImageSize, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 400;
	Invalidate();
}


void CImageProcessingView::OnRhPrewitt()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能图像增强(锐化)!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图像增强(锐化):采用Prewitt算子!");

	int HWS = 3;
	int H[3][3] = { {1,0,-1},    
				{1,0,-1},
				{1,0,-1} };

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	fread(m_pImage, m_nImage, 1, fpo);

	unsigned char *ImageSize;
	ImageSize = new unsigned char[m_nImage];
	int red, green, blue;
	int X, Y;
	int TR, TG, TB;

	for (int i = 0; i < m_nImage; i = i + 3)
	{
		X = (i / 3) % m_nWidth;    
		Y = (i / 3) / m_nWidth;    
		red = green = blue = 0;

		for (int j = Y - HWS / 2; j < Y + HWS / 2 + 1; j++)                      //第j行
		{
			for (int k = X - HWS / 2; k < X + HWS / 2 + 1; k++)                  //第k列
			{
				if (j >= 0 && k >= 0 && k < m_nWidth && j < m_nHeight)
				{

					TR = j * m_nWidth * 3 + k * 3;
					red += H[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TR]);
					TG = j * m_nWidth * 3 + k * 3 + 1;
					green += H[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TG]);
					TB = j * m_nWidth * 3 + k * 3 + 2;
					blue += H[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TB]);
				}
			}
		}

		if (red >= 0 && red < 256) ImageSize[i] = red;
		else if (red < 0) ImageSize[i] = 0;      
		else ImageSize[i] = 0;

		if (green >= 0 && green < 256) ImageSize[i + 1] = green;
		else if (green < 0) ImageSize[i + 1] = 0;  
		else ImageSize[i + 1] = 0;

		if (blue >= 0 && blue < 256) ImageSize[i + 2] = blue;
		else if (blue < 0) ImageSize[i + 2] = 0;   
		else ImageSize[i + 2] = 0;
	}
	fwrite(ImageSize, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 400;
	Invalidate();
}


void CImageProcessingView::OnRhSobel()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能图像增强(锐化)!", MB_OK, 0);
		return;
	}
	AfxMessageBox("图像增强(锐化):采用Sobel算子!");

	int HWS = 3;
	int HX[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} };
	int HY[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	fread(m_pImage, m_nImage, 1, fpo);

	unsigned char *ImageSize;
	ImageSize = new unsigned char[m_nImage];
	int redX, greenX, blueX;
	int redY, greenY, blueY;
	int X, Y;
	int TR, TG, TB;

	for (int i = 0; i < m_nImage; i = i + 3)
	{
		X = (i / 3) % m_nWidth;    
		Y = (i / 3) / m_nWidth;    
		redX = greenX = blueX = 0;
		redY = greenY = blueY = 0;

		for (int j = Y - HWS / 2; j < Y + HWS / 2 + 1; j++)                      
		{
			for (int k = X - HWS / 2; k < X + HWS / 2 + 1; k++)                  
			{
				if (j >= 0 && k >= 0 && k < m_nWidth && j < m_nHeight)
				{

					TR = j * m_nWidth * 3 + k * 3;
					redX += HX[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TR]);
					redY += HY[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TR]);
					TG = j * m_nWidth * 3 + k * 3 + 1;
					greenX += HX[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TG]);
					greenY += HY[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TG]);
					TB = j * m_nWidth * 3 + k * 3 + 2;
					blueX += HX[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TB]);
					blueY += HY[(j - Y + HWS / 2)][(k - X + HWS / 2)] * (m_pImage[TB]);
				}
			}
		}

		int R, G, B;
		R = (int)(sqrt(redX*redX*1.0 + redY * redY*1.0));
		G = (int)(sqrt(greenX*greenX*1.0 + greenY * greenY*1.0));
		B = (int)(sqrt(blueX*blueX*1.0 + blueY * blueY*1.0));

		if (redX < 0 && redY < 0) ImageSize[i] = 0;
		else if (R > 255) ImageSize[i] = 255;
		else ImageSize[i] = R;

		if (greenX < 0 && greenY < 0) ImageSize[i + 1] = 0;
		else if (G > 255) ImageSize[i + 1] = 255;
		else ImageSize[i + 1] = G;

		if (blueX < 0 && blueY < 0) ImageSize[i + 2] = 0;
		else if (B > 255) ImageSize[i + 2] = 255;
		else ImageSize[i + 2] = B;
	}

	fwrite(ImageSize, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 400;
	Invalidate();
}


void CImageProcessingView::OnAddJy()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能添加椒盐噪声!", MB_OK, 0);
		return;
	}
	AfxMessageBox("椒盐噪声添加成功!");
	int i;
	srand((unsigned)time(NULL));

	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");

	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);

	unsigned char color;
	unsigned char red, green, blue;
	for (i = 0; i < m_nImage / 3; i++)
	{
		fread(&red, sizeof(char), 1, fpo);
		fread(&green, sizeof(char), 1, fpo);
		fread(&blue, sizeof(char), 1, fpo);

		if (rand() % 50 == 0)
		{
			red = 0;
			green = 0;
			blue = 0;
		}
		if (rand() % 50 == 1)
		{
			red = 255;
			green = 255;
			blue = 255;
		}

		fwrite(&red, sizeof(char), 1, fpw);
		fwrite(&green, sizeof(char), 1, fpw);
		fwrite(&blue, sizeof(char), 1, fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 101;
	Invalidate();
}
