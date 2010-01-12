VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim WithEvents Client As CSocketMaster
Attribute Client.VB_VarHelpID = -1
Dim Cini As New Cini

Private Sub Form_Load()
    Dim IPstr As String
    Dim PORTstr As String
    Cini.Path = App.Path & "\UDPINI\IPSET.ini"
    IPstr = GetiniValue("IP", "IPVAL")
    PORTstr = GetiniValue("PORT", "IPPORT")
    'MsgBox IPstr & PORTstr
    Set Client = New CSocketMaster
    Client.Protocol = sckUDPProtocol
    Client.RemotePort = CLng(PORTstr)
    Client.RemoteHost = IPstr
    Client.Send2PG CDbl("0"), CDbl("0"), CDbl("0"), 32000, 1
    Client.Send2PG CDbl("0"), CDbl("0"), CDbl("0"), 32000, 1
    Client.Send2PG CDbl("0"), CDbl("0"), CDbl("0"), 32000, 1
    End
End Sub

Private Sub Client_Error(ByVal Number As Integer, Description As String, ByVal sCode As Long, ByVal Source As String, ByVal HelpFile As String, ByVal HelpContext As Long, CancelDisplay As Boolean)
MsgBox Description, vbCritical, "Client Error " & Number
Client.CloseSck
End
End Sub

Private Function GetiniValue(iSection As String, iKey As String) As String
Dim StrTmp As String
With Cini
    .Section = iSection
    .Key = iKey
     StrTmp = .Value
 End With
    GetiniValue = StrTmp
End Function

Private Sub Form_Unload(Cancel As Integer)
Client.CloseSck
End Sub
