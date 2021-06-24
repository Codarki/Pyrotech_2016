
Imports System
Imports System.Threading

'Namespace VBGui
Public Class ThreadWrapper

    Declare Sub run Lib "within_epsilon.dll" (ByVal width As Integer, ByVal height As Integer, ByVal windowed As Boolean)

    Public Shared res_x As Integer
    Public Shared res_y As Integer
    Public Shared windowed As Boolean

    Shared Sub run()
        run(res_x, res_y, windowed)
    End Sub

End Class
'End Namespace