
// FIshEyeLensMFCDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "FIshEyeLensMFC.h"
#include "FIshEyeLensMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* ���� ���� */
WebCam** webCamList = new WebCam*();
int nWebCamCnt = 0;
int nWidth = 640;
int nHeight = 480;
bool DoWork = true;

/* �Լ� ���� */
UINT Capture(LPVOID pParam);
char* ConvertCString2CharPointer(CString str);

/* �ְ� ���� */
double cx, cy, fx, fy, k1, k2, k3, p1, p2, skew_c = 0;
void DistortPixel(int& px, int& py);
void Normalize(double &x, double& y);
void Denormalize(double &x, double& y);
void Distort(double& x, double& y);
void InitParam();
void cvtImage(WebCam* iw);
int sizeRatio = 2;
double undistortFactor = 0;

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


// CFIshEyeLensMFCDlg ��ȭ ����



CFIshEyeLensMFCDlg::CFIshEyeLensMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FISHEYELENSMFC_DIALOG, pParent)
	, m_FACTOR(_T("0"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFIshEyeLensMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT_FACTOR, FACTOR);
	DDX_Text(pDX, IDC_EDIT_FACTOR, m_FACTOR);
}

BEGIN_MESSAGE_MAP(CFIshEyeLensMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START_WEBCAM, &CFIshEyeLensMFCDlg::OnBnClickedButtonStartWebcam)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_FACTOR, &CFIshEyeLensMFCDlg::OnChangeEditFactor)
END_MESSAGE_MAP()


// CFIshEyeLensMFCDlg �޽��� ó����

BOOL CFIshEyeLensMFCDlg::OnInitDialog()
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

	//�ʱ�ȭ
	InitParam();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CFIshEyeLensMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFIshEyeLensMFCDlg::OnPaint()
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
HCURSOR CFIshEyeLensMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFIshEyeLensMFCDlg::OnBnClickedButtonStartWebcam()
{
	StartCapture();
}

void CFIshEyeLensMFCDlg::StartCapture()
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

		*wc->imageConvert = Mat::zeros(Size(wc->imageSource->cols * sizeRatio, wc->imageSource->rows * sizeRatio), CV_8UC3);
		*wc->imageFishEye = Mat::zeros(wc->imageConvert->size(), CV_8UC3);

		cvtImage(wc);

		int h = wc->imageConvert->rows;
		int w = wc->imageConvert->cols;

		for (int y = 0; y<h; y++)
		{
			for (int x = 0; x<w; x++)
			{
				int px = x;
				int py = y;
				DistortPixel(px, py);

				if (px >= 0 && py >= 0 && px<w && py<h)
				{
					wc->imageFishEye->data[(y * wc->imageConvert->cols * 3) + (x * 3) + 2] = wc->imageConvert->data[(py * wc->imageConvert->cols * 3) + (px * 3) + 2];
					wc->imageFishEye->data[(y * wc->imageConvert->cols * 3) + (x * 3) + 1] = wc->imageConvert->data[(py * wc->imageConvert->cols * 3) + (px * 3) + 1];
					wc->imageFishEye->data[(y * wc->imageConvert->cols * 3) + (x * 3) + 0] = wc->imageConvert->data[(py * wc->imageConvert->cols * 3) + (px * 3) + 0];
				}
			}
		}

		imshow(wc->windowName, *wc->imageFishEye);

		if (waitKey(10) == 27)
		{
			break; // EXC
		}
	}

	return 0;
}

/*FishEye Start*/

void InitParam()
{
	fx = 214.885 * sizeRatio;
	fy = 214.909 * sizeRatio;
	cx = 318.885 * sizeRatio;
	cy = 238.485 * sizeRatio;

	
	k1 = -0.18894 * undistortFactor;
	k2 = 0.02724 * undistortFactor;
	k3 = 0 * undistortFactor;
	p1 = 0.000787 * undistortFactor;
	p2 = -0.003942 * undistortFactor;
}

void DistortPixel(int& px, int& py)
{
	double x = px;
	double y = py;

	Normalize(x, y);
	Distort(x, y);
	Denormalize(x, y);

	px = (int)(x + 0.5);
	py = (int)(y + 0.5);
}

void Normalize(double &x, double& y)
{
	double y_n = (y - cy) / fy;
	double x_n = (x - cx) / fx - skew_c*y_n;

	x = x_n;
	y = y_n;
}

void Denormalize(double &x, double& y)
{
	double x_p = fx*(x + skew_c*y) + cx;
	double y_p = fy*y + cy;

	x = x_p;
	y = y_p;
}

void Distort(double& x, double& y)
{
	double r2 = x*x + y*y;
	double radial_d = 1 + k1*r2 + k2*r2*r2 + k3*r2*r2*r2;
	double x_d = radial_d*x + 2 * p1*x*y + p2*(r2 + 2 * x*x);
	double y_d = radial_d*y + p1*(r2 + 2 * y*y) + 2 * p2*x*y;

	x = x_d;
	y = y_d;
}

void cvtImage(WebCam* webCam)
{
	int margin = 2;

	for (int y = 0; y < webCam->imageSource->rows; y++)
	{
		for (int x = 0; x < webCam->imageSource->cols; x++)
		{
			//int margin = 1;
			webCam->imageConvert->data[((y + webCam->imageSource->rows / margin) * webCam->imageConvert->cols * 3) + ((x + webCam->imageSource->cols / margin) * 3) + 2] = webCam->imageSource->data[(y * nWidth * 3) + (x * 3) + 2];

			webCam->imageConvert->data[((y + webCam->imageSource->rows / margin) * webCam->imageConvert->cols * 3) + ((x + webCam->imageSource->cols / margin) * 3) + 1] = webCam->imageSource->data[(y * nWidth * 3) + (x * 3) + 1];

			webCam->imageConvert->data[((y + webCam->imageSource->rows / margin) * webCam->imageConvert->cols * 3) + ((x + webCam->imageSource->cols / margin) * 3) + 0] = webCam->imageSource->data[(y * nWidth * 3) + (x * 3) + 0];
		}
	}
}
/*FishEye End*/


void CFIshEyeLensMFCDlg::OnClose()
{
	//// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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


void CFIshEyeLensMFCDlg::OnChangeEditFactor()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData();

	undistortFactor = _ttof(m_FACTOR);

	InitParam();
}
