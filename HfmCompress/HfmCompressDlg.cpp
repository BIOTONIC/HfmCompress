
// HfmCompressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HfmCompress.h"
#include "HfmCompressDlg.h"
#include "afxdialogex.h"


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CHfmCompressDlg �Ի���



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


// CHfmCompressDlg ��Ϣ�������

BOOL CHfmCompressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHfmCompressDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHfmCompressDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHfmCompressDlg::OnBnClickedButton1()
{
	fileName = _T("");                                                         //�򿪵��ļ���  
	openFileDlg = new CFileDialog(true, NULL, fileName, OFN_HIDEREADONLY | OFN_READONLY, NULL, NULL);
	if (openFileDlg->DoModal())
	{
		fileName = openFileDlg->GetPathName();
		USES_CONVERSION;                                                       //UNICODE����Ҫ��ת��һ��ʲô��
		char* inName = T2A((LPCTSTR)fileName);                                 //http://stackoverflow.com/questions/12584807/how-to-convert-cstring-to-const-char

		if (fileName.Right(4) == ".lvj")                                       //��ѹ�ļ�
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
			m_msg = _T("��ѹ�ɹ�\r\n��ѹ��Ϊ:") + str + _T("%");
		}
		else                                                                   //ѹ���ļ�
		{
			Compress* compress = new Compress(inName);                         //new����һ���ڴ�ռ� ���ص���ָ��
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
			m_msg = _T("ѹ���ɹ�\r\nѹ����Ϊ:") + str + _T("%");
		}
	}
	UpdateData(FALSE);

}
