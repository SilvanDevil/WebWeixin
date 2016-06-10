#pragma once
//�������࣬��ʼ��Ϊduilibû������࣬�Լ�׼��ȥ��װ����
class CListContainerItemUI:public CContainerUI, public IListItemUI
{
public:
	CListContainerItemUI(void);
	~CListContainerItemUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	//��������һ���¼�,һ����Ⱦ
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	virtual void DoEvent(TEventUI& event);

	//IListItemUI
	virtual int GetIndex() const;
	virtual void SetIndex(int iIndex);
	virtual IListOwnerUI* GetOwner();
	virtual void SetOwner(CControlUI* pOwner);
	virtual bool IsSelected() const;
	virtual bool Select(bool bSelect = true, bool bTriggerEvent=true);
	virtual bool IsExpanded() const;
	virtual bool Expand(bool bExpand = true);
	virtual void DrawItemText(HDC hDC, const RECT& rcItem);

	//������
	void		DrawItemBk(HDC hDC, const RECT& rcItem);
	void		Invalidata();
	bool		Activate();
public:
	void		DoItemEvent(TEventUI& event);


private:
	int					m_iIndex;
	IListOwnerUI*		m_pOwner;
	bool				m_bSelected;
	UINT				m_uButtonState;
};
