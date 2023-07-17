#include <stdafx.h>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <ShellAPI.h>
#include "iphlpapi.h"
#include <io.h>
#include <atltime.h>
#include <psapi.h>
#include <setupapi.h>
#include <initguid.h>
#include <cfgmgr32.h>
#include <dbt.h>
#include <winsvc.h>
#include <winioctl.h>
#include <initguid.h>

//app manage
#include <shellapi.h>
#include <tlhelp32.h>
#include <comdef.h>
#include "utils.h"

#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "PSAPI.lib")
#pragma comment(lib, "version.lib")

/*******************************************************
*	string convert
********************************************************/
std::wstring stringToWstring(const std::string & str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0){
		return std::wstring(L"");
	}

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst){
		return std::wstring(L"");
	}

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;
	return wstr;
}

std::string wstringToString(const std::wstring & wstr)
{
	LPCWSTR pwszSrc = wstr.c_str();
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0){
		return std::string("");
	}

	char* pszDst = new char[nLen];
	if (!pszDst){
		return std::string("");
	}

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	std::string str(pszDst);
	delete[] pszDst;
	pszDst = NULL;

	return str;
}


std::string utf8togbk(const char* szUtf8)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	WCHAR* wszGBK = new WCHAR[n + 1];
	wmemset(wszGBK, 0, n + 1);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszGBK, n);

	n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGbk = new char[n + 1];
	memset(szGbk, 0, n + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGbk, n, NULL, NULL);

	std::string retStr(szGbk);

	delete[]szGbk;
	delete[]wszGBK;

	return retStr;
}

std::string gbktoutf8(const char* szGbk)
{
	int n = MultiByteToWideChar(CP_ACP, 0, szGbk, -1, NULL, 0);
	WCHAR* wszGBK = new WCHAR[n + 1];
	wmemset(wszGBK, 0, n + 1);
	MultiByteToWideChar(CP_ACP, 0, szGbk, -1, wszGBK, n);

	n = WideCharToMultiByte(CP_UTF8, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szUtf8 = new char[n + 1];
	memset(szUtf8, 0, n + 1);
	WideCharToMultiByte(CP_UTF8, 0, wszGBK, -1, szUtf8, n, NULL, NULL);

	std::string retStr(szUtf8);

	delete[]szUtf8;
	delete[]wszGBK;

	return retStr;
}

void GetModuleFilePath(std::string& path)
{
	char strPath[260] = {0};
	DWORD dwRet = GetModuleFileNameA(NULL, strPath, MAX_PATH + 1);
	if (dwRet != 0)
	{
		path = strPath;
		int nPos = path.rfind('\\');
		path = path.substr(0,nPos);
	}
}

/// <summary>
/// time
/// </summary>
/// <returns></returns>
std::string getCurTimeString()
{
	char szCurrentDateTime[32];
	CTime nowtime;
	nowtime = CTime::GetCurrentTime();

	sprintf_s(szCurrentDateTime, "%04d-%.2d-%.2d %.2d:%.2d:%.2d",
		nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
		nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());
	std::string timeStr = szCurrentDateTime;
	return timeStr;
}

std::string getCurTimeString2()
{
	char szCurrentDateTime[32];
	CTime nowtime;
	nowtime = CTime::GetCurrentTime();

	sprintf_s(szCurrentDateTime, "%d", nowtime.GetTickCount());
	std::string timeStr = szCurrentDateTime;
	return timeStr;
}

std::string getCurTimeHourString()
{
	char szCurrentDateTime[32];
	CTime nowtime;
	nowtime = CTime::GetCurrentTime();

	sprintf_s(szCurrentDateTime, "%04d%.2d%.2d%.2d",
		nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
		nowtime.GetHour());
	std::string timeStr = szCurrentDateTime;
	return timeStr;
}

std::string formatTimeString(std::string timeStr)
{
	std::string newTimeStr;
	if (timeStr.length() >= 14){
		newTimeStr = timeStr.substr(0, 4) + "-" +
			timeStr.substr(4, 2) + "-" +
			timeStr.substr(6, 2) + " " +
			timeStr.substr(8, 2) + ":" +
			timeStr.substr(10, 2) + ":" +
			timeStr.substr(12, 2);
	}
	
	return newTimeStr;
}

std::string compresstTimeString(std::string timeStr)
{
	//2022-01-02 03:04:05
	std::string newTimeStr;
	if (timeStr.length() >= 19){
		newTimeStr = timeStr.substr(0, 4) +
			timeStr.substr(5, 2) +
			timeStr.substr(8, 2) +
			timeStr.substr(11, 2) +
			timeStr.substr(14, 2) +
			timeStr.substr(17, 2);
	}
	//printf("newTimeStr=%s\r\n", newTimeStr.c_str());
	return newTimeStr;
}

