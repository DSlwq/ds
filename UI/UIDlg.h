
// UIDlg.h: 头文件
//

#pragma once
#include "CVIEW.h"

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
	CVIEW* m_views;
	
	void initUIDlgs();

	

private:
	//自适应窗口
	CList<CRect, CRect&> m_listRect;
	
	void ReSize();//对所有控件进行布局
	void upListRect();//更新控件布局数据

public:
	
	
	
	
	
	

public:
	
	
	

};
