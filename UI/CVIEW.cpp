// CVIEW.cpp: 实现文件
//

#include "pch.h"
#include "UI.h"
#include "CVIEW.h"
#include "afxdialogex.h"
#include <iostream>
#include "UIDlg.h"
using namespace std;

// CVIEW 对话框

IMPLEMENT_DYNAMIC(CVIEW, CDialogEx)

CVIEW::CVIEW(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIEW, pParent)
{
	p_Color = new COLORREF(RGB(255, 0, 0));
	m_zoom = 1.0f;
	m_imgX = 0.0f;
	m_imgY = 0.0f;
	IsDraw = false;
	Ispic = false;
	Issend = false;
	m_PtStart.X = 0.0f;
	m_PtStart.Y = 0.0f;
	m_mousepressed = false;
	Picture = "./res/picbg.bmp";


	m_rimgX = 0.0f;
	m_rimgY = 0.0f;
	m_simgX = 0.0f;
	m_simgY = 0.0f;
}

CVIEW::~CVIEW()
{
}

void CVIEW::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVIEW, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CVIEW 消息处理程序


BOOL CVIEW::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MoveWindow(m_Rect);//初始化界面位置
	Imgwidth = m_Rect.Width();
	ImgHeight = m_Rect.Height();
	// TODO:  在此添加额外的初始化
	/////////////////////控制台
	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	GetClientRect(m_Rect);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CVIEW::OnSize(UINT nType, int cx, int cy)
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

void CVIEW::OnMouseMove(UINT nFlags, CPoint point)
{
	HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags & MK_MBUTTON)
	{
		REAL deltaX = point.x - m_mouseDown.X; //x轴方向偏移
		REAL deltaY = point.y - m_mouseDown.Y; //y轴方向偏移

		m_rimgX += deltaX;
		m_rimgX += deltaY;
		
		m_imgX = (m_PtStart.X + (deltaX / m_zoom)); //在原始图像中X坐标偏移，这里计算的是在原始图片中的位移，原因在上面已经说明，全局的仿射变换会影响实际的位移
		m_imgY = (m_PtStart.Y + (deltaY / m_zoom)); //在原始图像中Y坐标偏移
		if (!Isoutpic()) {
			m_imgX = m_PtStart.X;
			m_imgY = m_PtStart.Y;
		}
		Invalidate(FALSE);                           //重绘
	}
	else if (nFlags & MK_LBUTTON) {
		::SetCursor(hCur);
		m_EndPoint = point;

		Invalidate(FALSE);
	}
	
	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CVIEW::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	REAL oldzoom = m_zoom; //保存当前的缩放系数，用于计算当前滚动时的坐标
	m_PtStart.X = m_imgX;
	m_PtStart.Y = m_imgY;
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
	if (m_zoom >= MIN_SCALES) {
		GetWindowRect(rect); //注意这里的区域是窗口相对于整个屏幕的，因为cpoint pt这个点是相对于屏幕的
		CPoint ps;
		GetCursorPos(&ps);
		/*cout << "ppt:" << ppt.x << "  " << ppt.y << endl;
		cout << "ps:" << ps.x << "  " << ps.y << endl;*/
		ppt.x -= rect.left; //计算该点在对话框中的位置
		ppt.y -= rect.top;

		int x = ppt.x - m_Rect.left;
		int y = ppt.y - m_Rect.top;

		REAL oldimagex = (x / oldzoom); //缩放前鼠标在原图中的位置
		REAL oldimagey = (y / oldzoom);
		/*cout << "old:" << oldimagex << "," << m_zoom << endl;*/
		REAL newimagex = (x / m_zoom);//缩放后鼠标在原图中的位置
		REAL newimagey = (y / m_zoom);
		/*cout << "new:" << oldimagex << "," << m_zoom << endl;*/
		m_imgX = newimagex - oldimagex + m_imgX; //计算原图应该的偏移
		m_imgY = newimagey - oldimagey + m_imgY;
		/*cout << "mouse:" << oldimagex << "," << m_zoom << endl;*/

		if (!Isoutpic()) {
			m_imgX = m_PtStart.X;
			m_imgY = m_PtStart.Y;
			m_zoom = oldzoom;
		}
		Invalidate(FALSE); //绘图
	}
	else {
		m_zoom = oldzoom;
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CVIEW::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags & MK_MBUTTON)
	{
		if (!m_mousepressed)
		{
			m_mousepressed = true;
			m_mouseDown.X = point.x;
			m_mouseDown.Y = point.y;
			m_PtStart.X = m_imgX;
			m_PtStart.Y = m_imgY;
		}

	}
	CDialogEx::OnMButtonDown(nFlags, point);
}

