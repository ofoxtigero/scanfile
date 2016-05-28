#include "stdafx.h"
#include"hepler.h"
#include "uniconsole.h"
#include<Shlwapi.h>
#include<wchar.h>
#include<Strsafe.h>
#define PER_SECOND 10*1000*1000
#define MAXSTRINGLEN 300
DWORD g_dword_dircount=0;
DWORD g_dword_filecount = 0;
DWORD g_dword_tmpfilecount = 0;
DWORD g_dword_tmpfilesize = 0;
#define ROW_SCAN_STATUS TEXT("共扫描%d个目录,%d个文件,其中垃圾文件%d个,共%.2fM文件\r\n。")

void PrintFILETIME(PFILETIME pfileTime)
{
	ULARGE_INTEGER ulargeint;
	ulargeint.HighPart = pfileTime->dwHighDateTime;
	ulargeint.LowPart = pfileTime->dwLowDateTime;

	wprintf_s(L"时间长度为：%llu\r\n",ulargeint.QuadPart);

}

/*
*打印文件信息
*/
void PrintFileInfo(PWIN32_FIND_DATA pFindData)
{
	//如果传入的参数为空，则返回
	if (pFindData == NULL)
		return;
	LPCTSTR lpcstr_filename = pFindData->cFileName;
	FILETIME filetime_createtime = pFindData->ftCreationTime;

	/*ULARGE_INTEGER ulargeint;
			ULONGLONG ull_filesize;
			wprintf_s(TEXT("查找文件成功 文件名为：%s  %s\r\n"),FindFileData.cFileName,FindFileData.cAlternateFileName);
			ft = FindFileData.ftCreationTime;
			ull_filesize = ( FindFileData.nFileSizeHigh * (0x10000)) + FindFileData.nFileSizeLow;
			wprintf_s(TEXT("查找文件成功 文件名为：%u b \r\n"),ull_filesize);

			if(FileTimeToSystemTime(&ft,&SystemTime))
			{
				wprintf_s(TEXT("查找文件成功 文件创建时间按为：%d年%d月%d日 %d:%d:%d\r\n"),SystemTime.wYear,
					SystemTime.wMonth,
					SystemTime.wDay,
					SystemTime.wHour,
					SystemTime.wMinute,
					SystemTime.wSecond);
			}*/

}

/**
	@param LPCTSTR:file ro directory
*/
void FindAllFileFrom(LPCTSTR lpFileName,LPCTSTR lpStoreFilePath)
{
	//定义一个句柄，用来接受返回的文件句柄
	HANDLE  handle_file = INVALID_HANDLE_VALUE;

	//定义文件数据结构，用来接收返回文件的详细信息
	WIN32_FIND_DATA FindFileData;

	//定义文件搜索路径
	WCHAR szFindFilePath[MAXSTRINGLEN]={L"\0"};

	//定义垃圾文件全路径名称
	WCHAR szFindTmpFilePath[MAXSTRINGLEN]={L"\0"};

	WCHAR szPrintInfo[MAXSTRINGLEN]={L"\0"};
	//定义当前目录
	WCHAR wchar_dot = L'.';


	//检查参数
	if(lpFileName == NULL || wcslen(lpFileName) == 0)
	{
		wprintf_s(L"函数FindAllFileFrom传入参数错误！");
		return;
	}

	//初始化搜索文件路径
	wcscpy_s(szFindFilePath,lpFileName);
	wcscat_s(szFindFilePath,L"\\*.*");

	//查找该目录下的第一个文件，并获得句柄
	handle_file = FindFirstFile(szFindFilePath,&FindFileData);

	//如果找不到文件，则返回
	if (handle_file == INVALID_HANDLE_VALUE)
		return;

	do
	{
			if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != wchar_dot)
			{
				//搜索的目录数加1
				g_dword_dircount++;
				wcscpy_s(szFindFilePath,lpFileName);
				wcscat_s(szFindFilePath,L"\\");
				wcscat_s(szFindFilePath,FindFileData.cFileName);

				//格式化字符串
				StringCchPrintf(szPrintInfo,
								MAXSTRINGLEN,
								ROW_SCAN_STATUS,
								g_dword_dircount,
								g_dword_filecount,
								g_dword_tmpfilecount,
								g_dword_tmpfilesize/1024.0/1024.0);

				//清空控制台

				fun_ClearConsole(1,MAXSTRINGLEN);

				//重新打印扫描信息
				fun_PrintPos(0,1,szPrintInfo);

				FindAllFileFrom(szFindFilePath,lpStoreFilePath);
			}
			else if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				//搜索的文件数加1
				g_dword_filecount++;

				//格式化字符串
				StringCchPrintf(szPrintInfo,
								MAXSTRINGLEN,
								ROW_SCAN_STATUS,
								g_dword_dircount,
								g_dword_filecount,
								g_dword_tmpfilecount,
								g_dword_tmpfilesize/1024.0/1024.0);

				//清空控制台

				fun_ClearConsole(1,MAXSTRINGLEN);

				//重新打印扫描信息
				fun_PrintPos(0,1,szPrintInfo);

				if(fun_IsTmpFile(FindFileData.cFileName))
				{
					//搜索到的垃圾文件数加1
					g_dword_tmpfilecount++;
					//szFindTmpFilePath等于文件名
					ComBineTempFileName(szFindTmpFilePath,szFindFilePath,FindFileData.cFileName);
					
					wcscat_s(szFindTmpFilePath,L"\r\n");

					/*ULONGLONG ull_filesize;
			wprintf_s(TEXT("查找文件成功 文件名为：%s  %s\r\n"),FindFileData.cFileName,FindFileData.cAlternateFileName);
			ft = FindFileData.ftCreationTime;
			ull_filesize = ( FindFileData.nFileSizeHigh * (0x10000)) + FindFileData.nFileSizeLow;*/

						g_dword_tmpfilesize += FindFileData.nFileSizeLow;
						//格式化字符串
						StringCchPrintf(szPrintInfo,
										MAXSTRINGLEN,
										ROW_SCAN_STATUS,
										g_dword_dircount,
										g_dword_filecount,
										g_dword_tmpfilecount,
										g_dword_tmpfilesize/1024.0/1024.0);

						//清空控制台

						fun_ClearConsole(1,MAXSTRINGLEN);

						//重新打印扫描信息
						fun_PrintPos(0,1,szPrintInfo);

					
					fun_WriteToFile(lpStoreFilePath,szFindTmpFilePath,wcslen(szFindTmpFilePath)*sizeof(wchar_t));
				}
			}
		}while(FindNextFile(handle_file,&FindFileData));

	//关闭文件句柄
	FindClose(handle_file);

}

