
// FIshEyeLensMFC.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CFIshEyeLensMFCApp:
// �� Ŭ������ ������ ���ؼ��� FIshEyeLensMFC.cpp�� �����Ͻʽÿ�.
//

class CFIshEyeLensMFCApp : public CWinApp
{
public:
	CFIshEyeLensMFCApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CFIshEyeLensMFCApp theApp;