void CVIEW::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	m_mousepressed = false; 
	CDialogEx::OnMButtonUp(nFlags, point);
}

void CVIEW::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (Ispic&Issend) {
		w = (m_imgX * m_zoom);//左上角坐标
		h = (m_imgY * m_zoom);
		//显示坐标起点


		w = ((point.x - w) / m_zoom)*(WIDTH / Imgwidth);
		h = ((point.y - h) / m_zoom)*(HEIGHT / ImgHeight);
		if (w >= 0 & w <= WIDTH) {
			if (h >= 0 & h <= HEIGHT) {
				CPoint pp;
				pp.x = w;
				pp.y = h;

				//cout << "youxia:" << w << "\n" << h << endl;

				////向主窗口发送鼠标位置
				CUIDlg * adlg = (CUIDlg *)this->GetParent();
				adlg->getMousePoint(&pp);
				adlg->UpdateData(FALSE);
			}
		}
	}
	
	
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CVIEW::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_click = TRUE;
	m_StartPoint = point;
	m_EndPoint = point;

	//GDIInvalidate();
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CVIEW::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_click = FALSE;
	//Invalidate(FALSE);//更新界面
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CVIEW::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnRButtonUp(nFlags, point);
}

void CVIEW::setDlgRect(CRect rect)
{
	m_Rect=rect;
	
}

void CVIEW::setDlgPic(CString pic)
{
	Picture = pic;
	Ispic = true;
	Invalidate(FALSE);//更新界面
}

void CVIEW::setDlgDrawRect(BOOL ISDrawRect)
{
	IsDraw = ISDrawRect;
}

void CVIEW::setDlgSend(BOOL IsSend)
{
	Issend = IsSend;
}



void CVIEW::CImageToMat(CImage & cimage, cv::Mat & mat)
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

CImage * CVIEW::MatToCImage(cv::Mat & mat, CImage * cimage)
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

void CVIEW::Draw(CDC * pDC)
{
	Image img(Picture);  //注意，请设置你自己的图片</span>
	Graphics graph(pDC->GetSafeHdc());
	graph.SetInterpolationMode(InterpolationModeHighQualityBilinear);//设置缩放质量 
	graph.ScaleTransform(m_zoom, m_zoom);//缩放 
	graph.DrawImage(&img, m_imgX, m_imgY);//m_imgX,m_imgY是原图应该偏移的量
	//graph.ScaleTransform(1, 1);//缩放
	//graph.DrawImage(&img, 400, 0);//m_imgX,m_imgY是原图应该偏移的量
	/*printf("m_imgX= %f \n", m_imgX);
	printf("m_imgY= %f \n", m_imgY);*/
}

void CVIEW::GDIInvalidate()
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
	memDC.FillSolidRect(0, 0, m_Rect.Width(), m_Rect.Height(), RGB(34,34,34));
	
	//画图 
	Draw(&memDC);
	//拷贝到控件DC 
	dc.BitBlt(0, 0, m_Rect.Width(), m_Rect.Height(), &memDC, 0, 0, SRCCOPY);
	MemBitmap.DeleteObject();

	DrawRect(&dc);

	memDC.DeleteDC();
	dc.Detach();
	::ReleaseDC(m_hWnd, hdc);
}

bool CVIEW::Isoutpic()
{
	if (m_imgX > (20 - Imgwidth)&m_imgX < (Imgwidth - 20)) 
	{
		if (m_imgY > (20 - ImgHeight)&m_imgY < (ImgHeight - 20)) {
			return true;
		}
	}
	return false;
		
}

bool CVIEW::IsOnpic()
{
	

	return false;
}

void CVIEW::DrawRect(CDC * pDC)
{
	if (m_click&IsDraw) {
		//画矩形
		pDC->SelectStockObject(NULL_BRUSH);
		CPen NewPen(PS_SOLID, 1, *p_Color);
		pDC->SelectObject(&NewPen);
		pDC->Rectangle(&CRect(m_StartPoint, m_EndPoint));
	}
	
}


void CVIEW::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	GDIInvalidate();
}
