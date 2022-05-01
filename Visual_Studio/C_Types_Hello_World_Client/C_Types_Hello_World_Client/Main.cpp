#include <Windows.h>
#include <tchar.h>
//#include "HelloWorld.h"

typedef void (*helloWorld_ptr)(void);

HMODULE myDll = LoadLibrary(_T("C_Types_Hello_World.dll"));

helloWorld_ptr helloWorlddll = NULL;

int main()
{
    helloWorlddll = (helloWorld_ptr)GetProcAddress(myDll, "helloWorld");

    helloWorlddll();
    return 0;
}