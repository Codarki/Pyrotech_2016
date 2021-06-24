Imports Microsoft.DirectX
Imports Microsoft.DirectX.Direct3D
Imports Microsoft.DirectX.Diagnostics
'Imports System.Runtime.InteropServices
'Imports System.Threading


Namespace VBGui


    Public Class Form1
        Inherits System.Windows.Forms.Form

        Declare Function run Lib "demo.dll" (ByVal width As Integer, ByVal height As Integer, ByVal windowed As Boolean) As Integer


        'Private trd As Thread

        Private Function CheckPixelShaderVersion(ByVal adapterNum As Integer) As Boolean
            Dim shaderVer As System.Version = Manager.GetDeviceCaps(adapterNum, DeviceType.Hardware).PixelShaderVersion()
            Dim shaderVerStr As String = shaderVer.Major.ToString
            Dim shaderVerNum As Integer = Integer.Parse(shaderVerStr)
            If (shaderVerNum < 3) Then
                MsgBox("Adapter must support pixel shader 3 or above!", MsgBoxStyle.Critical)
                Return False
            End If
            Return True
        End Function

        Private Function FormatBit(ByVal f As String) As Integer

            Dim cArr() As Char = f.ToCharArray()
            Dim sum As Integer = 0
            'Console.WriteLine(cArr.Length)
            For i As Integer = 0 To cArr.Length - 1 Step 1
                'Console.WriteLine(cArr(i))
                If Char.IsNumber(cArr(i)) Then
                    sum = sum + Integer.Parse(cArr(i))
                End If
            Next

            Return sum
        End Function

        Private Sub PopulateAdapters()
            Dim adaptString As String
            For Each adapter As AdapterInformation In Manager.Adapters
                adaptString = adapter.Information.Description
                'Console.WriteLine("#{0}", adapter.Information.Description)
                Adapters.Items().Add(adaptString)
                'Console.WriteLine(Manager.GetDeviceCaps(0, DeviceType.Hardware).PixelShaderVersion())
            Next
        End Sub

        Private Sub PopulateDisplayModes()
            '           Dim i As Integer
            Dim j As Integer
            '            Dim adaptString As String

            Dim width As String
            Dim height As String
            Dim bpp As String
            Dim hz As String

            Dim selectedIndex As Integer = -1

            For Each dispMode As DisplayMode In Manager.Adapters(Adapters.SelectedIndex).SupportedDisplayModes
                width = dispMode.Width.ToString
                height = dispMode.Height.ToString
                bpp = FormatBit(dispMode.Format.ToString).ToString
                hz = dispMode.RefreshRate.ToString
                'Console.WriteLine(" {0}", dispMode.Width.ToString + "x" + dispMode.Height.ToString + "(" + dispMode.RefreshRate.ToString + ") " + dispMode.Format.ToString)

                'DisplayMode.Items().Add(dispMode.Width.ToString + "x" + dispMode.Height.ToString + "(" + dispMode.RefreshRate.ToString + ") " + dispMode.Format.ToString)

                If bpp = "32" Then
                    DisplayMode.Items().Add(width + "x" + height + "x" + bpp + "bpp" + "@" + hz)
                    'Console.WriteLine(dispMode.ToString)
                    If width = 1280 And height = 720 Then
                        selectedIndex = j
                    End If
                    j = j + 1
                End If

            Next

            If selectedIndex <> -1 Then
                DisplayMode.SelectedIndex = selectedIndex
            End If

        End Sub


