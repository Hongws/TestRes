
// TestRes.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CTestResApp:
// 有关此类的实现，请参阅 TestRes.cpp
//

class CTestResApp : public CWinApp
{
public:
	CTestResApp();
	~CTestResApp();

	void	InitTmpPath();
	void	ClearTmpFile();
	//是否有一个进程已经在运行
	BOOL	IsAnotherProcessIsRunning();
	BOOL	ReleaseComponent();	

private:
	CString		m_cstrTmpPath;

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTestResApp theApp;
