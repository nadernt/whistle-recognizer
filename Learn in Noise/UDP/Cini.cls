VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Cini"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
Private m_sPath As String
Private m_sKey As String
Private m_sSection As String
Private m_sDefault As String
Private m_lLastReturnCode As Long
' Declares for cIniFile:

#If Win32 Then
    ' Profile String functions:

Private Declare Function WritePrivateProfileString Lib "KERNEL32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As String, ByVal lpFileName As String) As Long

Private Declare Function GetPrivateProfileString Lib "KERNEL32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As Any, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
#Else
    ' Profile String functions:
    ' If you are developing in VB5, delete t
    '     his section
    ' otherwise SetupKit gets **confused**!

Private Declare Function WritePrivateProfileString Lib "Kernel" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) As Integer

Private Declare Function GetPrivateProfileString Lib "Kernel" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As Any, ByVal lpReturnedString As String, ByVal nSize As Integer, ByVal lpFileName As String) As Integer
#End If


Property Get LastReturnCode() As Long

    ' Did the last call succeed?
    ' 0 if not!
    LastReturnCode = m_lLastReturnCode
End Property


Property Let Default(sDefault As String)

    ' What to return if something goes wrong
    '     :
    m_sDefault = sDefault
End Property


Property Get Default() As String

    ' What to return if something goes wrong
    '     :
    Default = m_sDefault
End Property


Property Let Path(sPath As String)

    ' The filename of the INI file:
    m_sPath = sPath
End Property


Property Get Path() As String

    ' The filename of the INI file:
    Path = m_sPath
End Property


Property Let Key(sKey As String)

    ' The KEY= bit to look for
    m_sKey = sKey
End Property


Property Get Key() As String

    ' The KEY= bit to look for
    Key = m_sKey
End Property


Property Let Section(sSection As String)

    ' The [SECTION] bit to look for
    m_sSection = sSection
End Property


Property Get Section() As String

    ' The [SECTION] bit to look for
    Section = m_sSection
End Property


Property Get Value() As String

    ' Get the value of the current Key withi
    '     n Section of Path
    Dim sBuf As String
    Dim iSize As String
    Dim iRetCode As Integer
    sBuf = Space$(255)
    iSize = Len(sBuf)
    iRetCode = GetPrivateProfileString(m_sSection, m_sKey, m_sDefault, sBuf, iSize, m_sPath)


    If (iSize > 0) Then
        Value = Left$(sBuf, iRetCode)
    Else
        Value = ""
    End If

End Property


Property Let Value(sValue As String)

    ' Set the value of the current Key withi
    '     n Section of Path
    Dim iPos As Integer
    ' Strip chr$(0):
    iPos = InStr(sValue, Chr$(0))


    Do While iPos <> 0
        sValue = Left$(sValue, (iPos - 1)) & Mid$(sValue, (iPos + 1))
        iPos = InStr(sValue, Chr$(0))
    Loop

    m_lLastReturnCode = WritePrivateProfileString(m_sSection, m_sKey, sValue, m_sPath)
End Property


Public Sub DeleteValue()

    ' Delete the value at Key within Section
    '     of Path
    m_lLastReturnCode = WritePrivateProfileString(m_sSection, m_sKey, 0&, m_sPath)
End Sub


Public Sub DeleteSection()

    ' Delete the Section in Path
    m_lLastReturnCode = WritePrivateProfileString(m_sSection, 0&, 0&, m_sPath)
End Sub


Property Get INISection() As String

    ' Return all the keys and values within
    '     the current
    ' section, separated by chr$(0):
    Dim sBuf As String
    Dim iSize As String
    Dim iRetCode As Integer
    sBuf = Space$(255)
    iSize = Len(sBuf)
    iRetCode = GetPrivateProfileString(m_sSection, 0&, m_sDefault, sBuf, iSize, m_sPath)


    If (iSize > 0) Then
        INISection = Left$(sBuf, iRetCode)
    Else
        INISection = ""
    End If

End Property


Property Let INISection(sSection As String)

    ' Set one or more the keys within the cu
    '     rrent section.
    ' Keys and Values should be separated by
    '     chr$(0):
    m_lLastReturnCode = WritePrivateProfileString(m_sSection, 0&, sSection, m_sPath)
End Property




'Private Sub Command3_Click()
' With Cini
'    .Section = "WaveName" ' Look under the section headed [boot]
'    .Key = "Name" ' Search For shell=
'    MsgBox .Value
' End With
'End Sub

'Private Sub Command4_Click()
'With Cini
'    .Section = "WaveName" ' Look under the section headed [boot]
'    .Key = "Name2" ' Search For shell=
'    .Value = "Love"
'End With
'End Sub
'
'Private Sub Command5_Click()
'With Cini
'    .Section = "WaveName" ' Look under the section headed [boot]
'    .DeleteSection
'End With
'End Sub
'
'Private Sub Command6_Click()
'With Cini
'    .Section = "WaveName1" ' Look under the section headed [boot]
'    '.Key = "Name1" ' Search For shell=
'    MsgBox .INISection
'End With
'End Sub
'
'Private Sub Command7_Click()
'With Cini
'    .Section = "WaveName1" ' Look under the section headed [boot]
'    .Key = "Name2" ' Look under the section headed [boot]
'    .DeleteValue
'End With
'End Sub
'
'Private Sub Command8_Click()
'With Cini
'    .Section = "WaveName" ' Look under the section headed [boot]
    '.Key = "Name2" ' Search For shell=
'    MsgBox .INISection
'End With
'End Sub
