[Setup]
AppId={{636641D2-B432-415A-AE7F-6BB0AF42A86E}
AppName=Drag[en]gine IGDE
AppVersion=1.1
;AppVerName=Drag[en]gine IGDE 1.1
AppPublisher=Dragon Dreams
AppPublisherURL=https://dragondreams.ch
AppSupportURL=https://dragondreams.ch
AppUpdatesURL=https://dragondreams.ch
DefaultDirName={pf}\DEIGDE
DisableDirPage=no
DefaultGroupName=Drag[en]gine IGDE
DisableProgramGroupPage=no
OutputDir=build
OutputBaseFilename=install-deigde-windows64
SetupIconFile=deigde-installer.ico
Compression=lzma
SolidCompression=yes
ChangesAssociations=yes
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
PrivilegesRequired=admin

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "unpacked\@ProgramFiles\DEIGDE\Bin\*"; DestDir: "{app}\Bin"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "unpacked\@ProgramFiles\DEIGDE\Data\*"; DestDir: "{app}\Data"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "unpacked\@ProgramFiles\DEIGDE\Share\*"; DestDir: "{app}\Share"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Drag[en]gine IGDE"; Filename: "{app}\Bin\deigde.exe"
Name: "{group}\{cm:UninstallProgram,Drag[en]gine IGDE}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Drag[en]gine IGDE"; Filename: "{app}\Bin\deigde.exe"; Tasks: desktopicon
Name: "{commonappdata}\Microsoft\Internet Explorer\Quick Launch\Drag[en]gine IGDE"; Filename: "{app}\Bin\deigde.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\Bin\deigde.exe"; Flags: nowait postinstall skipifsilent; Description: "{cm:LaunchProgram,Drag[en]gine IGDE}"

[Registry]
Root: "HKCR"; Subkey: ".degd"; ValueType: string; ValueData: "Drag[en]gine Game Definition"; Flags: uninsdeletevalue
Root: "HKCR"; Subkey: "Drag[en]gine Game Definition"; ValueType: string; ValueData: "Drag[en]gine Game Definition"; Flags: uninsdeletekey
Root: "HKCR"; Subkey: "Drag[en]gine Game Definition\DefaultIcon"; ValueType: string; ValueData: "{app}\Bin\deigde.exe,0"
Root: "HKCR"; Subkey: ".degp"; ValueType: string; ValueData: "Drag[en]gine Game Project"; Flags: uninsdeletevalue
Root: "HKCR"; Subkey: "Drag[en]gine Game Project"; ValueType: string; ValueData: "Drag[en]gine Game Project"; Flags: uninsdeletekey
Root: "HKCR"; Subkey: "Drag[en]gine Game Project\DefaultIcon"; ValueType: string; ValueData: "{app}\Bin\deigde.exe,0"
Root: "HKCR"; Subkey: "Drag[en]gine Game Project\shell\open\command"; ValueType: string; ValueData: """{app}\Bin\deigde.exe"" ""%1"""; Flags: uninsdeletekey
Root: "HKLM"; Subkey: "SOFTWARE\Drag[en]gine"; ValueType: string; ValueName: "PathIgde"; ValueData: "{app}"
