#include "Define.h"

void InitCheat()
{
    FILE* ConsoleFile;
    AllocConsole();
    freopen_s(&ConsoleFile, "CONOUT$", "w", stdout);

    printf("Hello World");
}

BOOL APIENTRY DllMain( HMODULE ModuleHandle,DWORD  Reason,LPVOID Reserved)
{
    switch (Reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            InitCheat();
            break;
        }
    }
    return TRUE;
}

