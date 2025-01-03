﻿
// MainDlg.h: 헤더 파일
//

#pragma once
#include <iostream>
#include <vector>

// CMainDlg 대화 상자
class CMainDlg : public CDialogEx
{
// 생성입니다.
public:
	CMainDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	bool GetConfValues();
	void WriteLog(std::string _strLog);

public:
	std::string m_strErrorMsg = "";
	int m_nImageCnt;
	int m_nKernel_size;
	std::vector<std::string> m_vecInputPath;
	std::vector<std::string> m_vecOpencvPath;
	std::vector<std::string> m_vecCustomPath;
};


