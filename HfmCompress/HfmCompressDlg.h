
// HfmCompressDlg.h : ͷ�ļ�
//

#pragma once
#include "Compress.h";
#include "Decompress.h"

// CHfmCompressDlg �Ի���
class CHfmCompressDlg : public CDialogEx
{
// ����
public:
	CHfmCompressDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HFMCOMPRESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_msg;
	CString fileName;
	CFileDialog *openFileDlg;
};
