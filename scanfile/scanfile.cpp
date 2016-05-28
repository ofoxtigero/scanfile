// scanfile.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include "hepler.h"
#include "uniconsole.h"
#include "deletemodule.h"
#define GARBAGEFILEPATH	TEXT("c:\\garbagefile2.txt")
int _tmain(int argc, _TCHAR* argv[])
{
	
	//设置程序语言集
	_wsetlocale(LC_ALL,L"chs");
	

	//是否扫描系统垃圾文件
	if (fun_AskIfScanFile())
	{
		//找到所有的垃圾文件
		FindAllFileFrom(L"c:",GARBAGEFILEPATH);
	}

	//是否删除系统垃圾文件
	if (fun_AskIfDeleteFile())
	{
		//找到所有的垃圾文件
		fun_DeleteFile(GARBAGEFILEPATH);
	}



	getchar();

	return 0;
}

