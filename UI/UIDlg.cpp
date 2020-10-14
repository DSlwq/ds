
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

// 定义子窗口格局
#define SUB_DLG_NUM (1)
#define ROW_NUM (1)
#define COLUM_NUM (1)

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
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
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

	//initUIDlgs();//初始化界面布局
	m_zoom = 1.0f;

	m_imgX = 0.0f;
	m_imgY = 0.0f;
	m_PtStart.X = 0.0f;
	m_PtStart.Y = 0.0f;

	m_mousepressed = false;


	Picture = "456.bmp";

	GetClientRect(m_Rect);
	
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
		GDIInvalidate();
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
	//m_pMypicDlgs = new CUIDlg[SUB_DLG_NUM];

	//int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	//int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	//MoveWindow(0, 0, iScreenWidth, iScreenHeight);

	//CRect rectMainDlg;
	//GetClientRect(&rectMainDlg);

	//for (int i = 0; i < SUB_DLG_NUM; i++)
	//{
	//	CRect rect;
	//	/*int nWidth = rectMainDlg.Width() / iColum;
	//	int nHeight = rectMainDlg.Height() / iRow;*/
	//	int nWidth = 800;
	//	int nHeight = 600;

	//	rect.SetRect(
	//		rectMainDlg.TopLeft().x + (i % iColum) * nWidth,
	//		rectMainDlg.TopLeft().y + (i / iColum) * nHeight,
	//		rectMainDlg.TopLeft().x + (i % iColum + 1) * nWidth,
	//		rectMainDlg.TopLeft().y + ((i / iColum) + 1) * nHeight);

	//	m_pMypicDlgs[i].setRect(&rect);
	//	m_pMypicDlgs[i].Create(IDD_VIEW, this);
	//	m_pMypicDlgs[i].ShowWindow(SW_SHOW);
	//}
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

	return TRUE;
}



void CUIDlg::OnSize(UINT nType, int cx, int cy)
{
	//GetClientRect(m_Rect); //在你的实际应用中，可能需要把这行添加到对话框的OnSize()中
	
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(m_Rect);
	////根据屏幕大小缩放控件
	//if (nType != SIZE_MINIMIZED) {
	//	ReSize();
	//}
	//	
	//this->Invalidate();
	
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



void CUIDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags & MK_MBUTTON)
	{
		REAL deltaX = point.x - m_mouseDown.X; //x轴方向偏移
		REAL deltaY = point.y - m_mouseDown.Y; //y轴方向偏移

		m_imgX = (m_PtStart.X + (deltaX / m_zoom)); //在原始图像中X坐标偏移，这里计算的是在原始图片中的位移，原因在上面已经说明，全局的仿射变换会影响实际的位移
		m_imgY = (m_PtStart.Y + (deltaY / m_zoom)); //在原始图像中Y坐标偏移
		GDIInvalidate();                            //重绘
	}
	else if (nFlags & MK_LBUTTON) {
		::SetCursor(hCur);
		m_EndPoint = point;


	}
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CUIDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	REAL oldzoom = m_zoom; //保存当前的缩放系数，用于计算当前滚动时的坐标
	CPoint ppt = pt;
	CRect  rect;
	if (zDelta == 120)
	{
		m_zoom += SCALES;
	}
	if (zDelta == -120)
	{
		m_zoom = m_zoom - SCALES;
	}

	GetWindowRect(rect); //注意这里的区域是窗口相对于整个屏幕的，因为cpoint pt这个点是相对于屏幕的

	ppt.x -= rect.left; //计算该点在对话框中的位置
	ppt.y -= rect.top;

	int x = ppt.x - m_Rect.left;
	int y = ppt.y - m_Rect.top;

	REAL oldimagex = (x / oldzoom); //缩放前鼠标在原图中的位置
	REAL oldimagey = (y / oldzoom);

	REAL newimagex = (x / m_zoom);//缩放后鼠标在原图中的位置
	REAL newimagey = (y / m_zoom);

	m_imgX = newimagex - oldimagex + m_imgX; //计算原图应该的偏移
	m_imgY = newimagey - oldimagey + m_imgY;

	GDIInvalidate(); //绘图
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CUIDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_mousepressed = false;
	CDialogEx::OnMButtonDown(nFlags, point);
}


void CUIDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_click = FALSE;
	CDialogEx::OnMButtonUp(nFlags, point);
}


void CUIDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CUIDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_click = TRUE;
	m_StartPoint = point;
	m_EndPoint = point;

	//GDIInvalidate();
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CUIDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_click = false;
	Invalidate(FALSE);//更新界面
	CDialogEx::OnLButtonUp(nFlags, point);
}







void CUIDlg::CImageToMat(CImage & cimage, cv::Mat & mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage.GetWidth();
	int nHeight = cimage.GetHeight();


	//重建mat  
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}
	//拷贝数据  
	uchar* pucRow;                                  //指向数据区的行指针  
	uchar* pucImage = (uchar*)cimage.GetBits();     //指向数据区的指针  
	int nStep = cimage.GetPitch();                  //每行的字节数,注意这个返回值有正有负  


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}

CImage * CUIDlg::MatToCImage(cv::Mat & mat, CImage * cimage)
{
	if (0 == mat.total())
	{
		return nullptr;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return nullptr;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;


	//重建cimage  
	if (cimage) cimage->Destroy();
	cimage = new CImage();
	cimage->Create(nWidth, nHeight, 8 * nChannels);
	//拷贝数据  

	uchar* pucRow;                                  //指向数据区的行指针  
	uchar* pucImage = (uchar*)cimage->GetBits();     //指向数据区的指针  
	int nStep = cimage->GetPitch();                  //每行的字节数,注意这个返回值有正有负  


	if (1 == nChannels)                             //对于单通道的图像需要初始化调色板  
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage->GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage->SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
	return cimage;
}

void CUIDlg::Draw(CDC * pDC)
{
	Image img(_T("456.bmp"));  //注意，请设置你自己的图片</span>
	Graphics graph(pDC->GetSafeHdc());
	graph.SetInterpolationMode(InterpolationModeHighQualityBilinear);//设置缩放质量 
	graph.ScaleTransform(m_zoom, m_zoom);//缩放 
	graph.DrawImage(&img, m_imgX, m_imgY);//m_imgX,m_imgY是原图应该偏移的量
}

void CUIDlg::GDIInvalidate()
{

	HDC hdc = ::GetDC(m_hWnd);
	CDC dc;
	dc.Attach(hdc);
	CDC memDC;
	CBitmap MemBitmap;
	// 设备描述表初始化 
	memDC.CreateCompatibleDC(NULL);
	// 建立与屏幕显示兼容的内存显示设备 
	MemBitmap.CreateCompatibleBitmap(&dc, m_Rect.Width(), m_Rect.Height());
	// 选取空白位图 
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, m_Rect.Width(), m_Rect.Height(), RGB(255, 255, 255));
	//画图 
	Draw(&memDC);
	//拷贝到控件DC 
	dc.BitBlt(0, 0, m_Rect.Width(), m_Rect.Height(), &memDC, 0, 0, SRCCOPY);
	MemBitmap.DeleteObject();

	//画矩形
	dc.SelectStockObject(NULL_BRUSH);
	CPen NewPen(PS_SOLID, 1, m_Color);
	dc.SelectObject(&NewPen);
	dc.Rectangle(&CRect(m_StartPoint, m_EndPoint));

	memDC.DeleteDC();
	dc.Detach();
	::ReleaseDC(m_hWnd, hdc);
}


void CUIDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnRButtonUp(nFlags, point);
}
