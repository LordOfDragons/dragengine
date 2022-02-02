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

#include "resource.h"
#include "Launcher.h"
#include "WindowSplash.h"
#include "LauncherIni.h"

#include <stdexcept>
#include <sstream>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.ViewManagement.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::ViewManagement;

Launcher *Launcher::pTheLauncher = nullptr;

Launcher::Launcher(HINSTANCE hInstance) :
pInstance(hInstance)
{
    pTheLauncher = this;
}

Launcher::~Launcher(){
    pTheLauncher = nullptr;
}

int Launcher::Run(){
    //pWindowSplash = std::make_unique<WindowSplash>();
    //return pRunMessageLoop();
    
    wchar_t executablePath[MAX_PATH];
    GetModuleFileNameW(pInstance, executablePath, MAX_PATH);

    pLauncherDirectory = executablePath;
    
    const std::wstring::size_type pathSeparator = pLauncherDirectory.rfind(L'\\');
    if(pathSeparator != std::wstring::npos){
        pLauncherDirectory = pLauncherDirectory.substr(0, pathSeparator);
    }

    pLauncherIni = std::make_unique<LauncherIni>(pLauncherDirectory + L"\\Launcher.ini");

    pLaunchDelga();
    
    return 0;
}

std::wstring Launcher::ToWString(const std::string& string){
    const int neededSize = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.size(), NULL, 0);
    std::wstring wstring(neededSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.size(), &wstring[0], neededSize);
    return wstring;
}

std::string Launcher::ToString(const std::wstring& string){
    const int neededSize = WideCharToMultiByte(CP_UTF8, 0, string.c_str(), (int)string.size(), NULL, 0, NULL, NULL);
    std::string cstring(neededSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, string.c_str(), (int)string.size(), &cstring[0], neededSize, NULL, NULL);
    return cstring;
}

int Launcher::pRunMessageLoop(){
    MSG msg;
    HACCEL hAccelTable = LoadAccelerators(pInstance, MAKEINTRESOURCE(IDC_LAUNCHER));

    while(GetMessage(&msg, nullptr, 0, 0)){
        if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

void Launcher::pExitApplication(){
    PostQuitMessage(0);
}

void Launcher::pLaunchDelga(){
    Windows::System::LauncherOptions options;
    options.PreferredApplicationPackageFamilyName(L"DragonDreams.Dragengine.GameEngine");
    options.PreferredApplicationDisplayName(L"Drag[en]gine GUI Launcher");
    options.DesiredRemainingView(ViewSizePreference::UseNone);
    options.TreatAsUntrusted(false);

    std::wstring uriString{L"delauncher:run?file="};
    uriString += pLauncherDirectory;
    uriString += L"\\";
    uriString += ToWString(pLauncherIni->Get("File"));

    Uri uri{uriString};

    if(Windows::System::Launcher::LaunchUriAsync(uri, options).get()){

    }else{
        throw std::runtime_error("Failed launching DELGA");
    }
}

/** Entry point. */
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
_In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    try{
        Launcher launcher(hInstance);
        return launcher.Run();

    }catch(const std::exception &e){
        std::wstring message(L"Failed running launcher:\n");
        message += Launcher::ToWString(std::string(e.what()));
        MessageBoxW(NULL, message.c_str(), L"Error", MB_OK);
        return -1;
    }
}