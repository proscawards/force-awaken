; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Force Awaken"
#define MyAppVersion "1.0.1"
#define MyAppPublisher "proscawards"
#define MyAppURL "https://github.com/proscawards/force-awaken"
#define MyAppExeName "force-awaken.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{FB9C8273-AB10-480E-BD52-B5D93CC80F80}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=D:\GitHub\force-awaken\setup
OutputBaseFilename=setup
SetupIconFile=D:\GitHub\force-awaken\resources\assets\logo.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\force-awaken.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\Qt6Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\Qt6Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\Qt6Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\iconengines\*"; DestDir: "{app}\plugins/iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\imageformats\*"; DestDir: "{app}\plugins/imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\platforms\*"; DestDir: "{app}\plugins/platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\styles\*"; DestDir: "{app}\plugins/styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\GitHub\force-awaken\build-force-awaken-Desktop_Qt_6_2_3_MinGW_64_bit-Release\release\translations\*"; DestDir: "{app}\plugins/translations"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
