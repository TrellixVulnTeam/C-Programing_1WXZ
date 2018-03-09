// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_hGlobal = NULL;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_IMAGE, m_Image);
	DDX_Control(pDX, IDC_INFOLIST, m_InfoList);
	DDX_Control(pDX, IDC_SENDCONTENT, m_SendContent);
	DDX_Control(pDX, IDC_SERVERIP, m_ServerIP);
	DDX_Control(pDX, IDC_PORT, m_Port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_ClientSock.SetDialog(this);

	BOOL bRet = InitSocket();
	if (!bRet)
	{
		MessageBox("��ʼ���׽���ʧ��!");
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CClientDlg::OnOK() 
{
	
}

void CClientDlg::ReceiveData()
{
	static int nMaxLen = 1024*10;
	HGLOBAL hGlobal = GlobalAlloc(GHND, nMaxLen);
	BYTE* pBuffer = (BYTE*)GlobalLock(hGlobal);
	int nFact = m_ClientSock.Receive(pBuffer, nMaxLen);
	static BOOL bCompleted = TRUE;	//���ݰ��Ƿ�����
	static BOOL bFirstRec = TRUE;	//�״ν�������

	int nPackage = sizeof(CPackage);	//�������ݰ���С
rec:
	if(bFirstRec)	//�״ν�������,��Ϊ���ݰ���һ���������������ݰ��ṹ��Ϣ�����ݲ�һ������
	{
		CPackage* pPackage = (CPackage*)pBuffer;
		if (nFact - nPackage == pPackage->m_dwContent)	//������һ�����ݰ��Ĵ�С
		{
			bCompleted = TRUE;
			HandleRecData(pPackage);
			
			bFirstRec = TRUE;	
		
		}
		else	//���ݰ������������ܴ���һ�����ݰ�Ҳ����С��һ�����ݰ�
		{
			bCompleted = FALSE;
			if (nFact - nPackage < pPackage->m_dwContent)	//���ݰ���û�а�������������,�����˷ְ�
			{
				if (m_hGlobal != NULL)						//��m_hGlobal�д洢����
				{
					GlobalFree(m_hGlobal);
				}
				m_hGlobal = GlobalAlloc(GHND, nFact);
				BYTE* pData = (BYTE*)GlobalLock(m_hGlobal);
				//���Ʋ�������
				memcpy(pData, pBuffer, nFact);
				GlobalUnlock(m_hGlobal);
				bFirstRec = FALSE;
			}
			else											//���ݰ��а�����ֹһ�����ݰ���������ճ��
			{
				HandleRecData(pPackage);
				//����ʣ�������
				int nLeaving = nFact - nPackage - pPackage->m_dwContent;
				BYTE* pTmp = pBuffer + nPackage + pPackage->m_dwContent;
				while (nLeaving > 0)
				{
					bCompleted = FALSE;
					if (nLeaving > nPackage)	//���ݰ��������������ݰ��ṹ
					{
						CPackage* pPackage = (CPackage*) pTmp;
						//�����а���һ����һ�����ϵ����ݰ�
						if (nLeaving >= pPackage->m_dwContent + nPackage)
						{
							HandleRecData(pPackage);
							nLeaving -= pPackage->m_dwContent + nPackage;
							if (nLeaving == 0)
								bFirstRec = TRUE;
							pTmp += pPackage->m_dwContent + nPackage;
						}
						else	//�����а�����һ�����ݰ����ݲ�����
						{
							if (m_hGlobal != NULL)						//��m_hGlobal�д洢����
							{
								GlobalFree(m_hGlobal);
							}
							m_hGlobal = GlobalAlloc(GHND, nLeaving);
							BYTE* pData = (BYTE*)GlobalLock(m_hGlobal);
							memcpy(pData, pTmp, nLeaving);
							GlobalUnlock(m_hGlobal);
							nLeaving = 0;
							bCompleted = FALSE;
							GlobalUnlock(m_hGlobal);
							break;

						}
					}
					else		//���ݰ����в����������ݰ��ṹ	
					{
						if (m_hGlobal != NULL)						//��m_hGlobal�д洢����
						{
							GlobalFree(m_hGlobal);
						}
						m_hGlobal = GlobalAlloc(GHND, nLeaving);
						BYTE* pData = (BYTE*)GlobalLock(m_hGlobal);
						memcpy(pData, pTmp, nLeaving);
						nLeaving = 0;
						bCompleted = FALSE;
						GlobalUnlock(m_hGlobal);
						break;						
					}
				
				}
			}

		}
	}
	else							//bFirstRec==FALSE �����״ν�������
	{
		if (bCompleted)				//֮ǰ���յ����ݰ���������
		{
			bFirstRec = TRUE;
			goto rec;
		}
		if (bCompleted == FALSE)	//֮ǰ�����ݰ������˷ְ���ճ����������������	
		{
			//��ȡ֮ǰ���յ����ݴ�С
			DWORD dwSize = GlobalSize(m_hGlobal);
			BYTE* pGlobal = (BYTE*)GlobalLock(m_hGlobal);
			BYTE* pBuf = new BYTE[dwSize];
			//�����ݸ��Ƶ�pBuf��
			memcpy(pBuf, pGlobal, dwSize);
			GlobalUnlock(m_hGlobal);
			GlobalFree(m_hGlobal);
			m_hGlobal = NULL;
			//������ݰ��ṹ���������Ƚ������ݰ��Ľṹ
			if (dwSize < nPackage)
			{
				if(dwSize + nFact < nPackage)	//����ǰ��������ӵ������Բ����������������ݰ��ṹ
				{
					m_hGlobal = GlobalAlloc(GHND, dwSize + nFact);
					BYTE* pData = (BYTE*)GlobalLock(m_hGlobal);
					memcpy(pData, pBuf, dwSize);
					pData += dwSize;
					memcpy(pData, pBuffer, nFact);
					delete [] pBuf;
					bFirstRec = FALSE;
					bCompleted = FALSE;
					return;
				}
				else						//����ǰ��������ӵ������ܹ��������������ݰ��ṹ	
				{
					//��������ݰ��ṹ
					int nNeedPackage = nPackage - dwSize;
					BYTE* pPack = new BYTE[nPackage];
					memcpy(pPack, pBuf, dwSize);
					BYTE* pTmp = pPack;
					pTmp += dwSize;
					memcpy(pTmp, pBuffer, nNeedPackage);
					CPackage* pPackage = (CPackage*)pPack;
					//��ȡ���ݵĳ���
					int nDataLen = pPackage->m_dwContent;
					delete [] pPack;

					//�������ݰ�����
					if (nFact - nNeedPackage < nDataLen)	//pBuffer�а��������ݲ���һ�����������ݰ�����
					{
						m_hGlobal = GlobalAlloc(GHND, dwSize + nFact);
						BYTE* pData = (BYTE*)GlobalLock(m_hGlobal);
						memcpy(pData, pBuf, dwSize);
						pTmp = pData;
						pTmp += dwSize;
						memcpy(pTmp, pBuffer, nFact);
						GlobalUnlock(m_hGlobal);
						bCompleted = FALSE;
					}
					else	//pBuffer�а��������������ݰ����ݣ����ҿ��ܰ���������ݰ�
					{
						//�ȹ���һ�����������ݰ����д���
						m_hGlobal = GlobalAlloc(GHND, nPackage + nDataLen);
						BYTE* pData = (BYTE*)GlobalLock(m_hGlobal);
						memcpy(pData, pBuf, dwSize);
						pTmp = pData;
						pTmp += dwSize;
						memcpy(pTmp, pBuffer, nDataLen);

						pPackage = (CPackage*)pData;

						HandleRecData(pPackage);
			
						GlobalUnlock(m_hGlobal);
						GlobalFree(m_hGlobal);
						m_hGlobal = NULL;
						bCompleted = TRUE;

						//�����������ݰ�����
						//����ʣ�����ݵĴ�С
						int nLeaving = nFact - nNeedPackage - nDataLen;
						//��λ��pBuffer�е��������ݲ���
						pTmp = pBuffer + nNeedPackage + nDataLen;
						if (nLeaving == 0)
						{
							bCompleted = TRUE;
							bFirstRec = TRUE;							
						}
						while (nLeaving > 0)
						{
							bCompleted = FALSE;
							if (nLeaving > nPackage)	//���ݰ��������������ݰ��ṹ
							{
								CPackage* pPackage = (CPackage*) pTmp;
								//�����а���һ����һ�����ϵ����ݰ�
								if (nLeaving >= pPackage->m_dwContent + nPackage)
								{
									//�������ݰ�����
									HandleRecData(pPackage);

									nLeaving -= pPackage->m_dwContent + nPackage;
									if (nLeaving == 0)
									{
										bCompleted = TRUE;
										bFirstRec = TRUE;
									}
									pTmp += pPackage->m_dwContent + nPackage;
								}
								else	//�����а�����һ�����ݰ����ݲ�����
								{
									if (m_hGlobal != NULL)						//��m_hGlobal�д洢����
									{
										GlobalFree(m_hGlobal);
										m_hGlobal = NULL;

									}
									m_hGlobal = GlobalAlloc(GHND, nLeaving);
									void* pData = GlobalLock(m_hGlobal);
									memcpy(pData, pTmp, nLeaving);
									GlobalUnlock(m_hGlobal);
									nLeaving = 0;
									bCompleted = FALSE;
									GlobalUnlock(m_hGlobal);
									break;

								}
							}
							else		//���ݰ����в����������ݰ��ṹ	
							{
								if (m_hGlobal != NULL)	
								{
									GlobalFree(m_hGlobal);
									m_hGlobal = NULL;
								}
								m_hGlobal = GlobalAlloc(GHND, nLeaving);
								void* pData = GlobalLock(m_hGlobal);
								memcpy(pData, pTmp, nLeaving);
								nLeaving = 0;
								bCompleted = FALSE;
								GlobalUnlock(m_hGlobal);
								break;						
							}
						
						}
					
					}
				}
			}
			else	//(dwSize >= nPackage) ���ݰ��ṹ��������������
			{
				//��ȡ���ݰ��Ĵ�С���������ݰ��ṹ��С�����ݴ�С
				CPackage* pPackage = (CPackage*)pBuf;
				int nPackageSize = pPackage->m_dwContent + sizeof(CPackage);

				if (dwSize + nFact >= nPackageSize)	//֮ǰ���յ����������ڽ��յ������ܹ���������һ�����������ݰ�
				{
					//�����һ�����������ݰ����д���
					m_hGlobal = GlobalAlloc(GHND, nPackageSize);
					BYTE* pData = (BYTE*)GlobalLock(m_hGlobal);
					memcpy(pData, pBuf, dwSize);
					BYTE* pTmp = pData;
					pTmp += dwSize;
					int nNeed = nPackageSize - dwSize;
					memcpy(pTmp, pBuffer, nNeed);
					pPackage = (CPackage*)pData;
					//�������ݰ�����
					HandleRecData(pPackage);
					if (m_hGlobal != NULL)						//��m_hGlobal�д洢����
					{
						GlobalFree(m_hGlobal);
						m_hGlobal = NULL;
					}					

					bCompleted = TRUE;
					//����ʣ�������
					int nLeaving = nFact - nNeed;
					pTmp = pBuffer + nNeed;
				
					while (nLeaving > 0)
					{
						bCompleted = FALSE;
						if (nLeaving > nPackage)	//���ݰ��������������ݰ��ṹ
						{
							CPackage* pPackage = (CPackage*) pTmp;
							//�����а���һ����һ�����ϵ����ݰ�
							if (nLeaving >= pPackage->m_dwContent + nPackage)
							{
								HandleRecData(pPackage);
	
								nLeaving -= pPackage->m_dwContent + nPackage;
								if (nLeaving == 0)	
								{
									bCompleted = TRUE;
								}
								pTmp += pPackage->m_dwContent + nPackage;
							}
							else	//�����а�����һ�����ݰ����ݲ�����
							{
								if (m_hGlobal != NULL)						//��m_hGlobal�д洢����
								{
									GlobalFree(m_hGlobal);
									m_hGlobal = NULL;
								}
								m_hGlobal = GlobalAlloc(GHND, nLeaving);
								void* pData = GlobalLock(m_hGlobal);
								memcpy(pData, pTmp, nLeaving);
								GlobalUnlock(m_hGlobal);
								nLeaving = 0;
								bCompleted = FALSE;
								GlobalUnlock(m_hGlobal);
								break;

							}
						}
						else		//���ݰ����в����������ݰ��ṹ	
						{
							if (m_hGlobal != NULL)						//��m_hGlobal�д洢����
							{
								GlobalFree(m_hGlobal);
								m_hGlobal = NULL;
							}
							m_hGlobal = GlobalAlloc(GHND, nLeaving);
							void* pData = GlobalLock(m_hGlobal);
							memcpy(pData, pTmp, nLeaving);
							nLeaving = 0;
							bCompleted = FALSE;
							GlobalUnlock(m_hGlobal);
							break;						
						}
					}
				}
				else	//֮ǰ���յ����������ڽ��յ����ݲ��ܹ���һ�����������ݰ�������Ҫ������������
				{
					if (m_hGlobal != NULL)						//��m_hGlobal�д洢����
					{
						GlobalFree(m_hGlobal);
					}
					m_hGlobal = GlobalAlloc(GHND, dwSize + nFact);
					BYTE* pData = (BYTE*)GlobalLock(m_hGlobal);
					memcpy(pData, pBuf, dwSize);
					BYTE* pTmp = pData;
					pTmp += dwSize;
					memcpy(pTmp, pBuffer, nFact);
					bCompleted = FALSE;
					GlobalUnlock(m_hGlobal);					
				
				}
	
			}
			delete [] pBuf;
		}
	}
//	bFirstRec = FALSE;
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
}

//���ӷ�����
void CClientDlg::OnConnect() 
{
	CString szIP, szPort;
	m_ServerIP.GetWindowText(szIP);
	m_Port.GetWindowText(szPort);
	if (szIP.IsEmpty() || szPort.IsEmpty())
	{
		MessageBox("�����÷�����IP�Ͷ˿ں�");
		return;
	}
	int nPort = atoi(szPort.GetBuffer(0));
	szPort.ReleaseBuffer();
	
	BOOL bRet = m_ClientSock.Connect(szIP, nPort);
	if (!bRet)
	{
		MessageBox("���ӷ�����ʧ��!");
	}
}

BOOL CClientDlg::InitSocket()
{
	return m_ClientSock.Create();
}

//������յ�����
void CClientDlg::HandleRecData(CPackage *pPackage)
{
	if (pPackage != NULL)
	{
		if (pPackage->m_Type==ptText)	//�ı�����
		{
			char* szData = (char* )pPackage->m_Data;
			m_InfoList.SetSel(-1, 0);
			m_InfoList.ReplaceSel("������say: \n");
			m_InfoList.SetSel(-1, 0);
			m_InfoList.ReplaceSel(szData);
			m_InfoList.SetSel(-1, 0);
			m_InfoList.ReplaceSel("\n");	//��ӻ��з�
		}
		else if (pPackage->m_Type==ptImage)	//ͼ������
		{
			BITMAPINFOHEADER bmpHeader;
			BITMAPINFO bmpInfo;
			bmpInfo.bmiHeader = bmpHeader;
			memcpy(&bmpHeader, pPackage->m_Data, sizeof(bmpHeader));
			BYTE* pBmpData = pPackage->m_Data;
			pBmpData += sizeof(bmpHeader);
			CDC* pDC = GetDC();
			HBITMAP hBmp = CreateDIBitmap(pDC->m_hDC, &bmpHeader, CBM_INIT, pBmpData, (BITMAPINFO*)&bmpHeader, DIB_RGB_COLORS);
			HBITMAP hOldBmp = m_Image.SetBitmap(hBmp);
			if (hOldBmp != NULL)
			{
				DeleteObject(hOldBmp);
			}
			//��ʾͼ��
			//...
			ReleaseDC(pDC);
		}		
	
	}
}

void CClientDlg::OnSend() 
{
	CString szSendInfo;
	m_SendContent.GetWindowText(szSendInfo);
	if (!szSendInfo.IsEmpty())
	{
		//������ݰ�
		int nLen = szSendInfo.GetLength();
		HGLOBAL hGlobal = GlobalAlloc(GHND, sizeof(CPackage) + nLen);
		BYTE* pData = (BYTE*)GlobalLock(hGlobal);
		CPackage *pPackage = (CPackage*) pData;
		pPackage->m_Type = ptText;
		pPackage->m_dwContent = nLen;
		pPackage->m_dwSize = sizeof(CPackage);
		pPackage->m_dwData = nLen;
		BYTE* pTmp = pData;
		pTmp += sizeof(CPackage);
		memcpy(pTmp, szSendInfo, nLen);
		m_ClientSock.Send(pData, sizeof(CPackage) + nLen);

		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
		m_SendContent.SetWindowText("");
		m_InfoList.SetSel(-1, 0);
		m_InfoList.ReplaceSel("�ͻ���say: \n");
		m_InfoList.SetSel(-1, 0);
		m_InfoList.ReplaceSel(szSendInfo);
		m_InfoList.SetSel(-1, 0);
		m_InfoList.ReplaceSel("\n");
	}		
}
