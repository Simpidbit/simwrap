#include "simwinapi.h"

void
clear_console_simpid()
{
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO bufInfo;
    SMALL_RECT scroll;
    COORD newCursorPointer;
    CHAR_INFO ciFill;
    
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if(!GetConsoleScreenBufferInfo(hStdOut, &bufInfo)) return;
    
    scroll.Left = 0;
    scroll.Top = 0;
    scroll.Right = bufInfo.dwSize.X;
    scroll.Bottom = bufInfo.dwSize.Y;
    newCursorPointer.X = 0;
    newCursorPointer.Y = -bufInfo.dwSize.Y;
    ciFill.Char.UnicodeChar = L' ';
    ciFill.Attributes = bufInfo.wAttributes;
    ScrollConsoleScreenBufferW(hStdOut, &scroll, NULL, newCursorPointer, &ciFill);
    newCursorPointer.Y = 0;
    SetConsoleCursorPosition(hStdOut, newCursorPointer);
}


