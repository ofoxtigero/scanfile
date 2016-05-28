#include "stdafx.h"
#include  "uniconsole.h"
void fun_ClearConsole(short short_row,DWORD dword_param_length)
{
	//获得控制台窗口句柄
	HANDLE handle_console = GetStdHandle(STD_OUTPUT_HANDLE); 
	
	//定义行列结构
	COORD coord_write;
	coord_write.X  = 0;
	coord_write.Y = short_row;

	DWORD NumberOfCharsWritten = 0;

	if(handle_console != NULL)
	{
		FillConsoleOutputCharacter(handle_console,TEXT(' '),dword_param_length,coord_write,&NumberOfCharsWritten);
	}
}

void fun_PrintPos(short column,short row,wchar_t *wchar_PrintBuffer)
{
	//获得控制台窗口句柄
	HANDLE handle_console = GetStdHandle(STD_OUTPUT_HANDLE); 
	
	//定义行列结构
	COORD coord_write;
	coord_write.X  = column;
	coord_write.Y = row;

	DWORD NumberOfCharsWritten = 0;

	if(handle_console != NULL)
	{

		WriteConsoleOutputCharacter(handle_console,wchar_PrintBuffer,wcslen(wchar_PrintBuffer),coord_write,&NumberOfCharsWritten);

	}
}
/**
@param errorcode
return description of errorcode
*/
LPWSTR GetFormattedMessage(DWORD dword_errorcode)
{
	LPWSTR pBuffer = NULL;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL,dword_errorcode,0,(LPWSTR)&pBuffer,0,NULL);

	return pBuffer;

}