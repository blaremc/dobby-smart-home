using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;
using NAudio;
using NAudio.WindowsMediaFormat;
using NAudio.Wave;
using CUETools.Codecs;
using CUETools.Codecs.FLAKE;
using System.Data;
using System.IO;
using System.Linq;


namespace SmartHome
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public WaveIn waveSource = null;
        public WaveFileWriter waveFile = null;

        public MainWindow()
        {
            
            InitializeComponent();
        }
        
        private void SendFileRun(String filePath)
        {
        
          // Create delegate and make async call
          DoWorkDelegate worker = new DoWorkDelegate(SendFile);
          worker.BeginInvoke(filePath, new AsyncCallback(DoWorkComplete), worker);
        }
        private delegate void DoWorkDelegate(String filePath);



        private void DoWorkComplete(IAsyncResult workID)
        {

            DoWorkDelegate worker = workID.AsyncState as DoWorkDelegate;
            worker.EndInvoke(workID);
            //ReportOnProgress(100, "Запрос произведен");
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {

            waveSource = new WaveIn();
            waveSource.WaveFormat = new WaveFormat(44100, 1);

            waveSource.DataAvailable += new EventHandler<WaveInEventArgs>(waveSource_DataAvailable);
            waveSource.RecordingStopped += new EventHandler<StoppedEventArgs>(waveSource_RecordingStopped);
            
            waveFile = new WaveFileWriter(@"C:\Temp\Test0001.wav", waveSource.WaveFormat);

            waveSource.StartRecording();
        }


        void waveSource_DataAvailable(object sender, WaveInEventArgs e)
        {
            if (waveFile != null)
            {
                waveFile.Write(e.Buffer, 0, e.BytesRecorded);
                waveFile.Flush();
            }
        }

        void waveSource_RecordingStopped(object sender, StoppedEventArgs e)
        {
            if (waveSource != null)
            {
                waveSource.Dispose();
                waveSource = null;
            }

            if (waveFile != null)
            {
                waveFile.Dispose();
                waveFile = null;
            }

  
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
        //    waveSource.StopRecording();

            WebClient myWebClient = new WebClient();
            byte[] responseArray = myWebClient.UploadFile("https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=ru-RU", "POST", @"C:\Temp\Test0001.wav");

            // Decode and display the response.
            Console.WriteLine("\nResponse Received.The contents of the file uploaded are:\n{0}",
                System.Text.Encoding.ASCII.GetString(responseArray));
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {

            var input1 = new WaveFileReader(@"C:\Temp\Test0001.wav");
            var input2 = new WaveFileReader(@"C:\Temp\Test0001.wav");
            var waveProvider = new MultiplexingWaveProvider(new IWaveProvider[] { input1, input2 }, 4);

            waveProvider.ConnectInputToOutput(2,1);
            waveProvider.ConnectInputToOutput(3,1);
            waveProvider.ConnectInputToOutput(1,2);
            waveProvider.ConnectInputToOutput(1,3);
         

            AsioOut dfd = new AsioOut();
            string ffd = dfd.DriverName;
            dfd.Init(waveProvider);
            dfd.Play();
        }


        // <summary> Конвертирование wav-файла во flac </summary>        
        /// <returns>Частота дискретизации</returns>
        public static int Wav2Flac(String wavName, string flacName)
        {
            int sampleRate = 0;

            IAudioSource audioSource = new WAVReader(wavName, null);
            AudioBuffer buff = new AudioBuffer(audioSource, 0x10000);

            FlakeWriter flakewriter = new FlakeWriter(flacName, audioSource.PCM);
            sampleRate = audioSource.PCM.SampleRate;

            FlakeWriter audioDest = flakewriter;
            while (audioSource.Read(buff, -1) != 0)
            {
                audioDest.Write(buff);
            }
            audioDest.Close();

            audioDest.Close();

            return sampleRate;
        }

        private void SendFile(String filePath)
        {
            try
            {
                ReportOnProgress(10, "Идет запрос");
                String responseFromServer = GoogleVoice.GoogleSpeechRequest(filePath, @"tmp.flac");

                /*
                AddLog(responseFromServer + System.Environment.NewLine);
                JSon.RecognitionResult result = JSon.Parse(responseFromServer);
                if (result.hypotheses.Length > 0)
                {
                    JSon.RecognizedItem item = result.hypotheses.First();
                    //listBox1.Items.Add(String.Format("{0:0.000} ; {1}", item.confidence, item.utterance));
                    AddToList(String.Format("{0:0.000} ; {1}", item.confidence, item.utterance));
                }
                else
                {
                    AddToList(String.Format("None"));
                    //listBox1.Items.Add(String.Format("None"));
                }
                */
                ReportOnProgress(100, "Запрос успешно выполнен");
            }
            catch (Exception e)
            {
                ReportOnProgress(100, "Ошибка запроса: " + e.Message);
                AddLog(e.ToString());
            }

        }

        private delegate void ReportOnProgressDelegate(int progress, string msg);

        private void AddToList(String item)
        {
            if (InvokeRequired)
            {
                Invoke(new AddDelegate(AddToList), new object[] { item });
                return;
            }
            
            ListBox1.Items.Add(item);
        }


        private void ReportOnProgress(int progress, string msg)
        {
            if (InvokeRequired)
            {
                Invoke(new ReportOnProgressDelegate(ReportOnProgress),
                  new object[] { progress, msg });
                return;
                ;
            }

            this.Title =  " —  " + msg;
        }


        private delegate void AddDelegate(String log);
        private void AddLog(string log)
        {
            if (InvokeRequired)
            {
                Invoke(new AddDelegate(AddLog), new object[] { log });
                return;
            }

            txtLog.Content += log;
        }

        private void ListBox_SelectionChanged_1(object sender, SelectionChangedEventArgs e)
        {

        }


    }
}
