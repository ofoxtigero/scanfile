#include "stdafx.h"
#include<Shlwapi.h>
#include<wchar.h>
#include<Strsafe.h>

#include "deletemodule.h"
#include "uniconsole.h"
#define MAXSTRINGLEN 300
#define ROW_DELETE_STATUS TEXT("�������ļ�%d��,��%.2fM�ļ�\r\n��")
DWORD g_dword_deletefilecount = 0;

bool fun_AskIfDeleteFile(void)
{
	wchar_t wchar_YesOrNo;
	wprintf(L"�Ƿ���Ҫɾ��ϵͳ�����ļ�? ���밴 y �������밴 q ����\r\n");

	do
	{
		wscanf_s(L"%1c",&wchar_YesOrNo);
		getchar();
	}while((wchar_YesOrNo != L'y') && (wchar_YesOrNo != L'q'));


	//��������ַ� y �򷵻���
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

	//�����¼�ļ���С����
	DWORD dword_filelowsize =  0;
	DWORD dword_filehighsize =  0;
	//����һ���������ʼ��
	HANDLE handle_file = INVALID_HANDLE_VALUE;
	HANDLE handle_tmpfile = INVALID_HANDLE_VALUE;
	//����һ���ļ�,�Դ򿪴����ļ��ķ�ʽ��
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
			//����Ҫɾ���ļ��Ĵ�С
			dword_filelowsize += GetFileSize(handle_tmpfile,&dword_filehighsize);

				//�ر��ļ����
			FindClose(handle_tmpfile);
			//ɾ�������ļ�
			if(DeleteFile(ReadBuffer))
			{
				//ͳ���ж����ļ���ɾ��
				g_dword_deletefilecount++;

					//��ʽ���ַ���
				StringCchPrintf(szPrintInfo,
								MAXSTRINGLEN,
								ROW_DELETE_STATUS,
								g_dword_deletefilecount,
								dword_filelowsize/1024.0/1024.0);

			   //��տ���̨
				fun_ClearConsole(3,MAXSTRINGLEN);

				//���´�ӡɨ����Ϣ
				fun_PrintPos(0,3,szPrintInfo);
				
			}
			else
			{

				LPWSTR temp = GetFormattedMessage(GetLastError());
				wprintf_s(L"�޷�ɾ���ļ�ԭ��%s\r\n",temp);
				wprintf_s(L"�ļ����ƣ�%s\r\n",ReadBuffer);
				LocalFree(temp);
			}
			ldistance += (wcslen(ReadBuffer)+2);
			SetFilePointer(handle_file,ldistance*sizeof(wchar_t),NULL,FILE_BEGIN);
		}
	}
		//�ر��ļ����
	FindClose(handle_file);
}

/*
*���ܣ����ַ����н�ȡ��һ���ļ�������·��
*param:lpfilepath �����ļ�·�����ַ���
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