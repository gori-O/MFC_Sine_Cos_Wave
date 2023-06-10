
// MFC_SIN_COSDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_SIN_COS.h"
#include "MFC_SIN_COSDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSINCOSDlg 대화 상자



CMFCSINCOSDlg::CMFCSINCOSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_SIN_COS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSINCOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCSINCOSDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFCSINCOSDlg 메시지 처리기

BOOL CMFCSINCOSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	GetClientRect(m_rect);
	m_center_pos = m_rect.CenterPoint();

	m_image.Create(m_rect.Width(), m_rect.Height(), 32, 0);
	m_image_dc.Attach(m_image.GetDC());

	SetBackgroundColor(RGB(0, 0, 0));

	m_grid_pen.CreatePen(PS_DOT, 1, RGB(168, 168, 168));
	m_sine_pen.CreatePen(PS_SOLID, 2, RGB(0, 200, 255));
	m_cos_pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 100));
	m_circle_pen.CreatePen(PS_SOLID, 2, RGB(255, 255, 100));

	m_sine_brush.CreateSolidBrush(RGB(0, 100,200));
	m_cos_brush.CreateSolidBrush(RGB(0, 200, 100));
	m_circle_brush.CreateSolidBrush(RGB(200, 200, 0));

	SetTimer(1, 10, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

#include <math.h>
#define PI		3.141592

void CMFCSINCOSDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
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
		m_image.Draw(dc, 0, 0);
		//CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCSINCOSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCSINCOSDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
	m_grid_pen.DeleteObject();
	m_sine_pen.DeleteObject();
	m_circle_pen.DeleteObject();

	m_image_dc.Detach();
	m_image.ReleaseDC();
}


void CMFCSINCOSDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		if (m_step < 720) { m_step+=2; }				// 움직이는 속도 m_step 으로 조절
		else { m_step = 1; }

		m_image_dc.FillSolidRect(m_rect, RGB(0, 0, 0));

		ShowGrid();
		ShowSine();
		ShowCos();

		m_image_dc.SelectStockObject(WHITE_PEN);
		m_image_dc.MoveTo(m_sin_x, m_sin_y);
		m_image_dc.LineTo(m_cos_x, m_sin_y);

		m_image_dc.MoveTo(m_cos_x, m_cos_y);
		m_image_dc.LineTo(m_cos_x, m_sin_y);
		
		ShowCircle();
		

		Invalidate(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMFCSINCOSDlg::ShowGrid()
{
	m_image_dc.SelectObject(&m_grid_pen);

	m_image_dc.MoveTo(m_center_pos.x, 0);
	m_image_dc.LineTo(m_center_pos.x, m_rect.bottom);

	m_image_dc.MoveTo(0, m_center_pos.y);
	m_image_dc.LineTo(m_rect.right, m_center_pos.y);
}


void CMFCSINCOSDlg::ShowSine()
{
	m_image_dc.SelectObject(&m_sine_pen);
	int degreee, x, y, fix = m_center_pos.x - 360;
	double radian;

	//for (x = 0; x < m_rect.right; x++) {
	for (x = 0; x < m_step; x++) {
		degreee = x -360;
		radian = degreee * PI / 180;
		y = -(int)(sin(radian) * 100) + m_center_pos.y;

		if (x) { m_image_dc.LineTo(x+fix, y); }
		else { m_image_dc.MoveTo(x+fix, y); }
	}
	//x += fix;
	m_sin_x = x+fix;
	m_sin_y = y;
	m_image_dc.SelectObject(&m_sine_brush);
	m_image_dc.Ellipse(x+fix - 20, y - 20, x+fix + 20, y + 20);
}


void CMFCSINCOSDlg::ShowCos()
{
	m_image_dc.SelectObject(&m_cos_pen);
	int degreee, x, y, fix = m_center_pos.y - 360;
	double radian;

	for (y = 0; y < m_step; y++) {
		degreee = y - 360;
		radian = degreee * PI / 180;
		x = (int)(cos(radian) * 100) + m_center_pos.x;

		if (y) { m_image_dc.LineTo(x,y+fix); }
		else { m_image_dc.MoveTo(x,y+fix); }
	}
	//y += fix;
	m_cos_x = x;
	m_cos_y = y+fix;

	m_image_dc.SelectObject(&m_cos_brush);
	m_image_dc.Ellipse(x - 20, y+fix - 20, x + 20, y+fix + 20);
}


void CMFCSINCOSDlg::ShowCircle()
{

	m_image_dc.SelectObject(&m_circle_pen);
	m_image_dc.SelectStockObject(NULL_BRUSH);
	m_image_dc.Ellipse(m_center_pos.x - 100, m_center_pos.y - 100, m_center_pos.x + 100, m_center_pos.y + 100);
	m_image_dc.SelectObject(&m_circle_brush);
	m_image_dc.Ellipse(m_cos_x - 20, m_sin_y - 20, m_cos_x + 20, m_sin_y + 20);
}
