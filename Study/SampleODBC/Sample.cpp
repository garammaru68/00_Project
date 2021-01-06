#include <iostream>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <vector>
#pragma comment(lib, "odbc32.lib")

// 1)핸들선언
SQLHENV g_hEnv = SQL_NULL_HENV;		// 환경핸들
SQLHENV g_hDbc = SQL_NULL_HDBC;		// 연결핸들
SQLHENV g_hStmt = SQL_NULL_HSTMT;	// 명령핸들;

TCHAR		g_szInputUserID[32] = { 0, };
SQLHENV		g_hUserIDStmt = SQL_NULL_HSTMT; // 명령핸들
SQLTCHAR	g_szUserID[32] = { 0, };
SQLTCHAR	g_szUserPS[32] = { 0, };
SQLINTEGER	g_iID;
SQLLEN		lID, lUserID, lUserPS;

int			iNameLength = 0;
SQLLEN		cbColumn = SQL_NTS;
typedef		std::vector<std::wstring> RECORD;

struct ColDescription
{
	SQLUSMALLINT	icol;
	SQLWCHAR		szColName[80];
	SQLSMALLINT		cchColNameMax;
	SQLSMALLINT		pcchColName;
	SQLSMALLINT		pfSqlType;
	SQLULEN			pcbColDef;
	SQLSMALLINT		pibScale;
	SQLSMALLINT		pfNullable;
	SQLWCHAR		bindData[80];
	//TargetValuePtr 버퍼에서 리턴할 수 있는 바이트 수를 표시하는 값(또는 값 배열)의 포인터
	SQLLEN			byteSize; // bite
};
std::vector<ColDescription> m_ColumnList;
RECORD						m_ColumnResultList;
std::vector<RECORD>			m_RecordResultList;

SQLRETURN ret;
void Check(SQLHSTMT stmt)
{
	if (ret == SQL_SUCCESS) return;
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(g_hEnv, g_hDbc,
		stmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}
bool GetDiagRec(SQLHSTMT stmt)
{
	SQLRETURN ret;
	SQLSMALLINT		iCurrentRecord = 1, MsgLen;
	SQLTCHAR		SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER		NativeError;
	SQLLEN			numRecs = 0;

	SQLGetDiagField(SQL_HANDLE_STMT, stmt, 0, SQL_DIAG_NUMBER, &numRecs, 0, 0);

	while (iCurrentRecord <= numRecs)
	{
		if (ret = SQLGetDiagRec(SQL_HANDLE_STMT, stmt, iCurrentRecord, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
		{
			if (_tcscmp(SqlState, L"01004") == 0)
			{
				return true;
			}
			MessageBox(NULL, Msg, SqlState, MB_OK);
			iCurrentRecord++;
		}
	}
	return false;
}
void CreatePrepare()
{
	SQLRETURN ret;
	TCHAR sql[] = L"select * from DemoGame where userid=?";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hUserIDStmt);
	if (ret != SQL_SUCCESS)
	{
		return;
	}
	if (SQLPrepare(g_hUserIDStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}

	iNameLength = sizeof(g_szInputUserID);
	ZeroMemory(g_szInputUserID, iNameLength);
	cbColumn = SQL_NTS;

	ret = SQLBindParameter(g_hUserIDStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		iNameLength, 0, g_szInputUserID, iNameLength, &cbColumn);
	if (ret != SQL_SUCCESS)
	{
		Check(g_hUserIDStmt);
		return;
	}
	//ret = SQLBindCol(g_hUserIDStmt, 1, SQL_INTEGER, &g_iID, 0, &lID);
	//ret = SQLBindCol(g_hUserIDStmt, 2, SQL_UNICODE, (SQLPOINTER)g_szUserID, sizeof(g_szUserID), &lUserID);
	//ret = SQLBindCol(g_hUserIDStmt, 3, SQL_UNICODE, (SQLPOINTER)g_szUserPS, sizeof(g_szUserPS), &lUserPS);
}
SQLRETURN GetDescribeCol(SQLHSTMT stmt)
{
	SQLLEN count = 0;

	//UPDATE, INSERT, DELETE 문만 반환됨.
	SQLRETURN ret = SQLRowCount(stmt, &count);
	if (ret != SQL_SUCCESS)
	{
		Check(stmt);
	}
	//SQLSMALLINT colCount;
	//SQLNumResultCols(stmt, &colCount);

	m_ColumnList.clear();
	m_ColumnResultList.clear();
	m_RecordResultList.clear();

	ColDescription col;
	col.icol = 1;
	while (1)
	{
		ret = SQLDescribeCol(stmt,
			col.icol,
			col.szColName,
			sizeof(col.szColName),
			&col.pcchColName,
			&col.pfSqlType,
			&col.pcbColDef,
			&col.pibScale,
			&col.pfNullable);

		if (ret != SQL_SUCCESS)
		{
			//Check();
			break;
		}
		m_ColumnList.push_back(col);
		col.icol++;
	}
	return ret;
}
SQLRETURN GetColData(SQLHSTMT stmt, int column, std::wstring& rt)
{
	SQLRETURN ret;
	if (m_ColumnList[column - 1].pfSqlType != SQL_VARBINARY)
	{
		if (ret = SQLGetData(stmt, m_ColumnList[column - 1].icol, SQL_WCHAR,
			m_ColumnList[column - 1].bindData,
			sizeof(m_ColumnList[column - 1].bindData), NULL) == SQL_SUCCESS)
		{
			rt = m_ColumnList[column - 1].bindData;
		}
	}
	else
	{
		TCHAR buf[1000] = { 0, };
		SQLLEN cb;
		do {
			ZeroMemory(buf, sizeof(TCHAR) * 1000);
			ret = SQLGetData(stmt, m_ColumnList[column - 1].icol, SQL_WCHAR, buf, sizeof(buf), &cb);
			if (ret == SQL_NULL_DATA) break;
			rt += buf;
		} while (ret == SQL_SUCCESS_WITH_INFO);
	}
	return ret;
}
void main()
{
	setlocale(LC_ALL, "KOREAN");

	SQLHENV hEnv = SQL_NULL_HENV;
	SQLHENV hDbc = SQL_NULL_HDBC;
	SQLHENV hStmt = SQL_NULL_HSTMT;

	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	TCHAR InCon[256] = { 0, };
	TCHAR Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	GetCurrentDirectory(MAX_PATH, Dir);
	wsprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), Dir, L"DemoGame.accdb");
	ret = SQLDriverConnect(hDbc, NULL, InCon, _countof(InCon), NULL, 0, &cbCon, SQL_DRIVER_NOPROMPT);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (ret != SQL_SUCCESS)
	{
		return;
	}
	{
		SQLTCHAR userid[30] = { 0, };
		SQLTCHAR userps[30] = { 0, };
		SQLINTEGER id;
		SQLLEN lID, lUserID, IUserPS;
		SQLBindCol(hStmt, 1, SQL_INTEGER, &id, 0, &lID);
	}


}