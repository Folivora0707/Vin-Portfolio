#pragma once


// CImageXZDlg 对话框

class CImageXZDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageXZDlg)

public:
	CImageXZDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImageXZDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_XZ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_xzds;
};
