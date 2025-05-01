[Setup]
AppName=Voice Command App
AppVersion=1.0
DefaultDirName={pf}\VoiceCommandApp
OutputDir=.\Output
OutputBaseFilename=VoiceCommandInstaller
Compression=lzma
SolidCompression=yes
AppPublisher=Your Name
AppPublisherURL=https://yourwebsite.com
AppSupportURL=https://yourwebsite.com/support

[Files]
; Include the packaged executable and the requirements.txt file
Source: "C:\Users\Imogen\AppData\Roaming\Python\Python313\Scripts\dist\voice_gui.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Imogen\requirements.txt"; DestDir: "{app}"; Flags: ignoreversion

[Run]
; After installation, run pip to install the dependencies from the requirements.txt
Filename: "cmd.exe"; Parameters: "/C pip install -r ""{app}\requirements.txt"""; Flags: runhidden
