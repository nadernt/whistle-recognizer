#include "stdafx.h"
#include "apptools.h"
#include "INI.h"
#include <shlwapi.h>
#include <sys\types.h> 
#include <sys\stat.h> 


AppTools::AppTools(void)
{
}

AppTools::~AppTools(void)
{
}
BOOL AppTools::IsFileExist(CString FilePath)
{
CFileFind finder;
BOOL bResult= finder.FindFile(FilePath);
return bResult; 
}


CString AppTools::GetAppPath()
{
char dir[256] = "";
GetModuleFileName(NULL, dir, 256);
for (int i = strlen(dir); dir[i] != '\\' && i > 0; i--) 
	dir[i] = '\0';
return (CString)dir;
}

CString AppTools::ChooseFolder(HWND m_hWnd,BOOL NewFolderYes)
{
    BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.hwndOwner = m_hWnd;
	if(NewFolderYes)
	bi.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	else
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	BOOL bRet = FALSE;

	TCHAR szFolder[MAX_PATH*2];
	szFolder[0] = _T('\0');

	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
		{
			bRet = TRUE;
		}

		IMalloc *pMalloc = NULL; 
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc) 
		{  
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
		}
	}
	return szFolder;
}

	
BOOL AppTools::IsDirExist(CString DirPath)
{

	CFileFind ff;
	CString path = DirPath;
	
	if(path.Right(1) != "\\")
		path += "\\";


	if (PathIsDirectory(path))
			return TRUE;
		else 
			return FALSE;
}

void AppTools::EmptyDir(CString Dir_Path)
{
		CFileFind ff;
		CString path = Dir_Path;
	
		if(path.Right(1) != "\\")
		path += "\\";

		path += "*.*";

		BOOL res = ff.FindFile(path);

		while(res)
		{	
			res = ff.FindNextFile();
			if (!ff.IsDots() && !ff.IsDirectory())
				DeleteFile(ff.GetFilePath());
		}
		
}

BOOL AppTools::RemoveDir(CString Dir_Path)
{
	if(!IsDirExist(Dir_Path))
		return 0;
	EmptyDir(Dir_Path);
	RemoveDirectory(Dir_Path);
	return TRUE;
}

int AppTools::GetSettingInt(CString FileName,CString KeySection,CString Section)
{
	CIniReader m_IniReader; 
	m_IniReader.setINIFileName(FileName);
	CString m_strKeyValue;
	m_strKeyValue = m_IniReader.getKeyValue(KeySection,Section);
	return atoi(m_strKeyValue);
}

CString AppTools::GetSettingString(CString FileName,CString KeySection,CString Section)
{
	CIniReader m_IniReader; 
	m_IniReader.setINIFileName(FileName);
	CString m_strKeyValue;
	m_strKeyValue = m_IniReader.getKeyValue(KeySection,Section);
	return m_strKeyValue;
}

void AppTools::SetSetting(CString FileName,CString KeyValue,CString KeySection,CString Section)
{
	CIniReader m_IniReader; 
	m_IniReader.setINIFileName(FileName);
	m_IniReader.setKey(KeyValue,KeySection,Section);
}

__int64  AppTools::GetFileSizeInByte(CString FileName)
{
		if(IsFileExist(FileName))
		{
			struct __stat64 fileStat; 
			int err = _stat64( FileName, &fileStat ); 
			
			if (0 != err) 
				return -1;
			else
				return fileStat.st_size;

		}
		else
			return -2; //file if not exist
}