#pragma once


// CImagePYDlg 对话框

class CImagePYDlg : public CDialog
{
	DECLARE_DYNAMIC(CImagePYDlg)

public:
	CImagePYDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImagePYDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_xPY;
	int m_yPY;
};
