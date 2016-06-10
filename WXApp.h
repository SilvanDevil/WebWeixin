#pragma once

/************************************************************************/
/* ˵����΢�Ŷ���                                                       */
/* ���ߣ�xjm															*/
/* ʱ�䣺2016��1��10�� 14:50:06											*/
/************************************************************************/

#include <string>
#include <vector>
#include "json/json.h"
#include "HttpClientHelper.h"

struct MsgInfo 
{
    int type;
    std::string id;
    std::string msg;
    std::string fromeUser;
    std::string toUser;
};

typedef bool (*RecvMsgWrapperCallBack)(const std::vector<MsgInfo>& msgList);

class WXApp
{
public:
	WXApp(void);
	~WXApp(void);

	typedef std::vector<char>	ByteArray;
	
	enum UserInfoIndex
	{
		eAlias,
		eNickName,
	};

public:
	bool		login();
	std::string	requestLoginInfo();
	bool		requestLoinPage(std::string strLoginUrl);
	bool		SendMsg(std::string userName, std::string body);
	bool		WXInit();
	void		GetUserContactList(std::string& contactList);

public:
	Json::Value GetUserInfo(std::string Name,UserInfoIndex Key = eAlias);

	//��ȡ�Լ����û���Ϣ
	Json::Value GetSelfUserInfo();


private:
	Json::Value	GetBaseRequest();
	std::string GetInitJsonBody();
    std::string GetWxSyncBody();
//��ʱ
public:
	std::string	requestUUID();
	bool		requestQRCode(std::string strUUID,ByteArray& array);
	bool		requestContactList();
	bool		RequestAysnCheck(int& nResult);
	bool		RecvMsg(Json::Value& msg);

private:
	std::string GetLoginPageRequestObj(const std::string& url);
	std::string CreateTextMsg(std::string userName,std::string body);
	std::string GetRequestCheckAysnc();

private:
	bool		ParseInitInfo(const std::string& str);
	bool		ParseContactInfo(const std::string& str);
	bool		ParseJsonStr(const std::string& str,Json::Value& obj);

public:
    bool        ParseNewCheckAsyncResult(const std::string& str,Json::Value& obj);
	int			GetCheckAsyncResult(std::string& str);
	

private:
	std::string m_UUID;
	std::string m_EncryKey;
	std::string	m_Sid;
	std::string m_PassTicket;
	UINT		m_WxUin;
	UINT		m_MsgID;

	Json::Value	m_User;
	Json::Value m_ContactList;
	Json::Value m_CheckAysn;
    Json::Value m_AysnCheckResult;
};
