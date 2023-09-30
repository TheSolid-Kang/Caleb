#pragma once
#include <windows.h>
#include <iostream>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>

class MSSQLMgr
{
#define DEFAULT_CAP 2048
#if UNICODE 
    using TString = std::wstring;
    using tifstream = std::wifstream;
    using tofstream = std::wofstream;
    using tistringstream = std::wistringstream;
    using tstring_view = std::wstring_view;
    using tstringstream = std::wstringstream;
#define tcout  wcout
#define tcin  wcin
#else
    using TString = std::string;
    using tifstream = std::ifstream;
    using tofstream = std::ofstream;
    using tistringstream = std::istringstream;
    using tstring_view = std::string_view;
    using tstringstream = std::stringstream;
#define tcout  cout
#define tcin  cin
#endif;
public:
    MSSQLMgr() {
        // ODBC API 함수 호출 전 초기화
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
        SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
        SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    }
public:
    // ODBC 관련 핸들 변수
    SQLHENV henv;//SQL 핸들 
    SQLHDBC hdbc;//SQL 핸들 
    SQLHSTMT hstmt;//SQL 핸들 Statment
    /*
    * 원본 코드
    typedef SQLHANDLE               SQLHENV;
    typedef SQLHANDLE               SQLHDBC;
    typedef SQLHANDLE               SQLHSTMT;
    typedef SQLHANDLE               SQLHDESC;
    */
public:
    /// <summary>
    /// SQLAllocHandle() 함수: ODBC 핸들을 할당한다.
    /// SQLSetEnvAttr() 함수: ODBC 환경설정을 변경한다.
    /// SQLDriverConnect() 함수: MSSQL Server 데이터베이스에 연결한다.
    /// SQLExecDirect() 함수: SQL 문당을 실행한다. 
    /// SQLFetch()함수: 결과 집합의 다음 행을 가져온다. 
    /// SQLGetData() 함수: 결과 집합의 열 값을 가져온다. 
    /// SQLFreeHandle() 함수: ODBC 핸들을 해제한다.
    /// 
    /// 
    /// </summary>
    /// <param name="_str"></param>
    void SelectQuery(std::string _str) {
        //ODBC 환경 핸들 생성
        SQLHENV sqlHenv;
        SQLRETURN sqlReturn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlHenv);

        //ODBC 버전 설정
        sqlReturn = SQLSetEnvAttr(sqlHenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

        //DB연결 핸들 생성
        SQLHDBC sqlHandleDbConnection;
        sqlReturn = SQLAllocHandle(SQL_HANDLE_DBC, sqlHenv, &sqlHandleDbConnection);

        //연결 문자열 생성
        SQLTCHAR* sqlConnectionString = (SQLTCHAR*)_T("DRIVER={SQL Server};SERVER=localhost;DATABASE=TwoMites;UID=sa;PWD=qp06910691!");// SQLWCHAR == SQLTCHAR

        //DB에 연결
        sqlReturn = SQLDriverConnect(sqlHandleDbConnection, NULL, sqlConnectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

        //쿼리 실행을 위한 문장 핸들 생성
        SQLHSTMT sqlHandleStatment;
        sqlReturn = SQLAllocHandle(SQL_HANDLE_STMT, sqlHandleDbConnection, &sqlHandleStatment);

        //SQL문 실행
        SQLTCHAR arrQury[] = _T("SELECT * FROM _TFBible");
        sqlReturn = SQLExecDirect(sqlHandleStatment, arrQury, SQL_NTS);


        //INSERT, UPDATE, DELETE 
        /*
        SQLTCHAR arrQuery2[] = _T("INSERT INTO _TFUser(ChurchSeq,  ResidID, UserName, Empid, IsAdministrator, IsSaved, LastUserSeq, LastDateTime) VALUES(1,N\'caleb0109\', N\'강태경\', N\'0691\', 1,1,1,GETDATE()); ");
        sqlReturn = SQLExecDirect(sqlHandleStatment, arrQuery2, SQL_NTS);
        SQLExecute(sqlHandleStatment);
        if (sqlReturn == SQL_SUCCESS || sqlReturn == SQL_SUCCESS_WITH_INFO) {

            std::tcout << _T("성공") << std::endl;
        }
        else {
            std::tcout << _T("실패") << std::endl;
        }
        */

        //핸들 해제
        SQLDisconnect(sqlHandleDbConnection);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlHandleDbConnection);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlHenv);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlHandleStatment);



        //결과 가져오기; SELECT 
        if (sqlReturn == SQL_SUCCESS || sqlReturn == SQL_SUCCESS_WITH_INFO) {
            SQLTCHAR Testament[50];
            while (SQLFetch(sqlHandleStatment) == SQL_SUCCESS) {
                SQLGetData(sqlHandleStatment, 2, SQL_C_TCHAR, Testament, 50, NULL);
                std::tcout << _T("") << Testament << std::endl;
            }
        }


    }

};

