using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using NAudio;
using NAudio.Wave;


namespace MultiRoom
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private DirectSoundOut output;
        private Mp3FileReader wave1, wave2;


        private void button1_Click(object sender, EventArgs e)
        {
 


            output = new DirectSoundOut();

            wave1 = new Mp3FileReader(@"C:\Users\Blare\Music\Shrek (2001)\10. Rufus Wainwright - Hallelujah.mp3");
            wave2 = new Mp3FileReader(@"C:\Users\Blare\Music\Shrek (2001)\01. Self - Stay Home.mp3");
            var waveProvider = new MultiplexingWaveProvider(new IWaveProvider[]{ wave1, wave2 }, 2);

            waveProvider.ConnectInputToOutput(1, 0);
            waveProvider.ConnectInputToOutput(2, 1);
           AsioOut dfd = new AsioOut();
            string ffd = dfd.DriverName;
//dfd.Init(waveProvider);
       //     dfd.Play();

            output.Init(wave1);
            output.Play();
        }
    }
}
