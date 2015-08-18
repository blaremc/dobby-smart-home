using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Un4seen.Bass;
using System.Web;
using System.Configuration;

namespace Multiroom
{
    class Multiroom
    {

        public const int port = 6666;

        const byte FRONT = 0;
        const byte REAR = 1;
        const byte CENTERBASS = 2;
        const byte SIDE = 3;

        static public Dictionary<int, BASSFlag> channels;

        static public Dictionary<int, int> streams;

        private static Multiroom _instance;

        public Multiroom()
        {


            Multiroom.addLog("Init bass");
            if (!Bass.BASS_Init(-1, 44100, BASSInit.BASS_DEVICE_DEFAULT, IntPtr.Zero))
            {
                Multiroom.addLog("Bass init error " + Bass.BASS_ErrorGetCode());
            }else
            {
                Multiroom.addLog("Bass started");
            }

            channels = new Dictionary<int, BASSFlag>();
            channels.Add(-1, BASSFlag.BASS_DEFAULT | BASSFlag.BASS_SAMPLE_MONO);
            channels.Add(FRONT, BASSFlag.BASS_SPEAKER_FRONT);
            channels.Add(CENTERBASS, BASSFlag.BASS_SPEAKER_CENLFE);
            channels.Add(REAR, BASSFlag.BASS_SPEAKER_REAR);
            channels.Add(SIDE, BASSFlag.BASS_SPEAKER_REAR2);

            streams = new Dictionary<int, int>();
            streams.Add(FRONT, 0);
            streams.Add(CENTERBASS, 0);
            streams.Add(REAR, 0);
            streams.Add(SIDE, 0);

            Database.instance().connect(ConfigurationSettings.AppSettings["mysql"]);

            Multiroom.addLog("Init mysql");
         
            Multiroom.addLog("Mysql connected");
            
        }

        ~Multiroom()
        {
            Database.instance().close();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static Multiroom instance()
        {
            if (Multiroom._instance == null)
            {
                Multiroom.addLog("=== INSTANCE ===");
                Multiroom._instance = new Multiroom();
            }

            return Multiroom._instance;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        /// <returns></returns>
        public string execute(string message)
        {

            string command = HttpUtility.ParseQueryString(message).Get("command");
            try
            {
                Multiroom.addLog("Command " + command);
                switch (command)
                {
                    case "test":
                        return Test(Convert.ToInt32(HttpUtility.ParseQueryString(message).Get("channel")));
                    case "scan":
                        return Scan();
                    case "play":
                        return Play(HttpUtility.ParseQueryString(message).Get("id"), HttpUtility.ParseQueryString(message).Get("channels"));
                    case "stop":
                        return Stop( HttpUtility.ParseQueryString(message).Get("channels"));
                }
            }
            catch (Exception ex)
            {
                Multiroom.addLog(ex.ToString());
            }

            return "Unknow command";
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        /// <returns></returns>
        public string Test(int channel)
        { 
            string path = @"test.wav";
            int stream = Bass.BASS_StreamCreateFile(path, 0L, 0L, channels[channel]);

            if (stream != 0)
            {
                Bass.BASS_ChannelPlay(stream, false);
            }
            else
            {
                // error creating the stream 
                Multiroom.addLog("Stream error: " + Bass.BASS_ErrorGetCode());
                return "Error " + Bass.BASS_ErrorGetCode();
            }

            return "OK";
        }

        public string Scan()
        {
            string library = ConfigurationSettings.AppSettings["library"];
            try {
                Library.Scan(library);
            }
            catch (Exception ex)
            {
                Multiroom.addLog(ex.ToString());
            }
            return "OK";
        }

        public string Play(string id, string _channels)
        {

            string path = Library.getFile(id);
            Multiroom.addLog("Play "+path);

            string[] chs = explode(",", _channels);
            BASSFlag flags = 0;
            for (int i = 0; i<chs.Length; i++)
            {
                flags = flags | channels[Convert.ToInt32(chs[i])];
            }


            int stream = Bass.BASS_StreamCreateFile(path, 0L, 0L, flags);
            for (int i = 0; i < chs.Length; i++)
            {
                if (streams[Convert.ToInt32(chs[i])] !=0)
                {
                    Player.StopStream(streams[Convert.ToInt32(chs[i])]);
                }
                streams[Convert.ToInt32(chs[i])] = stream;
            }
            if (stream != 0)
            {
                Bass.BASS_ChannelPlay(stream, false);
            }
            else
            {
                // error creating the stream 
                Multiroom.addLog("Stream error: " + Bass.BASS_ErrorGetCode());
                return "Error " + Bass.BASS_ErrorGetCode();
            }
            return "OK";
        }

        public string Stop(string _channels)
        {
            string[] chs = explode(",", _channels);
            BASSFlag flags = 0;
            for (int i = 0; i < chs.Length; i++)
            {
                Player.StopStream(streams[Convert.ToInt32(chs[i])]);
            }
           
            return "OK";
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        public static void addLog(string message)
        {
            Console.WriteLine(message);
        }

        private string[] explode(string separator, string source)
        {
            return source.Split(new string[] { separator }, StringSplitOptions.None);
        }
    }
}
