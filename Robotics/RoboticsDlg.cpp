#include "stdafx.h"
#include "Robotics.h"
#include "RoboticsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CRoboticsDlg 대화 상자



CRoboticsDlg::CRoboticsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROBOTICS_DIALOG, pParent)
	, m_radio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(maze, 0, sizeof(maze));
}

void CRoboticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROW, m_row);
	DDX_Control(pDX, IDC_COMBO_COLUMN, m_column);
	DDX_Radio(pDX, IDC_RADIO_BFS, m_radio);
	DDX_Control(pDX, IDC_COMBO_UP, m_up);
	DDX_Control(pDX, IDC_COMBO_DOWN, m_down);
	DDX_Control(pDX, IDC_COMBO_LEFT, m_left);
	DDX_Control(pDX, IDC_COMBO_RIGHT, m_right);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Control(pDX, IDC_CHECK_SMOOTHING, m_ckSmoothing);
}

BEGIN_MESSAGE_MAP(CRoboticsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_BUILD, &CRoboticsDlg::OnBnClickedButtonBuild)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_STEPBYSTEP, &CRoboticsDlg::OnBnClickedButtonStepbystep)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CRoboticsDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_IMMEDIATELY, &CRoboticsDlg::OnBnClickedButtonImmediately)
	ON_BN_CLICKED(IDC_BUTTON_ACTION, &CRoboticsDlg::OnBnClickedButtonAction)
	ON_BN_CLICKED(IDC_CHECK_SMOOTHING, &CRoboticsDlg::OnBnClickedCheckSmoothing)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRoboticsDlg 메시지 처리기

BOOL CRoboticsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	for (int i = 5; i < 31; i++)	// row, column 리스트 박스
	{
		CString str;
		str.Format(_T("%d"), i);
		m_row.AddString(str);
		m_column.AddString(str);
	}
	for (int i = 1; i < 5; i++)		// up, down, left, right 리스트 박스
	{
		CString str;
		str.Format(_T("%d"), i);
		m_up.AddString(str);
		m_down.AddString(str);
		m_left.AddString(str);
		m_right.AddString(str);
	}

	m_row.SelectString(0, "5");
	m_column.SelectString(0, "5");
	m_up.SelectString(2, "3");
	m_down.SelectString(0, "1");
	m_left.SelectString(3, "4");
	m_right.SelectString(1, "2");
	m_slider.SetRange(0, 1000);

	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_IMMEDIATELY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STEPBYSTEP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ACTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SMOOTHING)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_LEFT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_RIGHT)->EnableWindow(FALSE);

	return TRUE;
}

void CRoboticsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRoboticsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
	}
}

HCURSOR CRoboticsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CRoboticsDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CRoboticsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)	// 오른쪽아래 Static Text
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	m_font.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, _T("HY견고딕"));
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_START)
	{
		pDC->SelectObject(m_font);
		pDC->SetTextColor(RGB(36, 223, 5));
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_END)
	{
		pDC->SelectObject(m_font);
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_EXPAND)
	{
		pDC->SelectObject(m_font);
		pDC->SetTextColor(RGB(0, 162, 232));
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_PATH)
	{
		pDC->SelectObject(m_font);
		pDC->SetTextColor(RGB(255, 192, 0));
	}
	return hbr;
}

void CRoboticsDlg::OnBnClickedButtonBuild()	// Build 버튼 클릭시 맵 생성
{
	Reset();
	memset(maze, 0, sizeof(maze));

	m_row.GetLBText(m_row.GetCurSel(), row);
	m_column.GetLBText(m_column.GetCurSel(), column);

	h = _ttoi(row);
	w = _ttoi(column);

	if (h > w)
		m_interval = 450 / h;
	else
		m_interval = 450 / w;

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			if (x == 1 && y == 1)
			{
				maze[y][x] = 2;	// 출발지
				startY = y;
				startX = x;
				DrawCell(RGB(36, 223, 5), x, y);
			}
			else if (x == w - 2 && y == h - 2)
			{
				maze[y][x] = 3;	// 목적지
				endY = y;
				endX = x;
				DrawCell(RGB(255, 0, 0), x, y);
			}
			else
			{
				maze[y][x] = 0;	// 빈 공간
				DrawCell(RGB(255, 255, 255), x, y);
			}
		}
	}

	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_IMMEDIATELY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ACTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STEPBYSTEP)->EnableWindow(TRUE);
	Activation(true);
}

