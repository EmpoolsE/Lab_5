#include "stdafx.h"
#include "Taxi.h"
#include "TaxiDlg.h"
#include "afxdialogex.h"
#include <afxdb.h>
#include "Update.h"
#include "Insert.h"
#include "Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDatabase db;
bool opend=false;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
  CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CTaxiDlg::CTaxiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaxiDlg::IDD, pParent)
	, vstav_id(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sort = 0;
}

void CTaxiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Radio(pDX, IDC_RADIO1, m_sort);
}

BEGIN_MESSAGE_MAP(CTaxiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTaxiDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CTaxiDlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTaxiDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTaxiDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CTaxiDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON3, &CTaxiDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTaxiDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTaxiDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CTaxiDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDCANCEL, &CTaxiDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON8, &CTaxiDlg::OnBnClickedButton8)
END_MESSAGE_MAP()

// Ckursova9Dlg message handlers

BOOL CTaxiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTaxiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTaxiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTaxiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTaxiDlg::Fiil_list(CString zapros) // Function for fill our list control
{
	// TODO: добавьте свой код обработчика уведомлений	
	int n,i;
	m_list.DeleteAllItems();
	n=m_list.GetHeaderCtrl()->GetItemCount();
	for(i=n-1;i>-1;i--)
		{
			m_list.DeleteColumn(i);
		}	
	CRecordset rec(&db);
	rec.Open(CRecordset::snapshot, zapros);
	int count=rec.GetODBCFieldCount();
	CODBCFieldInfo info;
	CRect rc;
	m_list.GetWindowRect(&rc);
	m_list.InsertColumn(0, _T("Id"), 0, rc.Width() / 15);
	m_list.InsertColumn(1, _T("Path"), 0, rc.Width() /2); 
	m_list.InsertColumn(2, _T("Km"), 0, rc.Width() / 20);
	m_list.InsertColumn(3, _T("Prise"), 0, rc.Width() / 5);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString buff;
	i=0;
	while (!rec.IsEOF())
	{
		for (short j=0; j<count; j++)
		{ 
			rec.GetFieldValue(j,buff);
			if(j == 0)
				m_list.InsertItem(i, buff);
			else
				m_list.SetItemText(i, j, buff);
		}
		rec.MoveNext();	
		i++;
   }	
}

void CTaxiDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CTaxiDlg::OnBnClickedButton1() //open BD
{
	CFileDialog dlg(true,NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"MS Access File (.mdb)|*.mdb||");
	if(dlg.DoModal() == IDOK)
	{
		CString ConnStr;
		CString FilePath = dlg.GetPathName();
		ConnStr.Format(_T("ODBC;DRIVER={MICROSOFT ACCESS DRIVER (*.mdb)};DSN='';DBQ=%s"),FilePath);
		if (db.Open(NULL, FALSE, FALSE, ConnStr))//если базу удалось открыть
			opend=true;	
		else 
		{
			MessageBox(L"Try again, maybe DB is opend by another program",L"Taxi",MB_OK);
			return;
		}
		CString zapros=L"Select * from Taxi";
	    Fiil_list(zapros);
	}
	
}	


void CTaxiDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CTaxiDlg::OnBnClickedButton2() //Update
{
	if (opend==false)
	{
		MessageBox(L"DB is not opened yet! Please open DB",L"Taxi",MB_OK);
		return;
	}

	UpdateData ();
	CString zapros;
	CUpdate update;
	int ed3,old;
	CString ed2;
	float ed4;
	m_list.GetSelectedCount();
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if (pos==NULL) AfxMessageBox(_T("ничего не выбрано"));
	else
	{
		int npos;
		while (pos)
		{
			npos=m_list.GetNextSelectedItem(pos);
			old=_ttoi(m_list.GetItemText(npos,0));
			ed2=m_list.GetItemText(npos,1);
			ed3=_ttoi(m_list.GetItemText(npos, 2));
			ed4=_ttoi(m_list.GetItemText(npos,3));

			
			update.m_Path=ed2;
			update.m_Km=ed3;
			update.m_Price=ed4;
			UpdateData(false);

			if (update.DoModal()==IDOK)
			{
				UpdateData();
				ed2=update.m_Path;
				ed3=update.m_Km;
				ed4=update.m_Price;
	zapros.Format(_T("update Taxi SET  Name='%s', Km='%d', Prise =%f WHERE Id = %d"),ed2,ed3,ed4,old);  
	db.ExecuteSQL(zapros);
			}
	
	zapros = "Select * from Taxi";
	Fiil_list(zapros);
		}
}
}

