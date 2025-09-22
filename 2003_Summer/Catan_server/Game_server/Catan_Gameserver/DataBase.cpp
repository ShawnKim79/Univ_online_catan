// DataBase.cpp: implementation of the CDataBase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DataBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataBase::CDataBase()
{
	AffectCount = -1;
	ret = SQL_SUCCESS;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
}

CDataBase::~CDataBase()
{
	SQLCloseCursor(hStmt);

	if(hStmt)	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if(hDbc)	SQLDisconnect(hDbc);
	if(hDbc)	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if(hEnv)	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}


CDataBase::Disconnect()
{
	SQLCloseCursor(hStmt);

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

BOOL CDataBase::Connect(int nType, char *ConStr, char *UID, char *PWD)
{
	SQLCHAR		InCon[255];
	SQLCHAR		OutCon[255];
	SQLSMALLINT	cbOutCon;

	int i = 1;
	SQLRETURN	Ret;
	SQLINTEGER	NativeError;
	SQLCHAR		SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;
	char str[256];

	SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	switch(nType)
	{
		case 1 :
			wsprintf((char *)InCon, "DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;", ConStr);
			ret = SQLDriverConnect(hDbc, NULL, (SQLCHAR *)InCon, sizeof(InCon), OutCon, sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
			break;

		case 2 :
			wsprintf((char *)InCon, "FileDsn=%s", ConStr);
			ret = SQLDriverConnect(hDbc, NULL, (SQLCHAR *)InCon, sizeof(InCon), OutCon, sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
			break;

		case 3 :
			ret = SQLConnect(hDbc, (SQLCHAR *)ConStr, SQL_NTS, (SQLCHAR *)UID, SQL_NTS, (SQLCHAR *)PWD, SQL_NTS);
			break;
	}

	if(( ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO))
	{
		while(Ret = SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
		{
			wsprintf(str, "SQLSTATE : %s, 진단정보 : %s", (LPCTSTR)SqlState, (LPCTSTR)Msg);
			::MessageBox(NULL, str, "진단정보", MB_OK);
			i++;
		}

		return FALSE;
	}

	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO))
	{
		hStmt = 0;
		return FALSE;
	}

	return TRUE;
}

BOOL CDataBase::SQLQuery(LPCTSTR szSQL)
{
	ret = SQLExecDirect(hStmt, (SQLCHAR *)szSQL, SQL_NTS);

	if((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO) && (ret != SQL_NO_DATA))
	{
		PrintDiag();
		return FALSE;
	}

	SQLRowCount(hStmt, &AffectCount);
	SQLNumResultCols(hStmt, &nCol);

	if(nCol > MAXCOL)
	{
		::MessageBox(NULL, "최대 컬럼수를 초과했습니다.", "Query Error", MB_OK);
		return FALSE;
	}

	if(nCol == 0)
	{
		Clear();
		return TRUE;
	}

	for(int c = 0 ; c < nCol ; c++)
	{
		SQLBindCol(hStmt, c+1, SQL_C_CHAR, Col[c], 255, &lCol[c]);
		SQLDescribeCol(hStmt, c+1, ColName[c], 30, NULL, NULL, NULL, NULL, NULL);
	}

	return TRUE;
}

int CDataBase::ExecGetInt(LPCTSTR szSQL)
{
	if(SQLQuery(szSQL) == FALSE)
	{
		return QUERYERROR;
	}

	if(Fetch() == SQL_NO_DATA)
	{
		Clear();
		return QUERYEOF;
	}

	int i = GetInt(1);
	Clear();

	return i;
}

void CDataBase::ExecGetStr(LPCTSTR szSQL, char *buf)
{
	if(SQLQuery(szSQL) == FALSE)
	{
		strcpy(buf, "QUERYERROR");
		return;
	}

	if(Fetch() == SQL_NO_DATA)
	{
		Clear();
		strcpy(buf, "EOF");
		return;
	}

	GetStr(1, buf);
	Clear();
}

SQLRETURN CDataBase::Fetch()
{
	ret = SQLFetch(hStmt);
	return ret;
}

void CDataBase::Clear()
{
	SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_UNBIND);
}


int CDataBase::FindCol(char *name)
{
	for(int i = 0 ; i < nCol ; i++)
	{
		if(stricmp(name, (LPCTSTR)ColName[i]) == 0)
		{
			return i+1;
		}
	}

	return -1;
}

int CDataBase::GetInt(int nCol)
{
	if(nCol > this->nCol) return QUERYNOCOL;
	if(lCol[nCol-1] == SQL_NULL_DATA) return QUERYNULL;
	else return atoi(Col[nCol-1]);
}

int CDataBase::GetInt(char *sCol)
{
	int n = FindCol(sCol);

	if(n == -1) return QUERYNOCOL;
	else return GetInt(n);
}

void CDataBase::GetStr(int nCol, char *buf)
{
	if(nCol > this->nCol)
	{
		strcpy(buf, "QUERYNOCOL");
		return;
	}

	if(lCol[nCol-1] == SQL_NULL_DATA) lstrcpy(buf, "NULL");
	else lstrcpy(buf, Col[nCol-1]);
}

void CDataBase::GetStr(char *sCol, char *buf)
{
	int n = FindCol(sCol);

	if(n == -1) lstrcpy(buf, "ERROR:Column not Found");
	else GetStr(n, buf);
}

void CDataBase::PrintDiag()
{
	int i = 1;
	SQLRETURN	Ret;
	SQLINTEGER	NativeError;
	SQLCHAR		SqlState[6], Msg[255];
	SQLSMALLINT	MsgLen;
	char str[256];

	while(Ret = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		wsprintf(str, "SQLSTATE:%s, 진단정보:%s", (LPCTSTR)SqlState, (LPCTSTR)Msg);
		::MessageBox(NULL, str, "진단정보", MB_OK);
		i++;
	}

	if(i > 1)
	{
		::MessageBox(NULL, "진단정보가 출력되었습니다. 이 정보가 출력될 경우는 네트워크 끊김, DB 서버 중지등의\r\n"
			"에러를 수정하신 후 프로그램을 다시 실행해 주십시오.", "Critical Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
}

int CDataBase::ReadBlob(LPCTSTR szSQL, void *buf)
{
	return 1;
}

void CDataBase::WriteBlob(LPCTSTR szSQL, void *buf, int size)
{

}

int CDataBase::GetRowCount()
{
	return (int)AffectCount;
}
