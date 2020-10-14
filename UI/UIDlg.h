
// UIDlg.h: 头文件
//

#pragma once
#include "CInitGdiplus.h"
#include "opencv2/opencv.hpp"
#define WIDTH 800        //图片控件的宽（初始状态）
#define HEIGHT 600        //图片控件的高（初始状态）
#define SCALES 0.1f		  //缩放的比例值

class CUIDlgAutoProxy;

// CUIDlg 对话框
class CUIDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUIDlg);
	friend class CUIDlgAutoProxy;

// 构造
public:
	CUIDlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~CUIDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CUIDlgAutoProxy* m_pAutoProxy;

	CInitGdiplus m_InitGdiplus;//初始化GDI

	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()


private:
	

	void initUIDlgs();

	///画矩形
	bool m_click = false;//鼠标左键按下
	//绘制参数
	CPoint m_StartPoint;
	CPoint m_EndPoint;
	COLORREF m_Color;

private:
	//自适应窗口
	CList<CRect, CRect&> m_listRect;
	
	void ReSize();//对所有控件进行布局
	void upListRect();//更新控件布局数据

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
	
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
protected:
	void CImageToMat(CImage& cimage, cv::Mat& mat);
	CImage * MatToCImage(cv::Mat& mat, CImage *cimage);
};