bool fun_IsTmpFile(LPWSTR pFileName)
{
	LPWSTR pFileNameExtension = PathFindExtension(pFileName);

	if(pFileNameExtension == NULL || wcslen(pFileNameExtension)==0)
		return false;
	if(_wcsicmp(pFileNameExtension,L".tmp") ==0 
		|| _wcsicmp(pFileNameExtension,L".gid") ==0 
		|| _wcsicmp(pFileNameExtension,L".old") ==0 
		|| _wcsicmp(pFileNameExtension,L"._mp") ==0 )
	{
		return true;
	}
	return  false;
}

void fun_WriteToFile(LPCTSTR lpFileName,LPCTSTR lpTempFileName,DWORD dwFileNameLength)
{
	

	//定义一个句柄并初始化
	HANDLE handle_file = INVALID_HANDLE_VALUE;

	//创建一个文件,以打开存在文件的方式打开
	handle_file = CreateFile(lpFileName,
								GENERIC_ALL,
								FILE_SHARE_WRITE,
								NULL,OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	//如果要打开的文件不存在，那么创建新的文件
	if ( handle_file == INVALID_HANDLE_VALUE )
	{
		handle_file = CreateFile(lpFileName,
								GENERIC_ALL,
								FILE_SHARE_WRITE,NULL,
								CREATE_NEW,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

		if(handle_file == INVALID_HANDLE_VALUE)
		{
			DWORD dw_errorcode = GetLastError();
			wprintf_s(L"创建文件：%s失败！,错误原因：%s。",lpFileName,GetFormattedMessage(dw_errorcode));
			return;
		}
		else
		{
				//执行到说明文件打开或者创建并打开成功
				
		}
	}
	else
	{
		//把文件指针设置到末尾
		SetFilePointer(handle_file,0,NULL,FILE_END);
	}

	DWORD writedBufferLen=0;
	WriteFile(handle_file,lpTempFileName,dwFileNameLength,&writedBufferLen,NULL);

	CloseHandle(handle_file);




}

/*
 *合成临时文件路径
*/
void ComBineTempFileName(LPWSTR pTmpFilePath, LPCTSTR pFilePath,LPWSTR pFileName)
{
	//检查参数合法性
	if (pFilePath == NULL || pFileName == NULL || pTmpFilePath == NULL)
		return;

	size_t iFilePathLength = wcslen(pFilePath);

	wcscpy(pTmpFilePath,pFilePath);

	pTmpFilePath[iFilePathLength-3] = '\0';

	wcscat(pTmpFilePath,pFileName);
	
}

bool fun_AskIfScanFile(void)
{
	wchar_t wchar_YesOrNo;
	wprintf(L"是否需要扫描系统垃圾文件? 是请按 y 键，否请按 q 键。\r\n");

	do
	{
		wscanf_s(L"%1c",&wchar_YesOrNo);
		getchar();
	}while((wchar_YesOrNo != L'y') && (wchar_YesOrNo != L'q'));


	//如果输入字符 y 则返回真
	if(wchar_YesOrNo == L'y')
	{
		return true;
	}
	
	return false;
}