#Region " Windows Form Designer generated code "

        Public Sub New()
            MyBase.New()

            'This call is required by the Windows Form Designer.
            InitializeComponent()

            'Add any initialization after the InitializeComponent() call
        End Sub

        'Form overrides dispose to clean up the component list.
        Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
            If disposing Then
                If Not (components Is Nothing) Then
                    components.Dispose()
                End If
            End If
            MyBase.Dispose(disposing)
        End Sub

        'Required by the Windows Form Designer
        Private components As System.ComponentModel.IContainer

        'NOTE: The following procedure is required by the Windows Form Designer
        'It can be modified using the Windows Form Designer.  
        'Do not modify it using the code editor.
        Friend WithEvents Button1 As System.Windows.Forms.Button
        Friend WithEvents Button2 As System.Windows.Forms.Button
        Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
        Friend WithEvents DisplayMode As System.Windows.Forms.ListBox
        Friend WithEvents Fullscreen As System.Windows.Forms.CheckBox
        Friend WithEvents Adapters As System.Windows.Forms.ComboBox
        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
            Me.Button1 = New System.Windows.Forms.Button()
            Me.Button2 = New System.Windows.Forms.Button()
            Me.PictureBox1 = New System.Windows.Forms.PictureBox()
            Me.DisplayMode = New System.Windows.Forms.ListBox()
            Me.Fullscreen = New System.Windows.Forms.CheckBox()
            Me.Adapters = New System.Windows.Forms.ComboBox()
            CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
            Me.SuspendLayout()
            '
            'Button1
            '
            Me.Button1.Location = New System.Drawing.Point(10, 270)
            Me.Button1.Name = "Button1"
            Me.Button1.Size = New System.Drawing.Size(96, 28)
            Me.Button1.TabIndex = 0
            Me.Button1.Text = "Start"
            '
            'Button2
            '
            Me.Button2.Location = New System.Drawing.Point(230, 270)
            Me.Button2.Name = "Button2"
            Me.Button2.Size = New System.Drawing.Size(96, 28)
            Me.Button2.TabIndex = 1
            Me.Button2.Text = "Exit"
            '
            'PictureBox1
            '
            Me.PictureBox1.Image = CType(resources.GetObject("PictureBox1.Image"), System.Drawing.Image)
            Me.PictureBox1.Location = New System.Drawing.Point(10, 9)
            Me.PictureBox1.Name = "PictureBox1"
            Me.PictureBox1.Size = New System.Drawing.Size(318, 116)
            Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
            Me.PictureBox1.TabIndex = 2
            Me.PictureBox1.TabStop = False
            '
            'DisplayMode
            '
            Me.DisplayMode.ItemHeight = 16
            Me.DisplayMode.Location = New System.Drawing.Point(10, 166)
            Me.DisplayMode.Name = "DisplayMode"
            Me.DisplayMode.Size = New System.Drawing.Size(318, 68)
            Me.DisplayMode.TabIndex = 3
            '
            'Fullscreen
            '
            Me.Fullscreen.Checked = True
            Me.Fullscreen.CheckState = System.Windows.Forms.CheckState.Checked
            Me.Fullscreen.Location = New System.Drawing.Point(10, 249)
            Me.Fullscreen.Name = "Fullscreen"
            Me.Fullscreen.Size = New System.Drawing.Size(96, 19)
            Me.Fullscreen.TabIndex = 4
            Me.Fullscreen.Text = "Fullscreen"
            '
            'Adapters
            '
            Me.Adapters.ImeMode = System.Windows.Forms.ImeMode.NoControl
            Me.Adapters.Location = New System.Drawing.Point(10, 135)
            Me.Adapters.Name = "Adapters"
            Me.Adapters.Size = New System.Drawing.Size(318, 24)
            Me.Adapters.TabIndex = 6
            '
            'Form1
            '
            Me.AutoScaleBaseSize = New System.Drawing.Size(6, 15)
            Me.ClientSize = New System.Drawing.Size(338, 311)
            Me.Controls.Add(Me.Adapters)
            Me.Controls.Add(Me.Fullscreen)
            Me.Controls.Add(Me.DisplayMode)
            Me.Controls.Add(Me.PictureBox1)
            Me.Controls.Add(Me.Button2)
            Me.Controls.Add(Me.Button1)
            Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
            Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
            Me.MaximizeBox = False
            Me.MinimizeBox = False
            Me.Name = "Form1"
            Me.Text = "Pyrotech"
            CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
            Me.ResumeLayout(False)

        End Sub

#End Region

        Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

            PopulateAdapters()
            Adapters.SelectedIndex = 0
            'PopulateDisplayModes()
        End Sub

        Private Sub PictureBox1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PictureBox1.Click

        End Sub

        Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
            Close()
        End Sub

        Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click

            If (DisplayMode.SelectedIndex < 0) Then
                MsgBox("Displaymode not selected!", MsgBoxStyle.ApplicationModal)
                Return
            End If

            If (Not CheckPixelShaderVersion(Adapters.SelectedIndex)) Then
                Close()
                Return
            End If
            'Console.WriteLine(DisplayMode.SelectedItem)
            Dim windowed As Boolean = Not Fullscreen.Checked
            Dim Resolution() As String = Split(DisplayMode.SelectedItem, "x")

            Dim width As Integer = Integer.Parse(Resolution(0))
            Dim height As Integer = Integer.Parse(Resolution(1))
            'Console.WriteLine(Resolution(0) + " " + Resolution(1))

            Try
                ' Hide()
                Call run(width, height, windowed)
            Catch ex As Exception
            Finally
                Close()

            End Try

            'ThreadWrapper.res_x = width
            'ThreadWrapper.res_y = height
            'ThreadWrapper.windowed = windowed

            'trd = New Thread(AddressOf ThreadWrapper.run)
            'trd.AllocateNamedDataSlot("res_x")
            'trd.AllocateNamedDataSlot("res_y")
            'trd.AllocateNamedDataSlot("windowed")
            'trd.SetData(trd.GetNamedDataSlot("res_x"), width)
            'trd.SetData(trd.GetNamedDataSlot("res_y"), height)
            'trd.SetData(trd.GetNamedDataSlot("windowed"), windowed)
            'trd.IsBackground = False
            'trd.Start()
            'Hide()
            'While (trd.ThreadState().Running)
            'do nothing
            'End While
            'Close()
        End Sub

        Private Sub CheckBox1_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Fullscreen.CheckedChanged

        End Sub

        Private Sub CheckBox1_CheckedChanged_1(ByVal sender As System.Object, ByVal e As System.EventArgs)

        End Sub

        Private Sub DisplayMode_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DisplayMode.SelectedIndexChanged

        End Sub

        Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Adapters.SelectedIndexChanged
            PopulateDisplayModes()
        End Sub

        Private Sub Adapters_KeyPress(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyPressEventArgs) Handles Adapters.KeyPress
            e.Handled = True
        End Sub

    End Class


End Namespace