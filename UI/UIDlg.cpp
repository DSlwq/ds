
// UIDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "UI.h"
#include "UIDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <iostream>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//// 定义子窗口格局
//#define SUB_DLG_NUM (2)
//#define ROW_NUM (1)
//#define COLUM_NUM (2)

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CUIDlg 对话框


IMPLEMENT_DYNAMIC(CUIDlg, CDialogEx);

CUIDlg::CUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = nullptr;
}

CUIDlg::~CUIDlg()
{
	// 如果该对话框有自动化代理，则
	//  此对话框的返回指针为 null，所以它知道
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void CUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	/*ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()*/
	ON_WM_SIZE()
	
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CUIDlg 消息处理程序

BOOL CUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	/////////////////////控制台
	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	// TODO: 在此添加额外的初始化代码
	
	initUIDlgs();//初始化界面布局
	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUIDlg::OnPaint()
{
	//GDIInvalidate();
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC   dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC   dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);  //对话框的背景图片  
		BITMAP   bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap   *pbmpOld = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。  这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。



void CUIDlg::initUIDlgs()
{
	//int iColum = COLUM_NUM;
	//int iRow = ROW_NUM;

	//int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);//根据显示器分辨率设置软件界面大小
	//int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	//MoveWindow(0, 0, iScreenWidth, iScreenHeight);

	m_views = new CVIEW[2];
	CRect rect[2];
	rect[0].SetRect(285, 225, 1085, 825);
	rect[1].SetRect(1090, 225, 1890, 825);

	m_views[0].setDlgRect(&rect[0]);
	m_views[0].Create(IDD_VIEW, this);
	m_views[0].ShowWindow(SW_SHOW);
	m_views[0].setDlgPic(_T("456.bmp"));
	Sleep(100);
	m_views[1].setDlgRect(&rect[1]);
	m_views[1].Create(IDD_VIEW, this);
	m_views[1].ShowWindow(SW_SHOW);
	//m_views[1].setDlgPic(_T(".bmp"));
}
void CUIDlg::OnClose()
{
	if (CanExit())
		
		CDialogEx::OnClose();
}

void CUIDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CUIDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CUIDlg::CanExit()
{
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}
	if (m_views != NULL) {
		//清理显示界面指针
		delete[] m_views;
		m_views = NULL;
	}

	return TRUE;
}


void CUIDlg::ReSize()
{
	
	if (m_listRect.GetCount() > 0)
	{
		CRect dlgNow;
		GetWindowRect(&dlgNow);
		POSITION pos = m_listRect.GetHeadPosition();//第一个保存的是对话框的Rect  

		CRect dlgSaved;
		dlgSaved = m_listRect.GetNext(pos);
		ScreenToClient(dlgNow);

		float x = dlgNow.Width() * 1.0 / dlgSaved.Width();//根据当前和之前保存的对话框的宽高求比例  
		float y = dlgNow.Height()  *1.0 / dlgSaved.Height();
		ClientToScreen(dlgNow);

		CRect childSaved;

		CWnd* pWnd = GetWindow(GW_CHILD);
		while (pWnd)
		{
			childSaved = m_listRect.GetNext(pos);//依次获取子窗体的Rect  
			childSaved.left = dlgNow.left + (childSaved.left - dlgSaved.left)*x;//根据比例调整控件上下左右距离对话框的距离  
			childSaved.right = dlgNow.right + (childSaved.right - dlgSaved.right)*x;
			childSaved.top = dlgNow.top + (childSaved.top - dlgSaved.top)*y;
			childSaved.bottom = dlgNow.bottom + (childSaved.bottom - dlgSaved.bottom)*y;
			ScreenToClient(childSaved);
			pWnd->MoveWindow(childSaved, TRUE);
			pWnd = pWnd->GetNextWindow();
		}

	}
	
	

}

void CUIDlg::upListRect()
{
	CRect rect;
	GetWindowRect(&rect);
	m_listRect.AddTail(rect);//对话框的区域  

	CWnd* pWnd = GetWindow(GW_CHILD);//获取子窗体  
	while (pWnd)
	{
		pWnd->GetWindowRect(rect);//子窗体的区域  
		m_listRect.AddTail(rect);           //CList<CRect,CRect> m_listRect成员变量  
		pWnd = pWnd->GetNextWindow();//取下一个子窗体  
	}
}

CPoint CUIDlg::getMousePoint(CPoint * p)
{
	cout << "坐标：" << p->x << " , " << p->y << endl;
	return CPoint();
}








