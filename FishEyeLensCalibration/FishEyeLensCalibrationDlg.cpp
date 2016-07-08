
// FishEyeLensCalibrationDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "FishEyeLensCalibration.h"
#include "FishEyeLensCalibrationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* ���� ���� */
WebCam** webCamList = new WebCam*();
int nWebCamCnt = 0;
int nWidth = 640;
int nHeight = 480;
int nImgCnt = 0;
bool DoWork = true;

/* �Լ� ���� */
UINT Capture(LPVOID pParam);
char* ConvertCString2CharPointer(CString str);

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFishEyeLensCalibrationDlg ��ȭ ����



CFishEyeLensCalibrationDlg::CFishEyeLensCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FISHEYELENSCALIBRATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFishEyeLensCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFishEyeLensCalibrationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CFishEyeLensCalibrationDlg::OnBnClickedButtonStart)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, &CFishEyeLensCalibrationDlg::OnBnClickedButtonCalibration)
END_MESSAGE_MAP()


// CFishEyeLensCalibrationDlg �޽��� ó����

BOOL CFishEyeLensCalibrationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CFishEyeLensCalibrationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CFishEyeLensCalibrationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CFishEyeLensCalibrationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFishEyeLensCalibrationDlg::OnBnClickedButtonStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	StartCapture();
}

void CFishEyeLensCalibrationDlg::StartCapture()
{
	/* ���ڿ� ��ȯ */
	CString str;

	str.Format(_T("window[%d]"), nWebCamCnt);

	/* ���ڿ� ��ȯ �Ϸ� */

	WebCam * webCam = new WebCam();
	webCam->nCameraIndex = nWebCamCnt;
	webCam->windowName = ConvertCString2CharPointer(str);

	webCamList[nWebCamCnt] = webCam;

	CWinThread* pThread0 = AfxBeginThread(Capture, webCam);

	nWebCamCnt++;

	SetTimer(1394, 100, NULL);
}

char* ConvertCString2CharPointer(CString str)
{
	wchar_t* wchar_str;
	char * char_str;
	int char_str_len;

	wchar_str = str.GetBuffer(str.GetLength());
	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	char_str = new char[char_str_len];
	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0, 0);

	return char_str;
}



UINT Capture(LPVOID pParam)
{
	WebCam* wc = (WebCam*)pParam;
	wc->vc = new VideoCapture(wc->nCameraIndex);

	while (DoWork)
	{
		*wc->vc >> *wc->imageSource;

		if (wc->imageSource->empty())
		{
			break;
		}

		imshow(wc->windowName, *wc->imageSource);

		char ch = waitKey(10);

		if (ch == 27)
		{
			break; 
		}

		else if (ch == 32)
		{
			CString str;

			str.Format(_T("%d.jpg"), nImgCnt);

			nImgCnt++;

			imwrite(ConvertCString2CharPointer(str), *wc->imageSource);
		}
	}

	return 0;
}

void CFishEyeLensCalibrationDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//CDialogEx::OnClose();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	DoWork = false;
	CDialogEx::OnClose();

	DWORD dwExitCode;
	DWORD dwPID = GetCurrentProcessId();    // ���� �ڽ��� ���μ��� ID ��������.

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPID);    // ���μ��� �ڵ� ��������

	if (NULL != hProcess)
	{
		GetExitCodeProcess(hProcess, &dwExitCode);   // ���μ��� ������ �ڵ� ������
		TerminateProcess(hProcess, dwExitCode);    // ���μ��� ���� ����
		WaitForSingleObject(hProcess, INFINITE); // ���� �ɶ����� ���
		CloseHandle(hProcess);                                 // ���μ��� �ڵ� �ݱ�
	}
}

//std::vector<std::vector<cv::Point2d> > imagePoints(52);
//std::vector<std::vector<cv::Point3d> > objectPoints(52);
//int saveImgCnt = 0;

void CFishEyeLensCalibrationDlg::OnBnClickedButtonCalibration()
{
	CFileFind file;
	bool result = file.FindFile(_T("*.jpg"));

	//cv::fisheye::stereoCalibrate()

	while (result)
	{
		result = file.FindNextFileW();

		if (file.IsDots()) continue;

		CString fileExtension = file.GetFileName();
	}
}
