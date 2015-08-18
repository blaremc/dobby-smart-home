using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Un4seen.Bass;


namespace MultiRoom_Bass
{
    public partial class Form1 : Form
    {

        public static string file = @"C:\Development\Sites\dobby\misc\arduino\MultiRoom_Bass\MultiRoom_Bass\MultiRoom_Bass\bin\Debug\sound\test.wav";
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Bass.BASS_Init(-1, 44100, BASSInit.BASS_DEVICE_DEFAULT, IntPtr.Zero))
            {
                // create a stream channel from a file 
                int stream = Bass.BASS_StreamCreateFile(file, 0, 0, BASSFlag.BASS_DEFAULT);
                if (stream != 0)
                {
                    // play the stream channel 
                    Bass.BASS_ChannelPlay(stream, false);
                }
                else
                {
                    // error creating the stream 
                    Console.WriteLine("Stream error: {0}", Bass.BASS_ErrorGetCode());
                }


            
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Bass.BASS_Init(-1, 44100, BASSInit.BASS_DEVICE_DEFAULT, IntPtr.Zero);


            // create a first stream in this context 
            int stream1 = Bass.BASS_StreamCreateFile(file, 0L, 0L, BASSFlag.BASS_SPEAKER_FRONTLEFT);
            Bass.BASS_ChannelPlay(stream1, false);
     
            // create a second stream using this context 
            int stream2 = Bass.BASS_StreamCreateFile(file, 0L, 0L, BASSFlag.BASS_SPEAKER_FRONTRIGHT );
            Bass.BASS_ChannelPlay(stream2, false);

            // create a second stream using this context 
            int stream3 = Bass.BASS_StreamCreateFile(file, 0L, 0L, BASSFlag.BASS_SPEAKER_CENTER );
            Bass.BASS_ChannelPlay(stream3, false);
          
            // create a second stream using this context 
            int stream4 = Bass.BASS_StreamCreateFile(file, 0L, 0L, BASSFlag.BASS_SPEAKER_REARLEFT );
            Bass.BASS_ChannelPlay(stream4, false);

            // create a second stream using this context 
            int stream5 = Bass.BASS_StreamCreateFile(file, 0L, 0L, BASSFlag.BASS_SPEAKER_REARRIGHT );
            Bass.BASS_ChannelPlay(stream5, false);

            // create a second stream using this context 
            int stream6 = Bass.BASS_StreamCreateFile(file, 0L, 0L, BASSFlag.BASS_SPEAKER_LFE );
            Bass.BASS_ChannelPlay(stream6, false);
        }
    }
}
