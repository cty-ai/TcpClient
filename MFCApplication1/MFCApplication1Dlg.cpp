
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include "socket.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region ConsoleWrite...

#define MAX_PATH 260
//调试窗口输出+换行
#define ConsoleWriteLine( fmt, ... ) {ConsoleWrite( fmt, ##__VA_ARGS__  );OutputDebugStringA("\r\n");}
//调试窗口输出
#define ConsoleWrite( fmt, ... ) {char szBuf[MAX_PATH]; sprintf_s(szBuf,fmt, ##__VA_ARGS__ ); OutputDebugStringA(szBuf);}

#pragma endregion //End ConsoleWrite...
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IP, m_editIp);
	DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
	DDX_Control(pDX, ID_SEND, m_buttenSend);
	DDX_Control(pDX, ID_CLEAR, m_buttenClear);
	DDX_Control(pDX, IDC_EDIT_S, m_editReq);
	DDX_Control(pDX, IDC_EDIT_R, m_editResp);
	DDX_Control(pDX, IDC_STATIC_S, m_staticS);
	DDX_Control(pDX, IDC_STATIC_R, m_staticR);
	DDX_Control(pDX, IDC_STATIC_T, m_staticT);
	DDX_Control(pDX, IDC_EDIT_H, m_editHead);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_SEND, &CMFCApplication1Dlg::OnBnClickedSend)
	ON_BN_CLICKED(ID_CLEAR, &CMFCApplication1Dlg::OnBnClickedClear)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	m_editReq.SetLimitText(UINT_MAX);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	m_editIp.SetLimitText(16);
	m_editPort.SetLimitText(5);
	m_editHead.SetLimitText(4);
	m_editIp.SetWindowTextW(L"127.0.0.1");
	m_editPort.SetWindowTextW(L"7800");
	m_editHead.SetWindowTextW(L"CIPC");
	//m_check.SetCheck(1);
	m_editReq.SetWindowTextW(L"发送区(双击可全选并复制)");
	m_editResp.SetWindowTextW(L"接收区(双击可全选并复制)");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI SendRcveThread(LPVOID pParam)
{
	CMFCApplication1Dlg *pThis = (CMFCApplication1Dlg *)pParam;
	CString reqWstr, respWstr, tmpCstr;

	std::string ip = wstringToString(pThis->m_ip.GetString());
	int port = _wtoi(pThis->m_port);

	std::string reqStr, respStr;
	pThis->m_editReq.GetWindowTextW(reqWstr);
	reqStr = wstringToString(reqWstr.GetString());

	tmpCstr.Format(L"S:%d", reqStr.length());
	pThis->m_staticS.SetWindowTextW(tmpCstr);


	eqkSocket sk(1000, 300 * 1000);
	eqkSocket::SocketErr err;
	auto start = std::chrono::high_resolution_clock::now();
	if (!pThis->m_head.IsEmpty()){
		//ConsoleWrite(wstringToString(pThis->m_head.GetString()).c_str());
		std::string headStr;
		int len = reqStr.length();
		len = htonl(len);
		char head[8] = "CIPC";
		wstringToString(pThis->m_head.GetString());
		_snprintf_s(head, 8, "%s", wstringToString(pThis->m_head.GetString()).c_str());
		memcpy(head + 4, &len, 4);
		err = sk.ccsrcSocketWithHead(ip, port, reqStr, respStr, head, 8);
	}
	else{
		err = sk.ccsrcSocket(ip, port, reqStr, respStr);
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> tm = end - start;
	tmpCstr.Format(L"耗时:%.0fms", tm.count());
	pThis->m_staticT.SetWindowTextW(tmpCstr);
	//printf("耗时:%fms", tm.count());
	UpdateWindow(pThis->m_hWnd);
	if (err == eqkSocket::SocketErr::SOCKET_NO_ERR){
		printf("send [%s:%d] success\r\n", ip.c_str(), port);
		respStr = utf8togbk(respStr.c_str());
		respWstr = stringToWstring(respStr).c_str();
		pThis->m_editResp.SetWindowTextW(respWstr);
		tmpCstr.Format(L"R:%d", respStr.length());
		pThis->m_staticR.SetWindowTextW(tmpCstr);
	}
	else{
		std::string errMsg = sk.getErrStr(err);
		printf("%s\r\n", errMsg.c_str());
		pThis->m_editResp.SetWindowTextW(stringToWstring(errMsg).c_str());
	}
	return 0;
}

void CMFCApplication1Dlg::OnBnClickedSend()
{
	// TODO:  在此添加控件通知处理程序代码
	m_editIp.GetWindowTextW(m_ip);
	m_editPort.GetWindowTextW(m_port);
	m_editHead.GetWindowTextW(m_head);
	m_editResp.SetWindowTextW(L"waiting...");
	UpdateWindow();
	m_staticT.SetWindowTextW(L"耗时:");

	DWORD threadId;
	HANDLE handle = CreateThread(NULL, 0, SendRcveThread, this, 0, &threadId);
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);

}


