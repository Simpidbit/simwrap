#include "simwinapi.h"

UINT
click_key(WORD keyvalue)
{
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = keyvalue;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = keyvalue;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    return uSent;
}
