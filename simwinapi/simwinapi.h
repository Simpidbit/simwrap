#ifndef SIMPIDBIT_WINAPI_H
#define SIMPIDBIT_WINAPI_H

#include <windows.h>
#include <winuser.h>

void clear_console_simpid();
char * get_front_window_title_simpid();
UINT click_key(WORD keyvalue);      // keyvalue: 见https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes
HDESK create_desktop(wchar_t* dtitle);

#endif
