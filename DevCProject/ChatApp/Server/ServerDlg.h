// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__6D9D1B03_052C_4A2C_A8F4_0A499EF73D28__INCLUDED_)
#define AFX_SERVERDLG_H__6D9D1B03_052C_4A2C_A8F4_0A499EF73D28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ServerSock.h"
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

#include "vfw.h"
#pragma comment (lib,"vfw32")


//�������ݰ����ͣ��ı���ͼ��
enum PackageType  {ptText, ptImage};

//����Ӧ�ò����ݰ��ṹ
class  CPackage
{
public:
	PackageType m_Type;
	DWORD		m_dwSize;	//���ݱ���С
	DWORD		m_dwContent;//���ݰ�Ӧ�������ݴ�С
	DWORD		m_dwData;	//m_Data�Ĵ�С
	BYTE		m_Data[];	//���ݻ�����
};



class CServerDlg : public CDialog
{
// Construction
public:
	void ReceiveData();
	void OnCancel();
	void AcceptConnect();
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

	CServerSock		m_ServerSock;
	CServerSock		m_ClientSock;
	HWND			m_hWndVideo; //��Ƶ��ʾ����
	BOOL			m_bSendImage;	//�Ƿ�������
	HGLOBAL			m_hGlobal;
// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CRichEditCtrl	m_InfoList;
	CStatic	m_Panel;
	CButton	m_Video;
	CEdit	m_SendInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSend();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__6D9D1B03_052C_4A2C_A8F4_0A499EF73D28__INCLUDED_)
