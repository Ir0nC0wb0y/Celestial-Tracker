#include <Windows.h>
#include <tchar.h>

typedef void (*helloWorldPtr)(void);

HMODULE myDll = LoadLibrary(_T("C_Types_Hello_World.dll"));

helloWorldPtr helloWorld = NULL;

int main()
{
    helloWorld = (helloWorldPtr)GetProcAddress(myDll, "helloWorld");

    helloWorld();

    return 0;
}