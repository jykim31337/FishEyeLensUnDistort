
// FIshEyeLensMFCDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"

typedef struct WebCam
{
	VideoCapture * vc = 0;
	byte nCameraIndex = 0;
	char* windowName = 0;
	Mat* imageSource = new Mat();
	Mat* imageConvert = new Mat();
	Mat* imageFishEye = new Mat();
};

// CFIshEyeLensMFCDlg 대화 상자
class CFIshEyeLensMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	CFIshEyeLensMFCDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FISHEYELENSMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartWebcam();

	/* 사용자 함수 선언 */
private:
	void CFIshEyeLensMFCDlg::StartCapture();
public:
	afx_msg void OnClose();
//	CEdit FACTOR;
	afx_msg void OnChangeEditFactor();
	CString m_FACTOR;
};
