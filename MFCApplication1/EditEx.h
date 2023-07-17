#pragma once
#include "afxwin.h"
class CEditEx :
	public CEdit
{
public:
	CEditEx();
	~CEditEx();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
};

