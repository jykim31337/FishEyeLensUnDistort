
// FishEyeLensCalibration.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CFishEyeLensCalibrationApp:
// �� Ŭ������ ������ ���ؼ��� FishEyeLensCalibration.cpp�� �����Ͻʽÿ�.
//

class CFishEyeLensCalibrationApp : public CWinApp
{
public:
	CFishEyeLensCalibrationApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CFishEyeLensCalibrationApp theApp;