void CRoboticsDlg::OnLButtonDown(UINT nFlags, CPoint point)	// 마우스 클릭 시
{
	x = (int)((point.x) / m_interval);
	y = (int)((point.y) / m_interval);
	if (x >= 0 && x < w && y >= 0 && y < h && !oneStep)	// 맵의 범위 제한, Search 진행 중 제한
	{
		if (maze[y][x] == 1)	// 장애물인 경우
		{
			maze[y][x] = 0;
			DrawCell(RGB(255, 255, 255), x, y);
		}
		else if (maze[y][x] == 0)	// 빈 공간인 경우
		{
			SetCapture();
			maze[y][x] = 1;
			DrawCell(RGB(0, 0, 0), x, y);
			m_dragFlag = 1;
		}
		else if (maze[y][x] == 2)	// 출발지인 경우
		{
			SetCapture();
			m_saveDragPos = point;
			m_dragFlag = 2;
		}
		else if (maze[y][x] == 3)	// 목적지인 경우
		{
			SetCapture();
			m_saveDragPos = point;
			m_dragFlag = 3;
		}
	}
}

void CRoboticsDlg::OnMouseMove(UINT nFlags, CPoint point)	// 마우스 드래그 시
{
	m_dragPos = point;
	x = (int)(m_dragPos.x / m_interval);
	y = (int)(m_dragPos.y / m_interval);
	old_x = (int)(m_saveDragPos.x / m_interval);
	old_y = (int)(m_saveDragPos.y / m_interval);

	if (x >= 0 && x < w && y >= 0 && y < h && !oneStep)
	{
		if (m_dragFlag == 1 && maze[y][x] == 0)	// 장애물인 경우
		{
			DrawCell(RGB(0, 0, 0), x, y);
			maze[y][x] = 1;
		}
		if (m_dragFlag == 2 && maze[y][x] == 0)	// 출발지인 경우
		{
			DrawCell(RGB(255, 255, 255), old_x, old_y);
			maze[old_y][old_x] = 0;

			DrawCell(RGB(36, 223, 5), x, y);
			maze[y][x] = 2;

			m_saveDragPos = point;
			startY = y;
			startX = x;
		}
		if (m_dragFlag == 3 && maze[y][x] == 0)	// 목적지인 경우
		{
			DrawCell(RGB(255, 255, 255), old_x, old_y);
			maze[old_y][old_x] = 0;

			DrawCell(RGB(255, 0, 0), x, y);
			maze[y][x] = 3;

			m_saveDragPos = point;
			endY = y;
			endX = x;
		}
	}
}

void CRoboticsDlg::OnLButtonUp(UINT nFlags, CPoint point)	// 마우스 왼쪽 버튼 업
{
	ReleaseCapture();
	m_dragFlag = 0;
}

void CRoboticsDlg::OnBnClickedButtonClear()	// 알고리즘 적용 전으로 초기화
{
	Reset();

	GetDlgItem(IDC_BUTTON_IMMEDIATELY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ACTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STEPBYSTEP)->EnableWindow(TRUE);
	Activation(true);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			switch (maze[j][i])
			{
			case 1:
				DrawCell(RGB(0, 0, 0), i, j);
				break;
			case 2:
				DrawCell(RGB(36, 223, 5), i, j);
				break;
			case 3:
				DrawCell(RGB(255, 0, 0), i, j);
				break;
			default:
				DrawCell(RGB(255, 255, 255), i, j);
				break;
			}
		}
	}
}

