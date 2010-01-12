#pragma once
class AppTools
{
public:
	AppTools(void);
	~AppTools(void);
	CString GetAppPath();
	CString ChooseFolder(HWND m_hWnd,BOOL NewFolderYes);
	BOOL IsDirExist(CString DirPath);
	BOOL IsFileExist(CString FilePath);
	void EmptyDir(CString Dir_Path);
	void SetSetting(CString FileName,CString KeyValue,CString KeySection,CString Section);
	CString GetSettingString(CString FileName,CString KeySection,CString Section);
	int GetSettingInt(CString FileName,CString KeySection,CString Section);
	BOOL RemoveDir(CString Dir_Path);
	__int64  AppTools::GetFileSizeInByte(CString FileName);
};
