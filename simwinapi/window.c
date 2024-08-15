#include "simwinapi.h"

char *
get_front_window_title_simpid()
{
    HWND hd;
    int len;
    char title[1024] = { 0 };

    hd = GetForegroundWindow(); 
    len = GetWindowTextLength(hd); 
    GetWindowTextA(hd, title, len + 1);
    return title;
}
