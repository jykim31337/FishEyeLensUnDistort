
// FIshEyeLensMFCDlg.h : ��� ����
//

#pragma once

typedef struct WebCam
{
	VideoCapture * vc = 0;
	byte nCameraIndex = 0;
	char* windowName = 0;
	Mat* imageSource = new Mat();
};

// CFIshEyeLensMFCDlg ��ȭ ����
class CFIshEyeLensMFCDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CFIshEyeLensMFCDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FISHEYELENSMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartWebcam();
};