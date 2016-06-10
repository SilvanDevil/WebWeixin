#pragma once

#include "json/json.h"
class MainPannel:public WindowImplBase
{
public:
	MainPannel();
	~MainPannel();
	DUI_DECLARE_MESSAGE_MAP()

	virtual CDuiString GetSkinFolder() {return L"";}
	virtual CDuiString GetSkinFile() {return L"Main/MainPanel.xml";}
	virtual LPCTSTR GetWindowClassName(void) const {return L"MainPanel";}
	virtual void InitWindow();
	virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void OnClick(TNotifyUI& msg);
	void OnListItemSelect(TNotifyUI& msg);
	void OnReturn(TNotifyUI& msg);	

public:
    void    UpdateFriendList(std::string& friendList);
	void	UpdateFriendICon(std::string strUserID,std::wstring strPath);
	void	UpdateChatViewItemICon(std::string strUserID,std::wstring strPath);
	void	StartAsyn();		//������ʱ�����Ϣ
	void	StartDownloadIcon();

private:
	void	OnFriendListSelect(TNotifyUI& msg);
	void	OnSendMsg(TNotifyUI& msg);
	void	OnTimer(WPARAM wParam,LPARAM lParam);
	void	OnReturnKey();
	void	OnCloseBtn(TNotifyUI& msg);

private:
    void	ToJsonObj(std::string& friendList,Json::Value& jsonOb);
	Json::Value	GetFriendListJson(std::string id);
private:
    void	UpdateFriendListUI();
	void	InsertByJsonInfo(Json::Value& jsonObj);
	void	UpdateChatView(std::string id,bool bShow=true);		//����List������л�����Ӧ��chat��ʾ
	CChildLayoutUI*	GetChatView(std::string id);		//��Chat Container��ȡchat view
	CChildLayoutUI*	CreateChatView(std::string id);
	void	SetChildContainerVisit(CContainerUI* ui,bool bVisit);
	void	InitChateView(std::string id,CChildLayoutUI* pChatView);
	void	UpdateUserBar();

	//������Ϣ
private:
	CChildLayoutUI*	CreateNormaTextMsg(std::string id,std::wstring msg,bool bLeft = true);
	void			SendNormalTextMsg(std::string id,std::wstring msg);
	void			CheckAsyn();
public:
    //���������Ҫ�Ż�
    void            InsertNewMsg(std::string id,std::wstring msg,bool bLeft = true);

private:
	std::wstring		FriendListItemUIClass(std::string id);
	std::wstring		ChatViewUIClass(std::string id);
	std::string			GetIDFromUIClass(std::wstring name);
	bool				IsChatView(std::string id);
    void                GetBubblePos(const std::wstring& str,int& height,int& width,bool bLeft = true);

public:
	CControlUI*		GetControlUI(CDuiString name);
	CListUI*		GetFriendListUI();
	CListUI*		GetChatViewListUI(std::string id);
	CContainerUI*	GetChatContainerUI();
	CControlUI*		GetNoSessionLaoutUI();
	CLabelUI*		GetChatViewTalkerUI(CContainerUI* container);
	CRichEditUI*	GetChatViewInput(std::string id);
	CContainerUI*	GetFriendListItemUI(std::string id);
	CControlUI*		GetFriendListItemIconUI(std::string id);
	CControlUI*		GetMainNameUI();

	

private:
    Json::Value     m_FriendList;
	bool			m_bInitFiendList;

};