void CRoboticsDlg::OnBnClickedButtonImmediately()	// 즉시 출력
{
	while (!end)
	{
		OnBnClickedButtonStepbystep();
	}
}

void CRoboticsDlg::OnBnClickedButtonStepbystep()	// 단계별 진행
{
	UpdateData(true);
	countExpand++;

	if (!oneStep)
	{
		Activation(false);
		GetDlgItem(IDC_CHECK_SMOOTHING)->EnableWindow(FALSE);
		operationSequence();

		if (m_radio == 0)	// BFS, using Queue
		{
			q.push(make_tuple(startY, startX, pt));
			check[startY][startX] = -1;
			cost[startY][startX] = 0;
		}
		else if (m_radio == 1)	// DFS, using Stack
		{
			s.push(make_tuple(startY, startX, pt));
			check[startY][startX] = -1;
			cost[startY][startX] = 0;
		}
		else if (m_radio == 2)	// Greedy
		{
			H = HeuristicDistance(startX, startY);
			pqGreedy.push(make_tuple(H, 0, startY, startX, pt));
			check[startY][startX] = -1;
		}
		else if (m_radio == 3)	// A*
		{
			F = G + HeuristicDistance(startX, startY);
			pq.push(make_tuple(F, G, 0, startY, startX, pt));
			check[startY][startX] = -1;
		}
		oneStep = true;
	}
	else if (oneStep && !(q.empty() && s.empty() && pq.empty() && pqGreedy.empty()))
	{
		if (m_radio == 0)		// BFS
		{
			tuple<int, int, vector<pair<int, int>>> temp = q.front();
			q.pop();

			if (get<0>(temp) == endY && get<1>(temp) == endX)
			{
				Complete(temp);
				return;
			}

			DrawCell(RGB(0, 162, 232), get<1>(temp), get<0>(temp));
			DrawInt(cost[get<0>(temp)][get<1>(temp)], get<1>(temp), get<0>(temp));

			for (int i = 0; i < 4; i++)
			{
				int n = get<0>(temp) + y_pos[i];
				int m = get<1>(temp) + x_pos[i];

				if (m >= 0 && m < w && n >= 0 && n < h && maze[n][m] != 1 && check[n][m] != -1)
				{
					cost[n][m] = cost[get<0>(temp)][get<1>(temp)] + 1;
					check[n][m] = -1;

					pt = get<2>(temp);
					pt.push_back(make_pair(get<0>(temp), get<1>(temp)));
					q.push(make_tuple(n, m, pt));

					DrawCell(RGB(153, 217, 234), m, n);
					DrawInt(cost[n][m], m, n);
				}
			}
		}
		else if (m_radio == 1)		// DFS
		{
			tuple<int, int, vector<pair<int, int>>>temp = s.top();
			int ck = 0;

			if (get<0>(temp) == endY && get<1>(temp) == endX)
			{
				Complete(temp);
				return;
			}

			DrawCell(RGB(0, 162, 232), get<1>(temp), get<0>(temp));
			DrawInt(cost[get<0>(temp)][get<1>(temp)], get<1>(temp), get<0>(temp));

			for (int i = 0; i < 4; i++)
			{
				int n = get<0>(temp) + y_pos[i];
				int m = get<1>(temp) + x_pos[i];

				if (m >= 0 && m < w && n >= 0 && n < h && maze[n][m] != 1 && check[n][m] != -1)
				{
					cost[n][m] = cost[get<0>(temp)][get<1>(temp)] + 1;
					check[n][m] = -1;
					ck = 1;

					pt = get<2>(temp);
					pt.push_back(make_pair(get<0>(temp), get<1>(temp)));
					s.push(make_tuple(n, m, pt));

					DrawCell(RGB(153, 217, 234), m, n);
					DrawInt(cost[n][m], m, n);
					break;
				}
			}
			if (!ck)
			{
				s.pop();
				OnBnClickedButtonStepbystep();
			}
		}
		else if (m_radio == 2)		// Greedy
		{
			tuple<int, int, int, int, vector<pair<int, int>>>temp = pqGreedy.top();
			pqGreedy.pop();

			H = get<0>(temp);

			if (get<2>(temp) == endY && get<3>(temp) == endX)
			{
				Complete(make_tuple(get<2>(temp), get<3>(temp), get<4>(temp)));
				return;
			}

			DrawCell(RGB(0, 162, 232), get<3>(temp), get<2>(temp));
			DrawInt(H, get<3>(temp), get<2>(temp));

			for (int i = 0; i < 4; i++)
			{
				int n = get<2>(temp) + y_pos[i];
				int m = get<3>(temp) + x_pos[i];

				if (m >= 0 && m < w && n >= 0 && n < h && maze[n][m] != 1 && check[n][m] != -1)
				{
					DrawCell(RGB(153, 217, 234), m, n);
					DrawInt(HeuristicDistance(m, n), m, n);

					H = HeuristicDistance(m, n);
					pt = get<4>(temp);
					pt.push_back(make_pair(get<2>(temp), get<3>(temp)));
					pqGreedy.push(make_tuple(H, get<1>(temp) + i, n, m, pt));
					check[n][m] = -1;
				}
			}
		}
		else if (m_radio == 3)	// Astar
		{
			tuple<int, int, int, int, int, vector<pair<int, int>>> temp = pq.top();
			pq.pop();

			F = get<0>(temp);
			G = get<1>(temp);

			if (get<3>(temp) == endY && get<4>(temp) == endX)
			{
				Complete(make_tuple(get<3>(temp), get<4>(temp), get<5>(temp)));
				return;
			}

			DrawCell(RGB(0, 162, 232), get<4>(temp), get<3>(temp));
			DrawInt(F, get<4>(temp), get<3>(temp));

			for (int i = 0; i < 4; i++)
			{
				int n = get<3>(temp) + y_pos[i];
				int m = get<4>(temp) + x_pos[i];

				if (m >= 0 && m < w && n >= 0 && n < h && maze[n][m] != 1 && check[n][m] != -1)
				{
					DrawCell(RGB(153, 217, 234), m, n);
					DrawInt(G + 1 + HeuristicDistance(m, n), m, n);

					F = G + 1 + HeuristicDistance(m, n);
					pt = get<5>(temp);
					pt.push_back(make_pair(get<3>(temp), get<4>(temp)));
					pq.push(make_tuple(F, G + 1, get<2>(temp) + i, n, m, pt));
					check[n][m] = -1;
				}
			}
		}
	}
	else if (oneStep && q.empty() && s.empty() && pq.empty() && pqGreedy.empty())
	{
		GetDlgItem(IDC_BUTTON_IMMEDIATELY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ACTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STEPBYSTEP)->EnableWindow(FALSE);
		CClientDC dc(this);
		CString str;
		str.Format(_T("There is no path to the end point"), countExpand - 1, countCost - 1);
		m_font.DeleteObject();
		m_font.CreateFont(32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("HY견고딕"));
		dc.SetBkMode(TRANSPARENT);
		dc.SelectObject(m_font);
		dc.TextOut(30, 460, str);
		end = true;
	}
}

