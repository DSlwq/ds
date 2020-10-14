
// DlgProxy.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "UI.h"
#include "DlgProxy.h"
#include "UIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIDlgAutoProxy

IMPLEMENT_DYNCREATE(CUIDlgAutoProxy, CCmdTarget)

CUIDlgAutoProxy::CUIDlgAutoProxy()
{
	EnableAutomation();

	// 为使应用程序在自动化对象处于活动状态时一直保持
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。  设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CUIDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CUIDlg)))
		{
			m_pDialog = reinterpret_cast<CUIDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CUIDlgAutoProxy::~CUIDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CUIDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CUIDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUIDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IUI 的支持来支持类型安全绑定
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {29abb7fd-f2db-4b88-ae99-db5a0da9ac84}
static const IID IID_IUI =
{0x29abb7fd,0xf2db,0x4b88,{0xae,0x99,0xdb,0x5a,0x0d,0xa9,0xac,0x84}};

BEGIN_INTERFACE_MAP(CUIDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CUIDlgAutoProxy, IID_IUI, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in pch.h of this project
// {bb289a1f-ad2e-45b5-b33c-7d66a40ea0c0}
IMPLEMENT_OLECREATE2(CUIDlgAutoProxy, "UI.Application", 0xbb289a1f,0xad2e,0x45b5,0xb3,0x3c,0x7d,0x66,0xa4,0x0e,0xa0,0xc0)


// CUIDlgAutoProxy 消息处理程序
