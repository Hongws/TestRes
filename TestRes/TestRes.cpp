
// TestRes.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "TestRes.h"
#include "TestResDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef GOOD_HANDLE
#define GOOD_HANDLE(h) (NULL!=(h) && INVALID_HANDLE_VALUE!=(h))
#endif

BOOL ExtraceFileFromResource(CString strFileName, UINT uResourceID, CString strResourceType, BOOL bSystemFile);

// CTestResApp

BEGIN_MESSAGE_MAP(CTestResApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestResApp 构造

CTestResApp::CTestResApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	InitTmpPath();
}

CTestResApp::~CTestResApp()
{
	ClearTmpFile();
}

// 唯一的 CTestResApp 对象

CTestResApp theApp;


// CTestResApp 初始化

BOOL CTestResApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	if (IsAnotherProcessIsRunning())
		return FALSE;

	ReleaseComponent();

	CTestResDlg dlg;
	dlg.InitValue(m_cstrTmpPath);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

void CTestResApp::InitTmpPath()
{
	WCHAR wszTmpPath[MAX_PATH] = { 0 };
	GetTempPath(MAX_PATH, wszTmpPath);
	PathAppend(wszTmpPath, L"TestRes");
	CreateDirectory(wszTmpPath, NULL);
	m_cstrTmpPath = wszTmpPath;
}

void CTestResApp::ClearTmpFile()
{
	for (int i = 0; i < PROGRAME_COUNT; i++)
	{
		DeleteFile(m_cstrTmpPath + L"\\" + programe_name[i]);
	}
	RemoveDirectory(m_cstrTmpPath);
}

BOOL CTestResApp::IsAnotherProcessIsRunning()
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, L"TestRes");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CTestResApp::ReleaseComponent()
{
	for (int i = 0; i < PROGRAME_COUNT; i++)
	{
		TCHAR wszDestFilePath[MAX_PATH] = { 0 };
		_stprintf_s(wszDestFilePath, MAX_PATH - 1, L"%s\\%s", m_cstrTmpPath.GetBuffer(), programe_name[i]);//gh:释放工具到临时目录
		if (!ExtraceFileFromResource(wszDestFilePath, IDR_FILELISTZZZ + i, L"RESIN", true))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ExtraceFileFromResource(CString strFileName, UINT uResourceID, CString strResourceType, BOOL bSystemFile)
{
	BOOL bTemp = FALSE;
	TCHAR szTempPath[MAX_PATH] = { 0 };
	HGLOBAL hBuf = NULL;
	DWORD dwWrite = 0;
	HANDLE hFile = NULL;
	DWORD dwResourceSize = 0;

	hBuf = ::LoadResource(NULL, FindResource(NULL, MAKEINTRESOURCE(uResourceID/*你Resource 的ID*/), strResourceType));
	if (!hBuf)
	{
		return false;
	}

	dwResourceSize = SizeofResource(NULL, FindResource(NULL, MAKEINTRESOURCE(uResourceID/*你Resource 的ID*/), strResourceType));
	if (dwResourceSize == 0)
	{
		return false;
	}

	if (bSystemFile)
	{
		hFile = CreateFile(strFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 38, NULL);
	}
	else
	{
		hFile = CreateFile(strFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (!GOOD_HANDLE(hFile))
	{
		return false;
	}

	if (!WriteFile(hFile, (LPBYTE)hBuf, dwResourceSize, &dwWrite, NULL) || dwWrite != dwResourceSize)
	{
		return false;
	}
	CloseHandle(hFile);
	return TRUE;
}

