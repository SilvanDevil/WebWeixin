/************************************************************************/
/* ˵����HTTP Client ��������ķ�װ                                     */
/* ʱ�䣺2015��12��9�� 22:37:07											*/
/* ���ߣ�xjm															*/
/************************************************************************/

/*
Ĭ�϶����ӣ�������Ҫʹ�ö�Ӧ�����غ���
*/
#ifndef	HTTPCLIENTHELPER_H
#define	HTTPCLIENTHELPER_H

#include <Windows.h>
#include "Wininet.h"
#include <string>
#include <map>
#include <vector>

struct InternalRequestContext ;
class HttpClientHelper
{
public:
	class HttpClientRespond;

	typedef std::map<std::string,std::string> HeadItems;
	typedef HeadItems::const_iterator HeadItemsIt;
	typedef std::vector<char>	ByteArray;
	typedef void (*RequstCallback)(HttpClientRespond& repond,DWORD_PTR dwContext);		//֪ͨ�ⲿ�ص�
	
public:
	HttpClientHelper(std::string host,int port = 80,bool bAsyn = false);
	~HttpClientHelper();

	bool Connect();
	void Close();

	//ͬ���ӿ�
	bool Post(std::string requestObj, const HeadItems* headItems = NULL, const char* data = NULL, int len = 0);
	bool Get(std::string requestObj = "", const HeadItems* headItems = NULL, const char* data = NULL, int len = 0);
	bool Head(std::string requestOb = "");

	bool Post(std::string requestObj, HttpClientRespond* repond , const HeadItems* headItems = NULL, const char* data = NULL, int len = 0);
	bool Get(std::string requestObj, HttpClientRespond* repond , const HeadItems* headItems = NULL, const char* data = NULL, int len = 0);

	//�첽�ӿ�
	bool Get(std::string requestObj, RequstCallback fun, DWORD_PTR dwContext =NULL ,const HeadItems* headItems = NULL, const char* data = NULL, int len = 0);
    bool Post(std::string requestObj, RequstCallback fun, DWORD_PTR dwContext =NULL ,const HeadItems* headItems = NULL, const char* data = NULL, int len = 0);
	//��ȡ��תurl,��Ϊwininet��Ĭ���Զ���ת����ʱ��������Ҫ��ת��URL��ַ������һЩ��������
	std::string GetLocationUrl();

	std::string GetRequestObj();

	//��url��host
	//tip:����������htt heads�������ã�����Ҫ����flag��������ϵͳ��COOKIE
	bool SetCookie(const std::string& cookie );

	std::string GetCookie(const std::string& cookieName);

	//����״̬��
	int GetResultCode();

	//Ĭ���Զ��ض��򣬵���ʱ����Ҫ��ȡ�ص����URL��һЩ��Ϣ
	void SetAutoRedirect(bool bVal);

	static void __stdcall HttpCallBack(HINTERNET hInternet,
		DWORD_PTR dwContext,
		DWORD dwInternetStatus,
		LPVOID lpvStatusInformation,
		DWORD dwStatusInformationLength);


private:
	bool Action(std::string type,std::string requestObj, HttpClientRespond* repond , const HeadItems* headItems = NULL, const char* data = NULL, int len = 0);
	bool Action(std::string type,std::string requestObj, RequstCallback fun, DWORD_PTR dwContext,const HeadItems* headItems = NULL, const char* data = NULL, int len = 0);

public:
	class HttpClientRespond
	{
		
	public:
		HttpClientRespond() { m_statuCode = 0; }
		~HttpClientRespond() {}

		void SetStatuCode(int code);
		void SetHeads(const HeadItems& heads);
		void SetBody(const ByteArray& body);

		void GetBody(ByteArray& body);
		void GetBody(std::string& body);
		int  GetStatuCode() {return m_statuCode;}

		std::string GetHeadItem(std::string name);
		std::string operator[](std::string name);

	private:
		int			m_statuCode;
		HeadItems	m_heads;
		ByteArray	m_bodys;
	};

private:
	void InitInternet();

	std::string	GetHeardItems(const HeadItems* headItems);

	//��ȡHead
	static void ReadHead(HINTERNET hRequest, HeadItems& heads, int& statuCode);

	//��ȡBODY����
	static void ReadData(HINTERNET hRequest,ByteArray& bodyData);

	//�ͷź���
	void CloseRequestInfo(HINTERNET hRequest);

	//��ȡ������
	static std::string GetRequestLine(const std::string& head);

	//��ȡ����
	std::string GetHostDomain();

private:
	static void ParseHead(const std::string& head, HeadItems& heads);

	static bool ParseLineText(const std::string& line, std::string& name, std::string& value);

	//����������

	static void ParsRequestLine(std::string line,std::string& obj);

	static bool CheckIOPENDING(DWORD dwResult);

public:
	static bool ReadRespond(HINTERNET hSession,HttpClientRespond& respond);

public:
	HINTERNET		m_hInternetRoot;
	HINTERNET		m_hInternetConnect;
	HINTERNET		m_hSession;				//����ṹ����Ҫ
	std::string		m_host;
	int				m_port;
	std::string		m_agent;
	bool			m_bAutoRedirect;
	bool			m_bAsyn;
    InternalRequestContext* m_context;
};


struct InternalRequestContext 
{
	DWORD_PTR ptr;
	HttpClientHelper::RequstCallback fn;
	HttpClientHelper* pThis;        //�����Լ�
	char*           postData;
	bool    bAutoRelese;
	int             State;

	InternalRequestContext()
	{
		ptr = NULL;
		fn = NULL;
		pThis = NULL;
		postData = NULL;
		bAutoRelese = true;
		State = 0;
	}

	~InternalRequestContext()
	{
		if(bAutoRelese)
		{
			if(NULL != pThis)   delete pThis; pThis = NULL;
		}

		if(NULL != postData) delete []postData; postData = NULL;
	}
};


#endif


