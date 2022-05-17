#include "Define.h"

Vector3 Rainbow()
{
    static uint32_t cnt = 0;
    float freq = 0.002f;

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    Vector3 ret = { std::sin(freq * cnt + 0) * 0.5f + 0.5f, std::sin(freq * cnt + 2) * 0.5f + 0.5f , std::sin(freq * cnt + 4) * 0.5f + 0.5f };
    return ret;
}

__declspec(naked) void ChamHook() 
{
    __asm
    {
        mov This, edi
    }

    if (This)
    CurrentMeshName = *(const char**)(This);

    if (CurrentMeshName)
    {

        if (strcmp(CurrentMeshName, "weapon") == NULL || strcmp(CurrentMeshName, "hands") == NULL)
        {
            GlDepthFunc(Gl_ALWAYS);

            GlDisableClientState(Gl_TEXTURE_COORD_ARRAY);
            GlBindTexture(Gl_TEXTURE_2D, 0);

            GlBlendFunc(Gl_SRC_ALPHA, Gl_ONE_MINUS_SRC_ALPHA);
            GlEnable(Gl_BLEND);

            GlDisableClientState(Gl_COLOR_ARRAY);
            GlColor4f(1.0f, 0.0f, 0.0f, 0.8f);

            if (strcmp(CurrentMeshName, "hands") == NULL)
            {
                GlColor4f(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }
        else if (strcmp(CurrentMeshName, "akimbo") == NULL || strcmp(CurrentMeshName, "health") == NULL || strcmp(CurrentMeshName, "ammobox") == NULL || strcmp(CurrentMeshName, "pistolclips") == NULL || strcmp(CurrentMeshName, "helmet") == NULL || strcmp(CurrentMeshName, "kevlar") == NULL || strcmp(CurrentMeshName, "nade") == NULL)
        {
            GlDepthFunc(Gl_ALWAYS);

            GlDisableClientState(Gl_TEXTURE_COORD_ARRAY);
            GlBindTexture(Gl_TEXTURE_2D, 0);

            GlBlendFunc(Gl_SRC_ALPHA, Gl_ONE_MINUS_SRC_ALPHA);
            GlEnable(Gl_BLEND);

            GlDisableClientState(Gl_COLOR_ARRAY);
            GlColor4f(1.0f, 1.0f, 0.0f, 0.8f);
        }
        else
        {
            GlDepthFunc(Gl_LESS);
        }
    }
    else
    {
        GlDepthFunc(Gl_ALWAYS);

        GlDisableClientState(Gl_TEXTURE_COORD_ARRAY);
        GlBindTexture(Gl_TEXTURE_2D, 0);

        GlBlendFunc(Gl_SRC_ALPHA, Gl_ONE_MINUS_SRC_ALPHA);
        GlEnable(Gl_BLEND);

        GlDisableClientState(Gl_COLOR_ARRAY);
        GlColor4f(RainbowColor.x, RainbowColor.y, RainbowColor.z, 0.6f);
    }

    __asm
    {
        mov Test, [0x57E4C8]
        cmp Test, 0
        jmp ReturnAddress
    }
}

BOOL __stdcall RenderHook(HDC HardwareDevice)
{
    if (!Once)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplWin32_Init(FindWindowA(NULL, "AssaultCube"));
        ImGui_ImplOpenGL2_Init();
        Once = true;
    }

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (ShowMenu)
    {
        SDLShowCursor(true);
        ShowCursor(true);

        ImGuiIO Io = ImGui::GetIO();
        ImGui::GetForegroundDrawList()->AddRectFilled({Io.MousePos.x - 1, Io.MousePos.y - 1 } , { Io.MousePos.x + 1, Io.MousePos.y + 1 }  ,ImColor(1.0f,1.0f,1.0f,1.0f));

        ImGui::Begin("Undetecable");

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    return GlSwapBuffers(HardwareDevice);
}

void CheatThread()
{
    OpenGlHandle = GetModuleHandleA("opengl32.dll");
    SDLHandle = GetModuleHandleA("SDL2.dll");

    if (SDLHandle && OpenGlHandle)
    {
        SDLSetCursor = (TypeSetCursor)GetProcAddress(SDLHandle, "SDL_SetCursor");
        SDLCreateCursor = (TypeCreateCursor)GetProcAddress(SDLHandle, "SDL_CreateCursor");
        SDLShowCursor = (TypeShowCursor)GetProcAddress(SDLHandle, "SDL_ShowCursor");
        GlSwapBuffers = (TypeGlSwapBuffers)GetProcAddress(OpenGlHandle, "wglSwapBuffers");
        GlDepthFunc = (TypeGlDepthFunc)GetProcAddress(OpenGlHandle, "glDepthFunc");
        GlDisableClientState = (TypeGlDisableClientState)GetProcAddress(OpenGlHandle, "glDisableClientState");
        GlBindTexture = (TypeGlBindTexture)GetProcAddress(OpenGlHandle, "glBindTexture");
        GlEnable = (TypeGlEnable)GetProcAddress(OpenGlHandle, "glEnable");
        GlDisable = (TypeGlDisable)GetProcAddress(OpenGlHandle, "glDisable");
        GlColor4f = (TypeGlColor4f)GetProcAddress(OpenGlHandle, "glColor4f");
        GlBlendFunc = (TypeGlBlendFunc)GetProcAddress(OpenGlHandle, "glBlendFunc");

        //ChamHook
        DWORD TempProtection;
        UINT32 ChamsHookAddress = 0x414568;
        VirtualProtect((void*)ChamsHookAddress, 5, PAGE_EXECUTE_READWRITE, &TempProtection);
        *(UINT8*)(ChamsHookAddress + 0x0) = 0xE9;
        *(UINT32*)(ChamsHookAddress + 0x1) = (UINT32)&ChamHook - ((UINT32)ChamsHookAddress + 0x5);
        ReturnAddress = (UINT32)(0x41456F);

        //RenderHook
        UINT32 RenderHookAddress = (UINT32)GlSwapBuffers;
        GlSwapBuffers = (TypeGlSwapBuffers)((UINT32)GetProcAddress(OpenGlHandle, "wglSwapBuffers") + 5);
        VirtualProtect((void*)RenderHookAddress, 5, PAGE_EXECUTE_READWRITE, &TempProtection);
        *(UINT8*)(RenderHookAddress + 0x0) = 0xE9;
        *(UINT32*)(RenderHookAddress + 0x1) = (UINT32)&RenderHook - ((UINT32)RenderHookAddress + 0x5);

        UINT8 HiddenCursorData = NULL;
        UINT32* Cursor = SDLCreateCursor(&HiddenCursorData, &HiddenCursorData, 8, 1, NULL, NULL);
        SDLSetCursor(Cursor);
        SDLShowCursor(true);
    }

    while (true)
    {
        RainbowColor = Rainbow();

        if(GetAsyncKeyState(VK_INSERT))
        { 
            ShowMenu = !ShowMenu;
        }

		Sleep(1);
    }
}

void InitCheat()
{
    FILE* ConsoleFile;
    AllocConsole();
    freopen_s(&ConsoleFile, "CONOUT$", "w", stdout);

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheatThread, NULL, 0, NULL);
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

