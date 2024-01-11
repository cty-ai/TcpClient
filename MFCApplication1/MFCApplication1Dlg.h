
// MFCApplication1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "EditEx.h"
#include <chrono>

// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedClear();
	CRect m_rect;
	CEdit m_editIp;
	CEdit m_editPort;
	CButton m_buttenSend;
	CButton m_buttenClear;
	CEditEx m_editReq;
	CEditEx m_editResp;
	CStatic m_staticS;
	CStatic m_staticR;
	CStatic m_staticT;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	void ChangeSize(CWnd *pWnd, int cx, int cy, int ID);
	CEdit m_editHead;
	CString m_ip, m_port, m_head;
};
