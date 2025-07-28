// Simple WinAPI Text Editor in C
// Compile under Linux with mingw-w64:
// x86_64-w64-mingw32-gcc lab3.c -o text_editor.exe -lcomdlg32

// Force ANSI (char*) versions of WinAPI functions
#undef UNICODE
#undef _UNICODE

#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <stdlib.h>

// Menu Command IDs
#define IDM_FILE_OPEN    100
#define IDM_FILE_SAVE    101
#define IDM_FILE_EXIT    102
#define IDM_EDIT_CUT     200
#define IDM_EDIT_COPY    201
#define IDM_EDIT_PASTE   202
#define IDM_HELP_ABOUT   300

HINSTANCE hInst;
HWND hEdit;
HACCEL hAccel;

// Function prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DoFileOpen(HWND);
void DoFileSave(HWND);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASSEX wc = {0};
    HWND hWnd;

    hInst = hInstance;

    // Register window class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "SimpleTextEditor";  // ANSI string
    wc.lpszMenuName  = NULL;
    RegisterClassEx(&wc);

    // Create main window
    hWnd = CreateWindowExA(0,
        "SimpleTextEditor",
        "Simple Text Editor",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL,
        hInstance, NULL);
    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Create accelerators
    ACCEL accelTable[] = {
        {FCONTROL, 'O', IDM_FILE_OPEN},
        {FCONTROL, 'S', IDM_FILE_SAVE},
        {FCONTROL, 'X', IDM_EDIT_CUT},
        {FCONTROL, 'C', IDM_EDIT_COPY},
        {FCONTROL, 'V', IDM_EDIT_PASTE},
        {0, VK_F1, IDM_HELP_ABOUT}
    };
    hAccel = CreateAcceleratorTableA(accelTable, sizeof(accelTable)/sizeof(ACCEL));

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAcceleratorA(hWnd, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        // Create multi-line edit control (ANSI)
        hEdit = CreateWindowExA(0, "EDIT", NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
            ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            0, 0, 0, 0,
            hWnd, (HMENU)1, hInst, NULL);

        // Create main menu
        HMENU hMenubar = CreateMenu();
        HMENU hFile = CreatePopupMenu();
        HMENU hEditMenu = CreatePopupMenu();
        HMENU hHelp = CreatePopupMenu();

        AppendMenuA(hFile, MF_STRING, IDM_FILE_OPEN, "&Open...\tCtrl+O");
        AppendMenuA(hFile, MF_STRING, IDM_FILE_SAVE, "&Save...\tCtrl+S");
        AppendMenuA(hFile, MF_SEPARATOR, 0, NULL);
        AppendMenuA(hFile, MF_STRING, IDM_FILE_EXIT, "E&xit");

        AppendMenuA(hEditMenu, MF_STRING, IDM_EDIT_CUT, "Cu&t\tCtrl+X");
        AppendMenuA(hEditMenu, MF_STRING, IDM_EDIT_COPY, "&Copy\tCtrl+C");
        AppendMenuA(hEditMenu, MF_STRING, IDM_EDIT_PASTE, "&Paste\tCtrl+V");

        AppendMenuA(hHelp, MF_STRING, IDM_HELP_ABOUT, "&About\tF1");

        AppendMenuA(hMenubar, MF_POPUP, (UINT_PTR)hFile, "&File");
        AppendMenuA(hMenubar, MF_POPUP, (UINT_PTR)hEditMenu, "&Edit");
        AppendMenuA(hMenubar, MF_POPUP, (UINT_PTR)hHelp, "&Help");

        SetMenu(hWnd, hMenubar);
        return 0;
    }

    case WM_SIZE:
        MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        return 0;

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDM_FILE_OPEN:
            DoFileOpen(hWnd);
            break;
        case IDM_FILE_SAVE:
            DoFileSave(hWnd);
            break;
        case IDM_FILE_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_EDIT_CUT:
            SendMessageA(hEdit, WM_CUT, 0, 0);
            break;
        case IDM_EDIT_COPY:
            SendMessageA(hEdit, WM_COPY, 0, 0);
            break;
        case IDM_EDIT_PASTE:
            SendMessageA(hEdit, WM_PASTE, 0, 0);
            break;
        case IDM_HELP_ABOUT:
            MessageBoxA(hWnd,
                "Simple Text Editor\nUsing WinAPI and mingw-gcc\nDemo: Wine & Windows",
                "About", MB_OK | MB_ICONINFORMATION);
            break;
        }
        return 0;
    }

    case WM_CONTEXTMENU: {
        if ((HWND)wParam == hEdit) {
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };
            HMENU hCtx = CreatePopupMenu();
            AppendMenuA(hCtx, MF_STRING, IDM_EDIT_CUT, "Cu&t");
            AppendMenuA(hCtx, MF_STRING, IDM_EDIT_COPY, "&Copy");
            AppendMenuA(hCtx, MF_STRING, IDM_EDIT_PASTE, "&Paste");
            TrackPopupMenu(hCtx, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
            DestroyMenu(hCtx);
        }
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hWnd, message, wParam, lParam);
}

void DoFileOpen(HWND hWnd) {
    OPENFILENAME ofn = {0};
    char szFile[260] = "";

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrTitle = "Open Text File";
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) {
        FILE *file = fopen(szFile, "r");
        if (file) {
            fseek(file, 0, SEEK_END);
            long size = ftell(file);
            fseek(file, 0, SEEK_SET);
            char *buffer = malloc(size + 1);
            fread(buffer, 1, size, file);
            buffer[size] = '\0';
            fclose(file);
            SetWindowTextA(hEdit, buffer);
            free(buffer);
        } else {
            MessageBoxA(hWnd, "Cannot open file.", "Error", MB_OK | MB_ICONERROR);
        }
    }
}

void DoFileSave(HWND hWnd) {
    OPENFILENAME ofn = {0};
    char szFile[260] = "";

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrTitle = "Save Text File";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameA(&ofn)) {
        int len = GetWindowTextLengthA(hEdit);
        char *buffer = malloc(len + 1);
        GetWindowTextA(hEdit, buffer, len + 1);
        FILE *file = fopen(szFile, "w");
        if (file) {
            fwrite(buffer, 1, len, file);
            fclose(file);
        } else {
            MessageBoxA(hWnd, "Cannot save file.", "Error", MB_OK | MB_ICONERROR);
        }
        free(buffer);
    }
}
