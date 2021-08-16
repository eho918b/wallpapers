#include <windows.h>
#include <stdio.h>

static char buffer[20] = {0};

const char* time_now()
{
    static SYSTEMTIME lt;
    GetLocalTime(&lt);
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", lt.wHour, lt.wMinute, lt.wSecond);
    return buffer;
}