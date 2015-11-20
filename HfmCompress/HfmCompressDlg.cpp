
// HfmCompressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HfmCompress.h"
#include "HfmCompressDlg.h"
#include "afxdialogex.h"


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


// CHfmCompressDlg 对话框



CHfmCompressDlg::CHfmCompressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HFMCOMPRESS_DIALOG, pParent)
	, m_msg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHfmCompressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_msg);
}

BEGIN_MESSAGE_MAP(CHfmCompressDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CHfmCompressDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHfmCompressDlg 消息处理程序

BOOL CHfmCompressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHfmCompressDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHfmCompressDlg::OnPaint()
{
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHfmCompressDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHfmCompressDlg::OnBnClickedButton1()
{
	fileName = _T("");                                                         //打开的文件名  
	openFileDlg = new CFileDialog(true, NULL, fileName, OFN_HIDEREADONLY | OFN_READONLY, NULL, NULL);
	if (openFileDlg->DoModal())
	{
		fileName = openFileDlg->GetPathName();
		USES_CONVERSION;                                                       //UNICODE编码要多转化一下什么的
		char* inName = T2A((LPCTSTR)fileName);                                 //http://stackoverflow.com/questions/12584807/how-to-convert-cstring-to-const-char

		if (fileName.Right(4) == ".lvj")                                       //解压文件
		{
			Decompress* decompress = new Decompress(inName);
			struct _stat info1;
			inName = T2A((LPCTSTR)fileName);
			_stat(inName, &info1);
			int inSize = info1.st_size;

			struct _stat info2;
			char* outName = decompress->m_fileName;
			_stat(outName, &info2);
			int outSize = info2.st_size;

			int rate = outSize * 100 / inSize;
			CString str;
			str.Format(_T("%d"), rate);
			m_msg = _T("解压成功\r\n解压率为:") + str + _T("%");
		}
		else                                                                   //压缩文件
		{
			Compress* compress = new Compress(inName);                         //new申请一个内存空间 返回的是指针
			compress->CreateTree();
			compress->CompressCode();

			struct _stat info1;
			_stat(inName, &info1);
			int inSize = info1.st_size;

			struct _stat info2;
			const char* outName = compress->outName.c_str();
			_stat(outName, &info2);
			int outSize = info2.st_size;

			int rate = outSize * 100 / inSize;
			CString str;
			str.Format(_T("%d"), rate);
			m_msg = _T("压缩成功\r\n压缩率为:") + str + _T("%");
		}
	}
	UpdateData(FALSE);

}
