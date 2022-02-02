/* 
 * Drag[en]gine Windows Launcher
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "WindowSplash.h"
#include "Launcher.h"
#include "Resource.h"
#include <stdexcept>

#define MAX_LOADSTRING 100

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
    UNREFERENCED_PARAMETER(lParam);
    switch(message){
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch(message){
    case WM_COMMAND:
        switch(LOWORD(wParam)){
        case IDM_ABOUT:
            DialogBox(Launcher::Get().GetInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case WM_PAINT:{
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
    
        EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

const wchar_t *WindowSplash::WindowClassName = L"LauncherWindowSplash";

WindowSplash::WindowSplash() :
pWindowClass(INVALID_ATOM),
pWindow(NULL)
{
	RegisterWindowClass();
    CreateSplashWindow();
    //ShowWindow(pWindow, SW_SHOWDEFAULT);
    //UpdateWindow(pWindow);
}

WindowSplash::~WindowSplash(){
}

void WindowSplash::RegisterWindowClass()
{
    const HINSTANCE hInstance = Launcher::Get().GetInstance();
	WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAUNCHER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAUNCHER);
    wcex.lpszClassName = WindowClassName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    pWindowClass = RegisterClassExW(&wcex);
    if(pWindowClass == INVALID_ATOM){
        throw std::runtime_error("RegisterClassExW failed");
    }
}

void WindowSplash::CreateSplashWindow(){
    const HINSTANCE hInstance = Launcher::Get().GetInstance();
	pWindow = CreateWindowW(WindowClassName, L"Launcher", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    
    if(pWindow == NULL){
        throw std::runtime_error("CreateWindowW failed");
    }
}