void CRoboticsDlg::OnBnClickedButtonAction()	// Action 버튼 클릭 시
{
	while (!end)
	{
		MSG msg;
		if (PeekMessage(&msg, GetSafeHwnd(), 0, 0, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		OnBnClickedButtonStepbystep();
		Sleep(m_slider.GetPos());
	}
}

void CRoboticsDlg::Activation(bool b)	// Item 컨트롤
{
	GetDlgItem(IDC_COMBO_ROW)->EnableWindow(b);
	GetDlgItem(IDC_COMBO_COLUMN)->EnableWindow(b);
	GetDlgItem(IDC_RADIO_BFS)->EnableWindow(b);
	GetDlgItem(IDC_RADIO_DFS)->EnableWindow(b);
	GetDlgItem(IDC_RADIO_GREEDY)->EnableWindow(b);
	GetDlgItem(IDC_RADIO_ASTAR)->EnableWindow(b);
	GetDlgItem(IDC_COMBO_UP)->EnableWindow(b);
	GetDlgItem(IDC_COMBO_DOWN)->EnableWindow(b);
	GetDlgItem(IDC_COMBO_LEFT)->EnableWindow(b);
	GetDlgItem(IDC_COMBO_RIGHT)->EnableWindow(b);
	GetDlgItem(IDC_CHECK_SMOOTHING)->EnableWindow(!b);
}

void CRoboticsDlg::operationSequence()	// 동작 순서 처리 함수
{
	m_up.GetLBText(m_up.GetCurSel(), up);
	m_down.GetLBText(m_down.GetCurSel(), down);
	m_left.GetLBText(m_left.GetCurSel(), left);
	m_right.GetLBText(m_right.GetCurSel(), right);

	direction.push(make_pair(_ttoi(up), 2));
	direction.push(make_pair(_ttoi(down), 0));
	direction.push(make_pair(_ttoi(left), 3));
	direction.push(make_pair(_ttoi(right), 1));

	for (int i = 0; i < 4; i++)
	{
		switch (direction.top().second)
		{
		case 0:
			x_pos[i] = 0;
			y_pos[i] = 1;
			break;
		case 1:
			x_pos[i] = 1;
			y_pos[i] = 0;
			break;
		case 2:
			x_pos[i] = 0;
			y_pos[i] = -1;
			break;
		case 3:
			x_pos[i] = -1;
			y_pos[i] = 0;
			break;
		}
		direction.pop();
	}
}

void CRoboticsDlg::Reset()	// Clear or Build 시 리셋 동작
{
	RedrawWindow();
	memset(check, 0, sizeof(check));
	memset(cost, 0, sizeof(cost));
	queue<tuple<int, int, vector<pair<int, int>>>>qEmpty;
	stack<tuple<int, int, vector<pair<int, int>>>>sEmpty;
	priority_queue<tuple<int, int, int, int, vector<pair<int, int>>>, vector<tuple<int, int, int, int, vector<pair<int, int>>>>, greater<tuple<int, int, int, int, vector<pair<int, int>>>>>pqGreedyEmpty;
	priority_queue<tuple<int, int, int, int, int, vector<pair<int, int>>>, vector<tuple<int, int, int, int, int, vector<pair<int, int>>>>, greater<tuple<int, int, int, int, int, vector<pair<int, int>>>>>pqEmpty;
	q.swap(qEmpty);
	s.swap(sEmpty);
	pqGreedy.swap(pqGreedyEmpty);
	pq.swap(pqEmpty);
	oneStep = false;
	pt.clear();
	G = 0;
	end = false;
	m_ckSmoothing.SetCheck(0);
	countExpand = 0;
	countCost = 0;
}

void CRoboticsDlg::DrawCell(DWORD color, int x, int y)	// 칸 그리는 함수
{
	CClientDC dc(this);
	SetDCBrushColor(dc, color);
	dc.SelectStockObject(DC_BRUSH);
	dc.Rectangle((int)(x * m_interval), (int)(y * m_interval), (int)((x + 1) * m_interval + 1), (int)((y + 1) * m_interval + 1));
}

void CRoboticsDlg::DrawInt(int value, int x, int y)	// 코스트 mapping
{
	CClientDC dc(this);
	m_font.DeleteObject();
	m_font.CreateFont((int)(m_interval), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("HY견고딕"));
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(m_font);

	CString str;
	str.Format("%d", value);
	if (value < 10)
		dc.TextOut((int)(x * m_interval + 4), (int)(y * m_interval), str);
	else if (value < 100)
		dc.TextOut((int)(x * m_interval + 1), (int)(y * m_interval), str);
	else
		dc.TextOut((int)(x * m_interval - 4), (int)(y * m_interval), str);
}

int CRoboticsDlg::HeuristicDistance(int x, int y)
{
	return (abs(x - endX) + abs(y - endY));
}

void CRoboticsDlg::Complete(tuple<int, int, vector<pair<int, int>>> temp)	// 목적지에 닿음, 경로 출력
{
	CClientDC dc(this);
	int i = 0;
	GetDlgItem(IDC_BUTTON_IMMEDIATELY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ACTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STEPBYSTEP)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SMOOTHING)->EnableWindow(TRUE);

	pt = get<2>(temp);
	pt.push_back(make_pair(get<0>(temp), get<1>(temp)));

	for(int i = 0; i < pt.size(); i++)
	{
		if (i == 0)	// 출발지
			DrawCell(RGB(36, 223, 5), pt.at(i).second, pt.at(i).first);
		else
			if (i == pt.size() - 1)	// 목적지
				DrawCell(RGB(255, 0, 0), pt.at(i).second, pt.at(i).first);
			else	// 경로
				DrawCell(RGB(255, 192, 0), pt.at(i).second, pt.at(i).first);
		DrawInt(i, pt.at(i).second, pt.at(i).first);
		Sleep(20);
		countCost++;
	}
	CString str;
	str.Format(_T("Expended Nodes : %d, Cost : %d"), countExpand - 1, countCost - 1);
	m_font.DeleteObject();
	m_font.CreateFont(32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("HY견고딕"));
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(m_font);
	dc.TextOut(30,460,str);
	end = true;
}

void CRoboticsDlg::OnBnClickedCheckSmoothing()	// Smoothing 알고리즘 구현
{
	if (IsDlgButtonChecked(IDC_CHECK_SMOOTHING))	// 체크박스 여부
		Smoothing();
	else	// 체크박스 여부
		for (int i = 0; i < pt.size(); i++)
		{
			if (i == 0)
				DrawCell(RGB(36, 223, 5), pt.at(i).second, pt.at(i).first);
			else
				if (i == pt.size() - 1)
					DrawCell(RGB(255, 0, 0), pt.at(i).second, pt.at(i).first);
				else
					DrawCell(RGB(255, 192, 0), pt.at(i).second, pt.at(i).first);
			DrawInt(i, pt.at(i).second, pt.at(i).first);
		}
}

void CRoboticsDlg::Smoothing()	// 스무딩 구현
{
	CClientDC dc(this);
	double tolrance = 0.0001;
	double changeX = tolrance;
	double changeY = tolrance;
	double alpha = 0.5;
	double beta = 0.25;
	double ax, ay, *x, *y;
	x = (double*)malloc(sizeof(double) * pt.size());
	y = (double*)malloc(sizeof(double) * pt.size());

	for (int i = 0; i < pt.size(); i++)
	{
		x[i] = pt.at(i).second * m_interval + m_interval / 2;
		y[i] = pt.at(i).first * m_interval + m_interval / 2;
	}
	while (changeX >= tolrance)	// x smoothing
	{
		changeX = 0.0;
		for (int i = 1; i < pt.size() - 1; i++)
		{
			ax = x[i];
			x[i] += alpha * (pt.at(i).second * m_interval + m_interval / 2 - x[i]);
			x[i] += beta * (x[i - 1] + x[i + 1] - (2.0 * x[i]));
			changeX += abs(ax - x[i]);
		}
	}
	while (changeY >= tolrance)	// y smoothing
	{
		changeY = 0.0;
		for (int i = 1; i < pt.size() - 1; i++)
		{
			ay = y[i];
			y[i] += alpha * (pt.at(i).first * m_interval + m_interval / 2 - y[i]);
			y[i] += beta * (y[i - 1] + y[i + 1] - (2.0 * y[i]));
		}
	}
	for (int i = 1; i < pt.size(); i++)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		dc.SelectObject(&pen);
		dc.MoveTo(x[i - 1], y[i - 1]);
		dc.LineTo(x[i], y[i]);
		Sleep(10);
	}
}