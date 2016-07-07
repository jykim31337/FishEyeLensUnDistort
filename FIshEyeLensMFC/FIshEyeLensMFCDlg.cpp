
// FIshEyeLensMFCDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "FIshEyeLensMFC.h"
#include "FIshEyeLensMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* 변수 선언 */
WebCam** webCamList = new WebCam*();
int nWebCamCnt = 0;
int nWidth = 640;
int nHeight = 480;
bool DoWork = true;

/* 함수 선언 */
UINT Capture(LPVOID pParam);
char* ConvertCString2CharPointer(CString str);

/* 왜곡 보정 */
double cx, cy, fx, fy, k1, k2, k3, p1, p2, skew_c = 0;
void DistortPixel(int& px, int& py);
void Normalize(double &x, double& y);
void Denormalize(double &x, double& y);
void Distort(double& x, double& y);
void InitParam();
void cvtImage(WebCam* iw);
int sizeRatio = 2;
double undistortFactor = 0;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CFIshEyeLensMFCDlg 대화 상자



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


// CFIshEyeLensMFCDlg 메시지 처리기

BOOL CFIshEyeLensMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//초기화
	InitParam();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFIshEyeLensMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
	/* 문자열 변환 */
	CString str;

	str.Format(_T("window[%d]"), nWebCamCnt);

	/* 문자열 변환 완료 */

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
	//// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CDialogEx::OnClose();
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DoWork = false;
	CDialogEx::OnClose();

	DWORD dwExitCode;
	DWORD dwPID = GetCurrentProcessId();    // 현재 자신의 프로세스 ID 가져오기.

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPID);    // 프로세스 핸들 가져오기

	if (NULL != hProcess)
	{
		GetExitCodeProcess(hProcess, &dwExitCode);   // 프로세스 나가기 코드 얻어오기
		TerminateProcess(hProcess, dwExitCode);    // 프로세스 연결 끊기
		WaitForSingleObject(hProcess, INFINITE); // 종료 될때까지 대기
		CloseHandle(hProcess);                                 // 프로세스 핸들 닫기
	}
}


void CFIshEyeLensMFCDlg::OnChangeEditFactor()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData();

	undistortFactor = _ttof(m_FACTOR);

	InitParam();
}
