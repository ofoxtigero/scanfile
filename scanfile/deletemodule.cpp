#include "stdafx.h"
#include<Shlwapi.h>
#include<wchar.h>
#include<Strsafe.h>

#include "deletemodule.h"
#include "uniconsole.h"
#define MAXSTRINGLEN 300
#define ROW_DELETE_STATUS TEXT("共垃圾文件%d个,共%.2fM文件\r\n。")
DWORD g_dword_deletefilecount = 0;

bool fun_AskIfDeleteFile(void)
{
	wchar_t wchar_YesOrNo;
	wprintf(L"是否需要删除系统垃圾文件? 是请按 y 键，否请按 q 键。\r\n");

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

void fun_DeleteFile(LPCTSTR lpFileName)
{
	DWORD  dwBytesRead = 0;
    wchar_t   ReadBuffer[MAXSTRINGLEN] = {TEXT('\0')};
	wchar_t   szPrintInfo[MAXSTRINGLEN] = {TEXT('\0')};
	LONG ldistance = 0;
	LONG long_temp=0;

	//定义记录文件大小变量
	DWORD dword_filelowsize =  0;
	DWORD dword_filehighsize =  0;
	//定义一个句柄并初始化
	HANDLE handle_file = INVALID_HANDLE_VALUE;
	HANDLE handle_tmpfile = INVALID_HANDLE_VALUE;
	//创建一个文件,以打开存在文件的方式打开
	handle_file = CreateFile(lpFileName,
								GENERIC_ALL,
								FILE_SHARE_WRITE,
								NULL,OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);



	while(false != ReadFile(handle_file,ReadBuffer,MAXSTRINGLEN-1,&dwBytesRead,NULL))
	{
		if (dwBytesRead > 0 || dwBytesRead <= MAXSTRINGLEN)
		{
			ReadBuffer[dwBytesRead]='\0'; // NULL character

			
			if ( 0 == fun_GetFilePathFromString(ReadBuffer))
			{
				break;
			}
			handle_tmpfile = CreateFile(ReadBuffer,
								GENERIC_ALL,
								FILE_SHARE_WRITE,
								NULL,OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
			//计算要删除文件的大小
			dword_filelowsize += GetFileSize(handle_tmpfile,&dword_filehighsize);

				//关闭文件句柄
			FindClose(handle_tmpfile);
			//删除垃圾文件
			if(DeleteFile(ReadBuffer))
			{
				//统计有多少文件被删除
				g_dword_deletefilecount++;

					//格式化字符串
				StringCchPrintf(szPrintInfo,
								MAXSTRINGLEN,
								ROW_DELETE_STATUS,
								g_dword_deletefilecount,
								dword_filelowsize/1024.0/1024.0);

			   //清空控制台
				fun_ClearConsole(3,MAXSTRINGLEN);

				//重新打印扫描信息
				fun_PrintPos(0,3,szPrintInfo);
				
			}
			else
			{

				LPWSTR temp = GetFormattedMessage(GetLastError());
				wprintf_s(L"无法删除文件原因：%s\r\n",temp);
				wprintf_s(L"文件名称：%s\r\n",ReadBuffer);
				LocalFree(temp);
			}
			ldistance += (wcslen(ReadBuffer)+2);
			SetFilePointer(handle_file,ldistance*sizeof(wchar_t),NULL,FILE_BEGIN);
		}
	}
		//关闭文件句柄
	FindClose(handle_file);
}

/*
*功能：从字符串中截取第一个文件的完整路径
*param:lpfilepath 包含文件路径的字符串
*/
long fun_GetFilePathFromString(wchar_t  * lpfilepath)
{
	LONG dword_filepathsize = 0;
	size_t filesize=wcslen(lpfilepath);
	
	if(filesize < 1)
		return 0;

	while(true)
	{
		if(lpfilepath[dword_filepathsize] == L'\r' || lpfilepath[dword_filepathsize] == L'\n')
		{
			lpfilepath[dword_filepathsize] = L'\0';
			break;
		}
		dword_filepathsize++;
	}

	return dword_filepathsize;
}