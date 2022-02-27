[Setup]
AppId={{15FD3C65-A7CA-4CE2-A500-6EF483EC5C4B}
AppName=Drag[en]gine IGDE SDK
AppVersion=1.10
AppPublisher=Dragon Dreams
AppPublisherURL=https://dragondreams.ch
AppSupportURL=https://dragondreams.ch
AppUpdatesURL=https://dragondreams.ch
DefaultDirName={pf}\DEIGDE SDK
DisableDirPage=no
DefaultGroupName=Drag[en]gine IGDE SDK
DisableProgramGroupPage=no
OutputDir=build
OutputBaseFilename=install-deigde-sdk-windows64
SetupIconFile=deigde-installer.ico
Compression=lzma
SolidCompression=yes
ChangesAssociations=yes
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
PrivilegesRequired=admin

[Files]
Source: "unpacked\@ProgramFiles\DEIGDE\SDK\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
