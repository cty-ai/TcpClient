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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetSel(0, -1);
	Copy();
	//CEdit::OnLButtonDblClk(nFlags, point);
}