void CMFCApplication1Dlg::OnBnClickedClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_editReq.SetWindowTextW(L"");

}


void CMFCApplication1Dlg::ChangeSize(CWnd *pWnd, int cx, int cy, int ID)
{
	if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前的大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标

		//    cx/m_rect.Width()为对话框在横向的变化比例
		//rect.left = rect.left*cx / m_rect.Width();//调整控件大小
		switch (ID){
		case IDC_EDIT_S:
		{
			rect.right = rect.right + cx - m_rect.Width();
			rect.bottom = rect.bottom + (cy - m_rect.Height()) / 2;
			break;
		}
		case IDC_EDIT_R:
		{
			rect.right = rect.right + cx - m_rect.Width();
			rect.top = rect.top + (cy - m_rect.Height()) / 2;
			rect.bottom = rect.bottom + cy - m_rect.Height();
			break;
		}
		case ID_CLEAR:
		{
			rect.left = rect.left + cx - m_rect.Width();
			rect.right = rect.right + cx - m_rect.Width();
			break;
		}
		case IDC_STATIC_R:
		case IDC_STATIC_S:
		{
			rect.top = rect.top + cy - m_rect.Height();
			rect.bottom = rect.bottom + cy - m_rect.Height();
			break;
		}
		}

		pWnd->MoveWindow(rect);//设置控件大小
	}
}

void CMFCApplication1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == 1) return;//最小化则什么都不做
	// TODO:  在此处添加消息处理程序代码

	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_EDIT_S);
	ChangeSize(pWnd, cx, cy, IDC_EDIT_S);
	pWnd = GetDlgItem(IDC_EDIT_R);
	ChangeSize(pWnd, cx, cy, IDC_EDIT_R);
	pWnd = GetDlgItem(ID_CLEAR);
	ChangeSize(pWnd, cx, cy, ID_CLEAR);
	pWnd = GetDlgItem(IDC_STATIC_R);
	ChangeSize(pWnd, cx, cy, IDC_STATIC_R);
	pWnd = GetDlgItem(IDC_STATIC_S);
	ChangeSize(pWnd, cx, cy, IDC_STATIC_S);
	GetClientRect(&m_rect);

}


void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rcS, rcR;
	if (nFlags == MK_LBUTTON){
		GetDlgItem(IDC_EDIT_S)->GetWindowRect(&rcS); //获取ID为IDC_LINK的Static Text的范围
		GetDlgItem(IDC_EDIT_R)->GetWindowRect(&rcR); //获取ID为IDC_LINK的Static Text的范围
		ScreenToClient(&rcS);//将控件大小转换为在对话框中的区域坐标
		ScreenToClient(&rcR);//将控件大小转换为在对话框中的区域坐标
		//ConsoleWriteLine("pos.y=%d, rcS.bottom=%d, rcR.top=%d", point.y, rcS.bottom, rcR.top);
		if (point.y >= rcS.bottom - 10 && point.y <= rcR.top + 10){
			SetCapture();
			rcS.bottom = point.y - 1;
			rcR.top = point.y + 1;
			GetDlgItem(IDC_EDIT_S)->MoveWindow(rcS);
			GetDlgItem(IDC_EDIT_R)->MoveWindow(rcR);
		}

	}
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CMFCApplication1Dlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CPoint pos;
	GetCursorPos(&pos);  //获取当前鼠标位置
	CRect rcS, rcR;
	GetDlgItem(IDC_EDIT_S)->GetWindowRect(&rcS); //获取ID为IDC_LINK的Static Text的范围
	GetDlgItem(IDC_EDIT_R)->GetWindowRect(&rcR); //获取ID为IDC_LINK的Static Text的范围
	//ConsoleWriteLine("pos.y=%d, rcS.bottom=%d, rcR.top=%d", pos.y, rcS.bottom, rcR.top);
	if (pos.y >= rcS.bottom - 10 && pos.y <= rcR.top + 10){
		SetCursor(LoadCursor(NULL, IDC_SIZENS)); //设置双箭头指向南北 
	}
	else{
		return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
	}
	//return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
	return true;
}


void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	ReleaseCapture();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCApplication1Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 400;
	lpMMI->ptMinTrackSize.y = 300;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
