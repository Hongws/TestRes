
// TestResDlg.h: 头文件
//

#pragma once

static WCHAR programe_name[][MAX_PATH] =
{
	L"FileListZZZ.exe",
	L"TestExe.exe"
};
#define  PROGRAME_COUNT sizeof(programe_name) / (sizeof(WCHAR) * MAX_PATH)

// CTestResDlg 对话框
class CTestResDlg : public CDialogEx
{
// 构造
public:
	CTestResDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTRES_DIALOG };
#endif

public:
	void	InitValue(CString cstrTmpPath);
	BOOL	IsAnotherProcessIsRunning();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CString		m_cstrTmpPath;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
