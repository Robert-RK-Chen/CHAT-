
// MultipleChatClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MultipleChatClient.h"
#include "MultipleChatClientDlg.h"
#include "afxdialogex.h"
#include <string>
#include "Client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Client c;
bool firstFlag = true;
CString msg;

UINT GetMsgFromClient(LPVOID lpParam)
{
	c.GetMsg();
	return 0;
}


// CMultipleChatClientDlg 对话框

CMultipleChatClientDlg::CMultipleChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MULTIPLECHATCLIENT_DIALOG, pParent)
	, sendingMsg("")
	, username("")
	, quickMsg("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultipleChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEND, sendingMsg);
	DDX_Text(pDX, IDC_YOURNAME, username);
	DDX_CBString(pDX, IDC_FASTMSG, quickMsg);
}

BEGIN_MESSAGE_MAP(CMultipleChatClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SENDBUTTON, &CMultipleChatClientDlg::OnBnClickedSendbutton)
	ON_CBN_SELCHANGE(IDC_FASTMSG, &CMultipleChatClientDlg::OnCbnSelchangeQuickmsg)
	ON_EN_KILLFOCUS(IDC_YOURNAME, &CMultipleChatClientDlg::OnEnKillfocusYourname)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMultipleChatClientDlg 消息处理程序

BOOL CMultipleChatClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMultipleChatClientDlg::OnPaint()
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
		CDialog::OnPaint();
	}

	// 启动服务
	if (firstFlag == true)
	{
		c.setipAddress("127.0.0.1");
		c.setPort(54000);
		//c.setipAddress("112.124.19.161");
		//c.setPort(4999);

		try
		{
			c.InitWinsock();
		}
		catch (exception& e)
		{
			MessageBoxA(e.what(), "来自 Chat! 的提示", MB_ICONINFORMATION);
			exit(1);
		}

		try
		{
			c.CreateSock();
		}
		catch (exception& e)
		{
			MessageBoxA(e.what(), "来自 Chat! 的提示", MB_ICONINFORMATION);
			exit(1);
		}

		c.HintStructure();

		try
		{
			c.ConnServer();
		}
		catch (exception& e)
		{
			MessageBoxA(e.what(), "来自 Chat! 的提示", MB_ICONINFORMATION);
			exit(1);
		}

		CWinThread* th = AfxBeginThread(GetMsgFromClient, 0);

		firstFlag = false;
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMultipleChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMultipleChatClientDlg::OnBnClickedSendbutton()
{
	CString time;
	SYSTEMTIME st;
	GetLocalTime(&st);
	time.Format("%2d:%2d:%2d", st.wHour, st.wMinute, st.wSecond);
	GetDlgItem(IDC_SEND)->GetWindowTextA(sendingMsg);
	GetDlgItem(IDC_YOURNAME)->GetWindowTextA(username);
	CEdit* msgWindow = (CEdit*)GetDlgItem(IDC_MSGBOX);
	CEdit* inputWindow = (CEdit*)GetDlgItem(IDC_SEND);
	CEdit* nameWindow = (CEdit*)GetDlgItem(IDC_YOURNAME);
	if (username.IsEmpty())
	{
		MessageBox("请先取一个昵称吧！\n昵称是你本次聊天的唯一标识符，\n取了之后不能更改哦！", "CHAT! 提示：昵称为空", MB_ICONINFORMATION);
		nameWindow->SetFocus();
	}
	else
	{
		if (sendingMsg.IsEmpty())
		{
			MessageBox("不能发送空消息！", "CHAT! 的提示：消息为空", MB_ICONINFORMATION);
		}
		else
		{
			std::string copymsg = "> " + username + "，" + time + "：\r\n" + sendingMsg + "\r\n\r\n";
			c.Send(copymsg);
			msgWindow->SetSel(-1);
			msgWindow->ReplaceSel(copymsg.c_str());
			inputWindow->SetWindowTextA("");
		}
	}
}

void CMultipleChatClientDlg::OnCbnSelchangeQuickmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_QUICKMSG)->GetWindowTextA(quickMsg);
	CComboBox* quickMsgBox = (CComboBox*)GetDlgItem(IDC_QUICKMSG);
	CEdit* inputWindow = (CEdit*)GetDlgItem(IDC_SEND);
	if (quickMsg == "自定义")
	{
		// 自定义代码
		quickMsgBox->SetCurSel(-1);
	}
	else
	{
		inputWindow->SetWindowTextA(quickMsg);
		quickMsgBox->SetCurSel(-1);
	}
}

void CMultipleChatClientDlg::OnEnKillfocusYourname()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit* nameWindow = (CEdit*)GetDlgItem(IDC_YOURNAME);
	GetDlgItem(IDC_YOURNAME)->GetWindowTextA(username);
	CString copyName = username;
	copyName.Remove(' ');
	if (copyName.IsEmpty())
	{
		MessageBox("昵称不能取空格哦！", "CHAT! 提示：昵称为空！", MB_ICONINFORMATION);
		nameWindow->SetFocus();
	}
	else
	{
		CString time;
		SYSTEMTIME st;
		GetLocalTime(&st);
		time.Format("%2d:%2d:%2d", st.wHour, st.wMinute, st.wSecond);
		CEdit* msgWindow = (CEdit*)GetDlgItem(IDC_MSGBOX);
		std::string copymsg = "> " + username + " 已进入聊天，时间：" + time + "\r\n\r\n";
		c.Send(copymsg);
		msgWindow->SetSel(-1);
		msgWindow->ReplaceSel(copymsg.c_str());
		nameWindow->EnableWindow(false);
	}
}

void CMultipleChatClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString time;
	SYSTEMTIME st;
	GetLocalTime(&st);
	time.Format("%2d:%2d:%2d", st.wHour, st.wMinute, st.wSecond);
	GetDlgItem(IDC_YOURNAME)->GetWindowTextA(username);
	if (!username.IsEmpty())
	{
		std::string copymsg = "> " + username + " 已离开聊天，时间：" + time + "\r\n\r\n";
		c.Send(copymsg);
	}
	CDialog::OnClose();
}