void CTaxiDlg::OnBnClickedButton6() //Sort
{
	if (opend==false)
	{
		MessageBox(L"Error",L"Error",MB_OK);
		return;
	}

	UpdateData();
	if(m_sort==0)
	{
	CString zapros;
	zapros.Format(_T("Select *From Taxi Order By Km"));
	Fiil_list(zapros);
	}
	if(m_sort==1)
	{
	CString zapros;
	zapros.Format(_T("Select *From Taxi Order By Prise"));
	Fiil_list(zapros);
	}
	if(m_sort==2)
	{
	CString zapros;
	zapros.Format(_T("Select *From Taxi Order By Path"));
	Fiil_list(zapros);
	}
	UpdateData(false);
}



void CTaxiDlg::OnBnClickedButton3()//Delete
{	
	if (opend==false)
	{
		MessageBox(L"DB is not opened yet! Please open DB",L"kursova9Dlg",MB_OK);
		return;
	}

	CString str, zapros;
	 if(m_list.GetSelectedCount())
	{POSITION pos=m_list.GetFirstSelectedItemPosition();
	int npos;
	while(pos)
	{
		npos=m_list.GetNextSelectedItem(pos);	
		str=m_list.GetItemText(npos,0);
		zapros.Format(L"DELETE * FROM Taxi WHERE ID = %s",str);
		db.ExecuteSQL(zapros);
		zapros = "Select * from TOV";
		Fiil_list(zapros);
	}		
	 }
	 else
		 AfxMessageBox(L"Вы ничего не выбрали!");
}


void CTaxiDlg::OnBnClickedButton4() //Insert
{	 
	if (opend==false)
	{
		MessageBox(L"Error",L"Error",MB_OK);
		return;
	}

	CInsert insert;
	insert.DoModal();
	if (insert.DoModal()==IDOK)
	{
	int  ed3,ed4;
	CString ed2, zapros;
	UpdateData();
	ed2=insert.m_Path;
	ed3=insert.m_Km;
	ed4=insert.m_Price;
	zapros=_T("Select * from Taxi");
	insert_new_id(zapros);
	zapros.Format(_T("Insert into Taxi values ('%s', '%s', %d ,%d)"),vstav_id,ed2,ed3,ed4);
	db.ExecuteSQL(zapros);	
	UpdateData (false);	
	zapros = "Select * from Taxi";
	Fiil_list(zapros);
	}
}


void CTaxiDlg::OnBnClickedButton5()//Search
{
		MessageBox(L"Not ready yet",L"Error",MB_OK);
		return;

}

void CTaxiDlg::insert_new_id(CString new_id)//Search max free id
{
	CString buff;
	int i=0, max_buff=10, max=0;
	CRecordset rec(&db);
	rec.Open(CRecordset::snapshot, new_id);
	int count=rec.GetODBCFieldCount();
	while (!rec.IsEOF())
	{
		for (short j=0; j<1; j++)
		{ 
			rec.GetFieldValue(j,buff);
			max_buff=_ttoi(buff);
			if (max_buff>max) max=max_buff;			
		}
		rec.MoveNext();	
		i++;
   }
	max+=1;
	vstav_id.Format(_T("%d"),max);
}

void CTaxiDlg::OnBnClickedButton7()
{
		MessageBox(L"Not ready yet",L"Error",MB_OK);
		return;
}


void CTaxiDlg::OnBnClickedCancel()
{
	if (MessageBox(L"Are you sure?",L"Exit",MB_YESNO|MB_ICONQUESTION)==IDYES)
	CDialogEx::OnCancel();
}


void CTaxiDlg::OnBnClickedButton8()
{
	CInfo info;
	info.DoModal();

}
