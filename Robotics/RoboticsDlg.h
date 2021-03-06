
// RoboticsDlg.h: 헤더 파일
//
#include <queue>
#include <stack>
#include <array>
using namespace std;

#pragma once
// CRoboticsDlg 대화 상자
class CRoboticsDlg : public CDialogEx
{
// 생성입니다.
public:
	CRoboticsDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBOTICS_DIALOG };
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
	BOOL PreTranslateMessage(MSG * pMsg);
	DECLARE_MESSAGE_MAP()
public:
	int maze[30][30];
	int check[30][30];
	int cost[30][30];
	double m_interval;
	CComboBox m_up;
	CComboBox m_down;
	CComboBox m_left;
	CComboBox m_right;
	CComboBox m_row;
	CComboBox m_column;
	CString row, column, up, down, left, right;
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>direction;
	int countExpand = 0, countCost = 0;
	int w, h;
	int x, y, old_x, old_y;
	int G = 0, H, F;
	int startX, startY, endX, endY;
	int x_pos[4] = { 0, 1, 0, -1 };
	int y_pos[4] = { 1, 0, -1, 0 };
	int m_dragFlag = 0;
	CPoint m_dragPos, m_saveDragPos;
	int m_radio;
	queue<tuple<int, int, vector<pair<int, int>>>>q;
	stack<tuple<int, int, vector<pair<int, int>>>>s;
	priority_queue<tuple<int, int, int, int, vector<pair<int, int>>>, vector<tuple<int, int, int, int, vector<pair<int, int>>>>, greater<tuple<int, int, int, int, vector<pair<int, int>>>>>pqGreedy;
	priority_queue<tuple<int, int, int, int, int, vector<pair<int, int>>>, vector<tuple<int, int, int, int, int, vector<pair<int, int>>>>, greater<tuple<int, int, int, int, int, vector<pair<int, int>>>>>pq;
	vector<pair<int, int>>pt;
	bool oneStep;
	bool end = false;
	CFont m_font;
	CSliderCtrl m_slider;
	CButton m_ckSmoothing;
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonBuild();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonImmediately();
	afx_msg void OnBnClickedButtonStepbystep();
	afx_msg void OnBnClickedButtonAction();
	afx_msg void OnBnClickedCheckSmoothing();
	void Activation(bool b);
	void DrawCell(DWORD color, int x, int y);
	void Reset();
	void DrawInt(int value, int x, int y);
	int HeuristicDistance(int x, int y);
	void Complete(tuple<int, int, vector<pair<int, int>>> temp);
	void operationSequence();
	void Smoothing();
};
