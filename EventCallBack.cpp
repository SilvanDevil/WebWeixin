#include "StdAfx.h"
#include "EventCallBack.h"
#include <assert.h>

EventCallBack::EventCallBack(AsynCallBack fun)
{
	assert(NULL != fun);
	m_CallBack = fun;
	m_lParam = NULL;
	m_wParam = NULL;
}

EventCallBack::~EventCallBack(void)
{
}

void EventCallBack::DoWork()
{
	assert(NULL != m_CallBack);
	m_CallBack(m_wParam,m_lParam);
	delete this;		//���������ͷţ����һ��Ҫͨ��new ,����ѹ��캯�����˽�У�ֻҪ����һ����Ԫ��������
}

void EventCallBack::PostIt( HWND hw,UINT uMsg )
{
	PostMessage(hw,uMsg,0,(LPARAM)this);
}
