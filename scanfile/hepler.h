

//#include<Windows.h>
//LPWSTR GetFormattedMessage(DWORD,LPWSTR);
int add(int,int);



void PrintFILETIME(PFILETIME);

/**
	@param LPCTSTR 文件或文件路径 
*/
void FindAllFileFrom(LPCTSTR,LPCTSTR);

/*
*Print all infomation of file
*/
void PrintFileInfo(PWIN32_FIND_DATA);

/*
* Get single attribute from attributes of file
*/
DWORD GetFileAttribute(DWORD);

/*
*判断后缀名是否为.tmp
*/
bool fun_IsTmpFile(LPWSTR);

void fun_WriteToFile(LPCTSTR,LPCTSTR,DWORD);

//拼接临时文件路径
void ComBineTempFileName(LPWSTR,LPCTSTR,LPWSTR);
//询问是否扫描垃圾文件
bool fun_AskIfScanFile(void);

