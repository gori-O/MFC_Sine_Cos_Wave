
// MFC_SIN_COSDlg.h: 헤더 파일
//

#pragma once


// CMFCSINCOSDlg 대화 상자
class CMFCSINCOSDlg : public CDialogEx
{
private:
	CImage m_image;						// wave 그릴 바탕이미지
	CDC m_image_dc;						// 이미지에 그림 그릴 DC (가상 DC) 로 여기에 실제로 그린다. 그 후에 paint() 에서 dc에 이 가상dc를 대입해 화면에 출력함
	CRect m_rect;						// 
	POINT m_center_pos;					// 수학에서 0,0 을 가운데 나타내는데. MFC는 다르다. 이를 보정해주기 위한 좌표값을 저장할 변수


	CPen m_grid_pen, m_sine_pen, m_cos_pen, m_circle_pen;			// 그릴 때 사용할 펜
	CBrush m_sine_brush, m_cos_brush,m_circle_brush;				// 그릴 때 사용할 브러쉬(동그란 원)
	int m_step = 1, m_sin_x, m_sin_y, m_cos_x, m_cos_y;				// m_step = 
	
// 생성입니다.
public:
	CMFCSINCOSDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_SIN_COS_DIALOG };
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
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ShowGrid();
	void ShowSine();
	void ShowCos();
	void ShowCircle();
};
