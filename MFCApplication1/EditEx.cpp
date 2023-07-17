#include "stdafx.h"
#include "EditEx.h"


CEditEx::CEditEx()
{
}


CEditEx::~CEditEx()
{
}

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CEditEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SetSel(0, -1);
	Copy();
	//CEdit::OnLButtonDblClk(nFlags, point);
}
