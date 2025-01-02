
// MainDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Main.h"
#include "MainDlg.h"
#include "afxdialogex.h"

#include "ImageObject/ImageObject.h"
#include "OpencvDLL/OpencvDLL.h"
#include "CustomDLL/CustomDLL.h"

#include <fstream>
#include <time.h>
#include <windows.h>
#include <psapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainDlg 대화 상자
CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMainDlg 메시지 처리기

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMainDlg::OnPaint()
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
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainDlg::OnBnClickedButton1()
{
	if (!GetConfValues())
		return;
	
	for (int i = 0; i < m_nImageCnt; i++)
	{
		ImageObject* src = new ImageObject(m_vecInputPath.at(i));
		ImageObject* dst_opencv = new ImageObject(m_vecOpencvPath.at(i));
		ImageObject* dst_custom = new ImageObject(m_vecCustomPath.at(i));

		if (!src->ImageLoad())
		{
			continue;
		}
		
		clock_t start_opencv = clock();
		WriteLog("[OpencvDLL] Blur Start");
		if (!OpencvDLL::ImageBlur(src, dst_opencv, m_nKernel_size))
		{
			WriteLog("[OpencvDLL] Blur Error");
			return;
		}
		else
		{
			clock_t end = clock();
			double duration = (double)(end - start_opencv) / CLOCKS_PER_SEC;
			CString log;
			log.Format(_T("[OpencvDLL] Blur Done (Time: %f)"), duration);
			WriteLog(std::string(CT2CA(log)));

			dst_opencv->ImageSave();
		}

		clock_t start_custom = clock();
		WriteLog("[CustomDLL] Blur Start");
		if (!CustomDLL::ImageBlur(src, dst_custom, m_nKernel_size))
		{
			WriteLog("[CustomDLL] Blur Error");
			return;
		}
		else
		{
			clock_t end = clock();
			double duration = (double)(end - start_custom) / CLOCKS_PER_SEC;
			CString log;
			log.Format(_T("[CustomDLL] Blur Done (Time: %f)"), duration);
			WriteLog(std::string(CT2CA(log)));

			dst_custom->ImageSave();
		}

		delete src;
		delete dst_opencv;
		delete dst_custom;
	}
}

bool CMainDlg::GetConfValues()
{
	m_nKernel_size = 0;
	m_vecInputPath.clear();
	m_vecOpencvPath.clear();
	m_vecCustomPath.clear();

	TCHAR buffer[4096];
	GetPrivateProfileString(_T("INPUT"), _T("kernel_size"), _T("21"), buffer, sizeof(buffer), _T("./Conf.ini"));
	m_nKernel_size = _ttoi(buffer);
	if (m_nKernel_size < 21)
		m_nKernel_size = 21;

	int cnt = 1;
	while (true)
	{
		CString key;
		key.Format(_T("%d"), cnt);
		GetPrivateProfileString(_T("INPUT"), key, _T("error"), buffer, sizeof(buffer), _T("./Conf.ini"));
		CString path(buffer);
		if (path == _T("error"))
			break;
		std::string str = std::string(CT2CA(path));
		m_vecInputPath.push_back(str);
		cnt++;
	}

	cnt = 1;
	while (true)
	{
		CString key;
		key.Format(_T("%d"), cnt);
		GetPrivateProfileString(_T("OUTPUT_OPENCV"), (LPCTSTR)key, _T("error"), buffer, sizeof(buffer), _T("./Conf.ini"));
		CString path(buffer);
		if (path == _T("error"))
			break;
		std::string str = std::string(CT2CA(path));
		m_vecOpencvPath.push_back(str);
		cnt++;
	}

	cnt = 1;
	while (true)
	{
		CString key;
		key.Format(_T("%d"), cnt);
		GetPrivateProfileString(_T("OUTPUT_CUSTOM"), (LPCTSTR)key, _T("error"), buffer, sizeof(buffer), _T("./Conf.ini"));
		CString path(buffer);
		if (path == _T("error"))
			break;
		std::string str = std::string(CT2CA(path));
		m_vecCustomPath.push_back(str);
		cnt++;
	}


	if (m_vecInputPath.size() < 1 || (m_vecInputPath.size() != m_vecOpencvPath.size()) || (m_vecInputPath.size() != m_vecCustomPath.size()))
	{
		WriteLog("[ERROR] Check Conf File. (Check the path)");
		return false;
	}

	m_nImageCnt = m_vecInputPath.size();
	
	return true;
}

void CMainDlg::WriteLog(std::string _strLog)
{
	auto now = std::chrono::system_clock::now();
	std::time_t current_time = std::chrono::system_clock::to_time_t(now);
	std::tm tm_time= *std::localtime(&current_time);

	std::ofstream file("image_blur.log", std::ios_base::app);
	if (!file)	return;

	file << std::put_time(&tm_time, "[%Y-%m-%d %H:%M:%S]") << " - " << _strLog << std::endl;
	std::cout << std::put_time(&tm_time, "[%Y-%m-%d %H:%M:%S]") << " - " << _strLog << std::endl;
}