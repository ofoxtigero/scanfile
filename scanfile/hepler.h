

//#include<Windows.h>
//LPWSTR GetFormattedMessage(DWORD,LPWSTR);
int add(int,int);



void PrintFILETIME(PFILETIME);

/**
	@param LPCTSTR �ļ����ļ�·�� 
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
*�жϺ�׺���Ƿ�Ϊ.tmp
*/
bool fun_IsTmpFile(LPWSTR);

void fun_WriteToFile(LPCTSTR,LPCTSTR,DWORD);

//ƴ����ʱ�ļ�·��
void ComBineTempFileName(LPWSTR,LPCTSTR,LPWSTR);
//ѯ���Ƿ�ɨ�������ļ�
bool fun_AskIfScanFile(void);

