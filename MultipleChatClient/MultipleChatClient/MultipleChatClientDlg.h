
// MultipleChatClientDlg.h: 头文件
//

#pragma once


// CMultipleChatClientDlg 对话框
class CMultipleChatClientDlg : public CDialog
{
// 构造
public:
	CMultipleChatClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MULTIPLECHATCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendbutton();
	CString sendingMsg;
	CString username;
	afx_msg void OnCbnSelchangeQuickmsg();
	CString quickMsg;
	afx_msg void OnEnKillfocusYourname();
	afx_msg void OnClose();
};
