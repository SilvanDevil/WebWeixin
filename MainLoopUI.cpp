#include "StdAfx.h"
#include "MainLoopUI.h"
#include "loginUI.h"
#include "WXAwrapper.h"
#include "StringUtility.h"
#include "MainPanel.h"
#include <sstream>
#include "HttpClientHelper.h"
#include "UserICon.h"
#include "EventCallBack.h"
#include "ByteStream.h"
#include "Utility.h"

MainLoopUI::MainLoopUI(void)
{
	m_Login = new loginUI();
	m_MainPannel = new MainPannel();
}

MainLoopUI::~MainLoopUI(void)
{
	delete m_Login; m_Login = NULL;
}
void MainLoopUI::InitWindow()
{
	WXAwrapper::GetInstance().Init(m_hWnd);
	m_Login->CreateDuiWindow(NULL,L"��½");
	PostMessage(UM_MSG_LOGIN);
}

LRESULT MainLoopUI::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    switch(uMsg)
    {
    case UM_MSG_LOGIN:
        OnLoginMsg(wParam,lParam);
        break;
    case UM_MSG_RESULT_QRCODE:
        OnQRCodeMsg(wParam,lParam);
        break;
    case UM_MSG_RESULT_LOGIN:
        OnLoginInfoMsg(wParam,lParam);
        break;
    case UM_MSG_RESULT_CONTACKLIST:
        OnContactListMsg(wParam,lParam);
        break;
    case UM_MSG_RESULT_RECVMSGLIST:
        OnRecvMsg(wParam,lParam);
        break;
    case UM_MSG_DO_CHECKASYN:
        OnDoCheckAsynMsg(wParam,lParam);
        break;
    case  UM_MSG_DO_DORECVMSG:
        OnDoRecvMsg(wParam,lParam);
        break;
    case UM_MSG_UI_SHOWMAINPANNEL:
        break;
	case UM_MSG_EVENT_CORE:
		OnEventMsg(wParam,lParam);
		break;
	case UM_MSG_DO_UPDATEICON:
		OnDoUpdateIconMsg(wParam,lParam);
		break;
    default:
        break;
    }
    return TRUE;
}

LRESULT MainLoopUI::OnClose( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	PostQuitMessage(0);
	return TRUE;
}

void MainLoopUI::OnLoginMsg( WPARAM wParam,LPARAM lParam )
{
	m_Login->CenterWindow();
	m_Login->ShowWindow();
}

void MainLoopUI::OnLogoutMsg( WPARAM wParam,LPARAM lParam )
{

}

void MainLoopUI::OnQRCodeMsg( WPARAM wParam,LPARAM lParam )
{
	BOOL nResult = wParam;
	if(nResult)
	{
		m_Login->ShowQRCodePicture();
		WXAwrapper::GetInstance().LoginInfo();
		
	}
	else
	{
		::MessageBox(0,L"��ȡ��ά��ʧ��",0,0);
	}
}

void MainLoopUI::OnLoginInfoMsg( WPARAM wParam,LPARAM lParam )
{
	BOOL nResult = wParam;
	if(nResult)
	{
		OutputDebugString(L"��¼�ɹ�\r\n");
		WXAwrapper::GetInstance().ContactList();
		
	}
	else
	{
		OutputDebugString(L"���Ϊ��½�ɹ�,�������\r\n");
		WXAwrapper::GetInstance().LoginInfo();
	}
}

void MainLoopUI::OnContactListMsg( WPARAM wParam,LPARAM lParam )
{
	BOOL nResult = wParam;
	std::string* strUtf8UserList = (std::string*)lParam;
	if(nResult)
	{
		OutputDebugString(L"��ȡ�û��б�ɹ�\r\n");
		m_ContactList = *strUtf8UserList;
		delete strUtf8UserList;
		ShowMainPannel();
	}
	else
	{
		::MessageBox(NULL,L"��ȡ��¼��Ϣʧ��",0,0);
	}
}

void MainLoopUI::OnRecvMsg( WPARAM wParam,LPARAM lParam )
{
	Json::Value* msg = (Json::Value*)wParam;
	assert(NULL != msg);
	Json::Value& AddListMsg = (*msg)["AddMsgList"];
    for(size_t i = 0; i != AddListMsg.size(); i++)
    {
        std::wstring msg = StringUtility::Utf8ToWString(AddListMsg[i]["Content"].asString());
        m_MainPannel->InsertNewMsg(AddListMsg[i]["FromUserName"].asString(),msg,false);
    }
}

void MainLoopUI::OnDoCheckAsynMsg( WPARAM wParam,LPARAM lParam )
{
    WXAwrapper::GetInstance().CheckAsyn();
}


void MainLoopUI::OnDoRecvMsg( WPARAM wParam,LPARAM lParam )
{
    WXAwrapper::GetInstance().Recv();
}


void MainLoopUI::OnDoUpdateIconMsg( WPARAM wParam,LPARAM lParam )
{
	//���浽Ӳ��
	//�������ݵ�
	char* pBuf = (char*)lParam;
	UINT uSucess = (WPARAM)wParam;
	assert(uSucess == 0 || uSucess == 1);
	assert(NULL != pBuf);

	char userID[256] = {0};
	ByteStream stream(pBuf,256);
	uint32_t uLen = 0;
	stream.Read(uLen);
	assert(uLen < 256);
	stream.Read(userID,uLen);
	if(uSucess)
	{
		uint32_t nSize = 0;
		stream.Read(nSize);
		if(UserICon::GetInstance().SaveIcon(userID,stream.GetPosBuffer(),nSize))
		{
			UserICon::GetInstance().UpdateIconDownload(userID,true);
			std::wstring PicPath = UserICon::GetInstance().GetPicPath(userID);
			m_MainPannel->UpdateFriendICon(userID,PicPath);

			//����������˵�����
			m_MainPannel->UpdateChatViewItemICon(userID,PicPath);
		}
	}
	else
	{
		UserICon::GetInstance().UpdateIconDownload(userID,false);
	}

	UserICon::GetInstance().StartDownload();

	SAFADELARRAY(pBuf);
}


void MainLoopUI::ShowMainPannel( bool bShow )
{
	if(!m_MainPannel->GetHWND())
	{
		OutputDebugString(L"��ʾ����\r\n");
		m_MainPannel->CreateDuiWindow(NULL,L"������");
		m_MainPannel->UpdateFriendList(m_ContactList);
	}
	
	m_MainPannel->ShowWindow(bShow);
	m_Login->ShowWindow(!bShow);
}


void MainLoopUI::OnEventMsg( WPARAM wParam,LPARAM lParam )
{
	/*InternalRequestContext* pC = (InternalRequestContext*)lParam;

	HttpClientHelper::HttpClientRespond res;
	HttpClientHelper::ByteArray body;
	pC->pThis->ReadRespond(pC->pThis->m_hSession,res);
	res.GetBody(body);
	SavePicture(body);*/
	EventCallBack* pEve = (EventCallBack*)lParam;
	assert(NULL != pEve);
	pEve->DoWork();



	//�첽�ص�����


}




