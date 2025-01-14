[Setup]
AppName=RuNda
AppVersion=0.996
AppVerName=RuNda 0.996
DefaultDirName={pf}\RuNda
AllowNoIcons=Yes
DefaultGroupName=RuNda
LicenseFile=BSD
Compression=bzip
OutputBaseFileName=runda-0.996
OutputDir=.
AppPublisher=Taku Kudo
AppPublisherURL=http://runda.sourceforge.net/
ShowLanguageDialog=yes

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "jp"; MessagesFile: "compiler:Japanese.isl"

[Files]
Source: "AUTHORS";               DestDir: "{app}"
Source: "COPYING";               DestDir: "{app}"
Source: "GPL";                   DestDir: "{app}"
Source: "LGPL";                  DestDir: "{app}"
Source: "BSD";                   DestDir: "{app}"
Source: "src\runda.exe";         DestDir: "{app}\bin"

Source: "src\runda-*-*.exe";     DestDir: "{app}\bin"
Source: "src\librunda.dll";      DestDir: "{app}\bin"
Source: "wintmp\rundarc";    DestDir: "{app}\etc"
Source: "src\librunda.lib";  DestDir: "{app}\sdk"
Source: "src\librunda.dll";      DestDir: "{app}\bin"
Source: "wintmp\*.c";   DestDir: "{app}\sdk"
Source: "wintmp\*.cpp"; DestDir: "{app}\sdk"
Source: "src\runda.h";  DestDir: "{app}\sdk"
Source: "doc\*.html";   DestDir: "{app}\doc"
Source: "doc\*.png";   DestDir: "{app}\doc"
Source: "doc\*.css";    DestDir: "{app}\doc"
Source: "wintmp\dic\*.csv";  DestDir: "{app}\dic\ipadic"
Source: "wintmp\dic\*.def";  DestDir: "{app}\dic\ipadic"
Source: "wintmp\dic\dicrc";  DestDir: "{app}\dic\ipadic"

[Icons]
Name: "{commonprograms}\RuNda\RuNda";           Filename: "{app}\bin\runda.exe"
Name: "{commonprograms}\RuNda\Recompile SHIFT-JIS Dictionary"; WorkingDir: "{app}\dic\ipadic"; Filename: "{app}\bin\runda-dict-index.exe"; Parameters: "-f SHIFT-JIS -t SHIFT-JIS"; Comment: "Recompile SHIFT-JIS dictionary"
Name: "{commonprograms}\RuNda\Recompile UTF-8 Dictionary"; WorkingDir: "{app}\dic\ipadic";  Filename: "{app}\bin\runda-dict-index.exe"; Parameters: "-f SHIFT-JIS -t UTF-8"; Comment: "Recompile UTF-8 dictionary"
Name: "{commonprograms}\RuNda\Recompile UTF-16 Dictionary"; WorkingDir: "{app}\dic\ipadic";  Filename: "{app}\bin\runda-dict-index.exe"; Parameters: "-f SHIFT-JIS -t UTF-16"; Comment: "Recompile UTF-16 dictionary"
Name: "{commonprograms}\RuNda\Recompile EUC-JP Dictionary"; WorkingDir: "{app}\dic\ipadic";  Filename: "{app}\bin\runda-dict-index.exe"; Parameters: "-f SHIFT-JIS -t EUC-JP"; Comment: "Recompile EUC-JP dictionary"
Name: "{commonprograms}\RuNda\Uninstall RuNda"; Filename: "{app}\unins000.exe"
Name: "{commonprograms}\RuNda\RuNda Document";  Filename: "{app}\doc\index.html"
Name: "{userdesktop}\RuNda"; Filename:  "{app}\bin\runda.exe"

[Run]
Filename: "{app}\bin\runda-dict-index.exe"; Parameters: "-f SHIFT-JIS -t {code:GetCharCode}"; WorkingDir: "{app}\dic\ipadic"

[UninstallDelete]
Type: files; Name: "{app}\dic\ipadic\*.bin"
Type: files; Name: "{app}\dic\ipadic\*.dic"

[Registry]
Root: HKLM; Subkey: "software\RuNda"; Flags: uninsdeletekey; ValueType: string; ValueName: "rundarc"; ValueData: "{app}\etc\rundarc" ; Check: IsAdmin
Root: HKCU; Subkey: "software\RuNda"; Flags: uninsdeletekey; ValueType: string; ValueName: "rundarc"; ValueData: "{app}\etc\rundarc"

[Code]
Program Setup;
var
  IsAdminFlg:         boolean;
  IsAdminCheckedFlg:  boolean;
  UserPage: TInputOptionWizardPage;

Function IsAdmin (): Boolean;
var
  conf: String;
begin
  if not IsAdminLoggedOn () then
  begin
    Result := false;
  end
  else
  begin
    conf := 'You have administrator privileges. Do you permit all users to run RuNda';
    if ActiveLanguage = 'jp' then
    begin
      conf := '�Ǘ��Ҍ������������̂悤�ł��B���̃R���s���[�^�̑S���[�U��RuNda�̎��s�������܂���?';
    end
    if not IsAdminCheckedFlg then
       IsAdminFlg := MsgBox (conf, mbInformation, mb_YesNo) = idYes;
    IsAdminCheckedFlg := true;
    Result := IsAdminFlg;
  end;
end;

Function GetCharCode (Param: String): String;
var
  msg: String;
begin
  msg := 'Start compiling RuNda dictionary. It will take 30-60secs.';
  if ActiveLanguage = 'jp' then
  begin
    msg := 'RuNda�̎������쐬���܂��B��Ƃɂ�1���قǂ�����ꍇ���������܂��B';
  end;
  MsgBox(msg, mbInformation, mb_OK);
  Result := 'SHIFT-JIS';
  if UserPage.Values[0] = True then
  begin
     Result := 'SHIFT-JIS';
  end;
  if UserPage.Values[1] = True then
  begin
     Result := 'UTF-8';
  end;
  if UserPage.Values[2] = True then
  begin
     Result := 'UTF-16';
  end;
  if UserPage.Values[3] = True then
  begin
     Result := 'EUC-JP';
  end;
end;

procedure InitializeWizard;
var
  msg : array[0..3] of String;
begin
  msg[0] := 'Dictionary Charset'
  msg[1] := 'Please choose character set';
  msg[2] := 'Please specify charset set of dictionary, then click Next.';
  if ActiveLanguage = 'jp' then
  begin
    msg[0] := '�����̕����R�[�h�̑I��'
    msg[1] := '�����̕����R�[�h��I�����Ă��������B';
    msg[2] := '�C���X�g�[�����鎫���̕����R�[�h��I����(�ʏ��SHIFT-JIS)�A���ւ��N���b�N���Ă��������B';
  end;
  UserPage := CreateInputOptionPage(wpWelcome, msg[0], msg[1], msg[2], True, True);
  UserPage.Add('SHIFT-JIS');
  UserPage.Add('UTF-8');
  UserPage.Add('UTF-16');
  UserPage.Add('EUC-JP');
  UserPage.Values[0] := True;
end;
