using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Runtime.InteropServices;

namespace CSharpGui
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.Fullscreen.Checked = true;
            PopulateDisplayModes();
        }

        [DllImport("user32.dll")]
        public static extern bool EnumDisplaySettings(string deviceName, int modeNum, ref DEVMODE devMode);
        const int ENUM_CURRENT_SETTINGS = -1;
        const int ENUM_REGISTRY_SETTINGS = -2;

        [StructLayout(LayoutKind.Sequential)]
        public struct DEVMODE
        {
            private const int CCHDEVICENAME = 0x20;
            private const int CCHFORMNAME = 0x20;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 0x20)]
            public string dmDeviceName;
            public short dmSpecVersion;
            public short dmDriverVersion;
            public short dmSize;
            public short dmDriverExtra;
            public int dmFields;
            public int dmPositionX;
            public int dmPositionY;
            public ScreenOrientation dmDisplayOrientation;
            public int dmDisplayFixedOutput;
            public short dmColor;
            public short dmDuplex;
            public short dmYResolution;
            public short dmTTOption;
            public short dmCollate;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 0x20)]
            public string dmFormName;
            public short dmLogPixels;
            public int dmBitsPerPel;
            public int dmPelsWidth;
            public int dmPelsHeight;
            public int dmDisplayFlags;
            public int dmDisplayFrequency;
            public int dmICMMethod;
            public int dmICMIntent;
            public int dmMediaType;
            public int dmDitherType;
            public int dmReserved1;
            public int dmReserved2;
            public int dmPanningWidth;
            public int dmPanningHeight;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private int FormatBit(string text)
        {
            char[] cArr = text.ToCharArray();
            int sum = 0;
            for (int i = 0; i < cArr.Length; ++i)
            {
                if (Char.IsNumber(cArr[i]))
                {
                    sum = sum + Int32.Parse(cArr[i].ToString());
                }
            }
            return sum;
        }

        private void PopulateDisplayModes()
        {
            int indexToSelect = -1;
            DEVMODE vDevMode = new DEVMODE();
            int i = 0;
            while (EnumDisplaySettings(null, i, ref vDevMode))
            {
                Console.WriteLine("Width:{0} Height:{1} Color:{2} Frequency:{3}",
                                        vDevMode.dmPelsWidth,
                                        vDevMode.dmPelsHeight,
                                        1 << vDevMode.dmBitsPerPel, vDevMode.dmDisplayFrequency
                                    );
                i++;

                int bpp = vDevMode.dmBitsPerPel;
                if (bpp == 32)
                {
                    DisplayMode.Items.Add(vDevMode.dmPelsWidth + "x" + vDevMode.dmPelsHeight + "x" + bpp + "bpp" + "@" + vDevMode.dmDisplayFrequency);

                    if (vDevMode.dmPelsWidth == 1280 && vDevMode.dmPelsHeight == 720 && vDevMode.dmDisplayFrequency >= 50)
                    {
                        indexToSelect = DisplayMode.Items.Count - 1;
                    }
                }
            }

            if (indexToSelect != -1)
                this.DisplayMode.SelectedIndex = indexToSelect;

        }

        private void resolutionLabel_Click(object sender, EventArgs e)
        {
        }

        [DllImport("demo.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int run(int resX, int resY, int windowed);

        private void button1_Click(object sender, EventArgs e)
        {
            bool windowed = !this.Fullscreen.Checked;
            string displayModeText = (string)DisplayMode.SelectedItem;
            char[] separators = new char[1];
            separators[0] = 'x';
            string[] resolution = displayModeText.Split(separators);

            int width = Int32.Parse(resolution[0]);
            int height = Int32.Parse(resolution[1]);

            try
            {
                int result = run(width, height, windowed ? 1 : 0);
                //if (result != 0)
                //    throw new Exception("Virhe");
            }
            catch (Exception exception)
            {
 
            }

            Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
