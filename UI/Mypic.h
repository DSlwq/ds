#pragma once
#include "CInitGdiplus.h"
#include "opencv2/opencv.hpp"
#define WIDTH 800        //图片控件的宽（初始状态）
#define HEIGHT 600        //图片控件的高（初始状态）
#define SCALES 0.1f		  //缩放的比例值
// Mypic 对话框

class Mypic : public CDialogEx
{
	DECLARE_DYNAMIC(Mypic)

public:
	Mypic(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Mypic();

public:
	void setRect(CRect rect);
// 对话框数据

	enum { IDD = IDD_VIEW };

private:
	// 自定义变量
	CRect							m_rectDlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	REAL      m_zoom; //缩放系数
	CRect     m_Rect; //对话框的矩形区域

	REAL      m_imgX; //图片左上角X轴偏移量
	REAL      m_imgY; //图片左上角Y轴偏移量

	PointF    m_PtStart; //按下鼠标中键，记录当前的m_imgX,m_imgY
	PointF    m_mouseDown; //按下鼠标中键时，鼠标在图片控件中的位置；
	bool     m_mousepressed; //记录是否按下了鼠标中键
	
	void Draw(CDC *pDC);//画图片
	void GDIInvalidate();///双缓冲绘图
	CString Picture;
public:
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

private:
	///画矩形
	bool m_click = false;//鼠标左键按下
	//绘制参数
	CPoint m_StartPoint;
	CPoint m_EndPoint;
	COLORREF m_Color ;
protected:
	void CImageToMat(CImage& cimage, cv::Mat& mat);
	CImage * MatToCImage(cv::Mat& mat, CImage *cimage);
};
