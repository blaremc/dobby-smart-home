using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CUETools.Codecs;
using CUETools.Codecs.FLAKE;
using NAudio;
using NAudio.WindowsMediaFormat;
using NAudio.Wave;

namespace SmartHome
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        public WaveIn waveSource = null;
        public WaveFileWriter waveFile = null;
        public string wavFileName = @"C:\Temp\Test0001.wav";

        public string tmpName = @"tmp.flac";

        private delegate void DoWorkDelegate(String filePath);

        private void SendFileRun(String filePath)
        {
          
            // Create delegate and make async call
            DoWorkDelegate worker = new DoWorkDelegate(SendFile);
            worker.BeginInvoke(filePath, new AsyncCallback(DoWorkComplete), worker);
        }
        private void DoWorkComplete(IAsyncResult workID)
        {
    
            DoWorkDelegate worker = workID.AsyncState as DoWorkDelegate;
            worker.EndInvoke(workID);
            //ReportOnProgress(100, "Запрос произведен");
        }


        private delegate void ReportOnProgressDelegate(int progress, string msg);

        private void ReportOnProgress(int progress, string msg)
        {
            if (InvokeRequired)
            {
                Invoke(new ReportOnProgressDelegate(ReportOnProgress),
                  new object[] { progress, msg });
                return;
                ;
            }
            AddToList(msg);
            this.Text =  " —  " + msg;
        }

        private void SendFile(String filePath)
        {
            try
            {
                
                ReportOnProgress(10, "Идет запрос");
                String responseFromServer = GoogleVoice.GoogleSpeechRequest(filePath, tmpName);


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

                ReportOnProgress(100, "Запрос успешно выполнен");
            }
            catch (Exception e)
            {
                ReportOnProgress(100, "Ошибка запроса: " + e.Message);
                AddLog(e.ToString());
            }

        }

        private delegate void AddDelegate(String log);
        private void AddLog(string log)
        {
            if (InvokeRequired)
            {
                Invoke(new AddDelegate(AddLog), new object[] { log });
                return;
            }

            txtLog.Text += log;
        }

        private void AddToList(String item)
        {
            if (InvokeRequired)
            {
                Invoke(new AddDelegate(AddToList), new object[] { item });
                return;
            }
            listBox1.Items.Add(item);
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            waveSource.StopRecording();
            ReportOnProgress(100, "Остановили запись");
            SendFileRun(wavFileName);

        }

        private void button1_Click(object sender, EventArgs e)
        {
            waveSource = new WaveIn();
            waveSource.WaveFormat = new WaveFormat(44100, 1);

            waveSource.DataAvailable += new EventHandler<WaveInEventArgs>(waveSource_DataAvailable);
            waveSource.RecordingStopped += new EventHandler<StoppedEventArgs>(waveSource_RecordingStopped);

            waveFile = new WaveFileWriter(wavFileName, waveSource.WaveFormat);

            waveSource.StartRecording();
            ReportOnProgress(100, "Начали запись");
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

    }
}
