// scanfile.cpp : �������̨Ӧ�ó������ڵ㡣

#include "stdafx.h"
#include "hepler.h"
#include "uniconsole.h"
#include "deletemodule.h"
#define GARBAGEFILEPATH	TEXT("c:\\garbagefile2.txt")
int _tmain(int argc, _TCHAR* argv[])
{
	
	//���ó������Լ�
	_wsetlocale(LC_ALL,L"chs");
	

	//�Ƿ�ɨ��ϵͳ�����ļ�
	if (fun_AskIfScanFile())
	{
		//�ҵ����е������ļ�
		FindAllFileFrom(L"c:",GARBAGEFILEPATH);
	}

	//�Ƿ�ɾ��ϵͳ�����ļ�
	if (fun_AskIfDeleteFile())
	{
		//�ҵ����е������ļ�
		fun_DeleteFile(GARBAGEFILEPATH);
	}



	getchar();

	return 0;
}

