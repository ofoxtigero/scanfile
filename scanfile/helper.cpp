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
#define ROW_SCAN_STATUS TEXT("��ɨ��%d��Ŀ¼,%d���ļ�,���������ļ�%d��,��%.2fM�ļ�\r\n��")

void PrintFILETIME(PFILETIME pfileTime)
{
	ULARGE_INTEGER ulargeint;
	ulargeint.HighPart = pfileTime->dwHighDateTime;
	ulargeint.LowPart = pfileTime->dwLowDateTime;

	wprintf_s(L"ʱ�䳤��Ϊ��%llu\r\n",ulargeint.QuadPart);

}

/*
*��ӡ�ļ���Ϣ
*/
void PrintFileInfo(PWIN32_FIND_DATA pFindData)
{
	//�������Ĳ���Ϊ�գ��򷵻�
	if (pFindData == NULL)
		return;
	LPCTSTR lpcstr_filename = pFindData->cFileName;
	FILETIME filetime_createtime = pFindData->ftCreationTime;

	/*ULARGE_INTEGER ulargeint;
			ULONGLONG ull_filesize;
			wprintf_s(TEXT("�����ļ��ɹ� �ļ���Ϊ��%s  %s\r\n"),FindFileData.cFileName,FindFileData.cAlternateFileName);
			ft = FindFileData.ftCreationTime;
			ull_filesize = ( FindFileData.nFileSizeHigh * (0x10000)) + FindFileData.nFileSizeLow;
			wprintf_s(TEXT("�����ļ��ɹ� �ļ���Ϊ��%u b \r\n"),ull_filesize);

			if(FileTimeToSystemTime(&ft,&SystemTime))
			{
				wprintf_s(TEXT("�����ļ��ɹ� �ļ�����ʱ�䰴Ϊ��%d��%d��%d�� %d:%d:%d\r\n"),SystemTime.wYear,
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
	//����һ��������������ܷ��ص��ļ����
	HANDLE  handle_file = INVALID_HANDLE_VALUE;

	//�����ļ����ݽṹ���������շ����ļ�����ϸ��Ϣ
	WIN32_FIND_DATA FindFileData;

	//�����ļ�����·��
	WCHAR szFindFilePath[MAXSTRINGLEN]={L"\0"};

	//���������ļ�ȫ·������
	WCHAR szFindTmpFilePath[MAXSTRINGLEN]={L"\0"};

	WCHAR szPrintInfo[MAXSTRINGLEN]={L"\0"};
	//���嵱ǰĿ¼
	WCHAR wchar_dot = L'.';


	//������
	if(lpFileName == NULL || wcslen(lpFileName) == 0)
	{
		wprintf_s(L"����FindAllFileFrom�����������");
		return;
	}

	//��ʼ�������ļ�·��
	wcscpy_s(szFindFilePath,lpFileName);
	wcscat_s(szFindFilePath,L"\\*.*");

	//���Ҹ�Ŀ¼�µĵ�һ���ļ�������þ��
	handle_file = FindFirstFile(szFindFilePath,&FindFileData);

	//����Ҳ����ļ����򷵻�
	if (handle_file == INVALID_HANDLE_VALUE)
		return;

	do
	{
			if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != wchar_dot)
			{
				//������Ŀ¼����1
				g_dword_dircount++;
				wcscpy_s(szFindFilePath,lpFileName);
				wcscat_s(szFindFilePath,L"\\");
				wcscat_s(szFindFilePath,FindFileData.cFileName);

				//��ʽ���ַ���
				StringCchPrintf(szPrintInfo,
								MAXSTRINGLEN,
								ROW_SCAN_STATUS,
								g_dword_dircount,
								g_dword_filecount,
								g_dword_tmpfilecount,
								g_dword_tmpfilesize/1024.0/1024.0);

				//��տ���̨

				fun_ClearConsole(1,MAXSTRINGLEN);

				//���´�ӡɨ����Ϣ
				fun_PrintPos(0,1,szPrintInfo);

				FindAllFileFrom(szFindFilePath,lpStoreFilePath);
			}
			else if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				//�������ļ�����1
				g_dword_filecount++;

				//��ʽ���ַ���
				StringCchPrintf(szPrintInfo,
								MAXSTRINGLEN,
								ROW_SCAN_STATUS,
								g_dword_dircount,
								g_dword_filecount,
								g_dword_tmpfilecount,
								g_dword_tmpfilesize/1024.0/1024.0);

				//��տ���̨

				fun_ClearConsole(1,MAXSTRINGLEN);

				//���´�ӡɨ����Ϣ
				fun_PrintPos(0,1,szPrintInfo);

				if(fun_IsTmpFile(FindFileData.cFileName))
				{
					//�������������ļ�����1
					g_dword_tmpfilecount++;
					//szFindTmpFilePath�����ļ���
					ComBineTempFileName(szFindTmpFilePath,szFindFilePath,FindFileData.cFileName);
					
					wcscat_s(szFindTmpFilePath,L"\r\n");

					/*ULONGLONG ull_filesize;
			wprintf_s(TEXT("�����ļ��ɹ� �ļ���Ϊ��%s  %s\r\n"),FindFileData.cFileName,FindFileData.cAlternateFileName);
			ft = FindFileData.ftCreationTime;
			ull_filesize = ( FindFileData.nFileSizeHigh * (0x10000)) + FindFileData.nFileSizeLow;*/

						g_dword_tmpfilesize += FindFileData.nFileSizeLow;
						//��ʽ���ַ���
						StringCchPrintf(szPrintInfo,
										MAXSTRINGLEN,
										ROW_SCAN_STATUS,
										g_dword_dircount,
										g_dword_filecount,
										g_dword_tmpfilecount,
										g_dword_tmpfilesize/1024.0/1024.0);

						//��տ���̨

						fun_ClearConsole(1,MAXSTRINGLEN);

						//���´�ӡɨ����Ϣ
						fun_PrintPos(0,1,szPrintInfo);

					
					fun_WriteToFile(lpStoreFilePath,szFindTmpFilePath,wcslen(szFindTmpFilePath)*sizeof(wchar_t));
				}
			}
		}while(FindNextFile(handle_file,&FindFileData));

	//�ر��ļ����
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
	

	//����һ���������ʼ��
	HANDLE handle_file = INVALID_HANDLE_VALUE;

	//����һ���ļ�,�Դ򿪴����ļ��ķ�ʽ��
	handle_file = CreateFile(lpFileName,
								GENERIC_ALL,
								FILE_SHARE_WRITE,
								NULL,OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	//���Ҫ�򿪵��ļ������ڣ���ô�����µ��ļ�
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
			wprintf_s(L"�����ļ���%sʧ�ܣ�,����ԭ��%s��",lpFileName,GetFormattedMessage(dw_errorcode));
			return;
		}
		else
		{
				//ִ�е�˵���ļ��򿪻��ߴ������򿪳ɹ�
				
		}
	}
	else
	{
		//���ļ�ָ�����õ�ĩβ
		SetFilePointer(handle_file,0,NULL,FILE_END);
	}

	DWORD writedBufferLen=0;
	WriteFile(handle_file,lpTempFileName,dwFileNameLength,&writedBufferLen,NULL);

	CloseHandle(handle_file);




}

/*
 *�ϳ���ʱ�ļ�·��
*/
void ComBineTempFileName(LPWSTR pTmpFilePath, LPCTSTR pFilePath,LPWSTR pFileName)
{
	//�������Ϸ���
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
	wprintf(L"�Ƿ���Ҫɨ��ϵͳ�����ļ�? ���밴 y �������밴 q ����\r\n");

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
