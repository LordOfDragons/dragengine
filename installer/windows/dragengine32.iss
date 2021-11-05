[Setup]
AppId={{870ACA91-F55D-415A-9EB0-5F7F7E4DE2CA}
AppName=Drag[en]gine Game Engine
AppVersion=1.7
;AppVerName=Drag[en]gine Game Engine 1.7
AppPublisher=Dragon Dreams
AppPublisherURL=https://dragondreams.ch
AppSupportURL=https://dragondreams.ch
AppUpdatesURL=https://dragondreams.ch
DefaultDirName={pf}\Dragengine
DisableDirPage=no
DefaultGroupName=Drag[en]gine Game Engine
DisableProgramGroupPage=no
OutputDir=build
OutputBaseFilename=install-dragengine-windows32
SetupIconFile=dragengine-installer.ico
Compression=lzma
SolidCompression=yes
ChangesAssociations=yes
PrivilegesRequired=admin

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Dirs]
Name: "{app}\Launchers\Games"; Permissions: users-modify

[Files]
Source: "unpacked\@ProgramFiles\Dragengine\Data\*"; DestDir: "{app}\Data"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "unpacked\@ProgramFiles\Dragengine\Launchers\*"; DestDir: "{app}\Launchers"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "unpacked\@ProgramFiles\Dragengine\Share\*"; DestDir: "{app}\Share"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "unpacked\@System\dragengine.dll"; DestDir: "{sys}"
Source: "unpacked\@System\delauncher.dll"; DestDir: "{sys}"
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Drag[en]gine GUI Launcher"; Filename: "{app}\Launchers\Bin\delauncher-gui.exe"
Name: "{group}\{cm:UninstallProgram,Drag[en]gine GUI Launcher}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Drag[en]gine GUI Launcher"; Filename: "{app}\Launchers\Bin\delauncher-gui.exe"; Tasks: desktopicon
Name: "{commonappdata}\Microsoft\Internet Explorer\Quick Launch\Drag[en]gine GUI Launcher"; Filename: "{app}\Launchers\Bin\delauncher-gui.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\Launchers\Bin\delauncher-gui.exe"; Flags: nowait postinstall skipifsilent; Description: "{cm:LaunchProgram,Drag[en]gine Game Engine}"

[Registry]
Root: "HKCR"; Subkey: ".delga"; ValueType: string; ValueData: "Drag[en]gine DELGA"; Flags: uninsdeletevalue
Root: "HKCR"; Subkey: "Drag[en]gine DELGA"; ValueType: string; ValueData: "Drag[en]gine DELGA"; Flags: uninsdeletekey
Root: "HKCR"; Subkey: "Drag[en]gine DELGA\DefaultIcon"; ValueType: string; ValueData: "{app}\Launchers\Bin\delauncher-gui.exe,0"
Root: "HKCR"; Subkey: "Drag[en]gine DELGA\shell\open\command"; ValueType: string; ValueData: """{app}\Launchers\Bin\delauncher-gui.exe"" ""%1"""; Flags: uninsdeletekey
Root: "HKCR"; Subkey: "Drag[en]gine DELGA\shell\install"; ValueType: string; ValueData: "Install"; Flags: uninsdeletekey
Root: "HKCR"; Subkey: "Drag[en]gine DELGA\shell\install\command"; ValueType: string; ValueData: """{app}\Launchers\Bin\delauncher-gui.exe"" --install ""%1"""
Root: "HKLM"; Subkey: "SOFTWARE\Drag[en]gine"; ValueType: string; ValueName: "PathEngine"; ValueData: "{app}"
Root: "HKLM"; Subkey: "SOFTWARE\Drag[en]gine"; ValueType: string; ValueName: "PathLauncherGames"; ValueData: "{app}\Launchers\Games"
