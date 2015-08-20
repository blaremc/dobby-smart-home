using System;
using System.Collections.Generic;
using Un4seen.Bass;
using System.IO;
using System.Threading;
namespace Multiroom
{

    struct Stream
    {
        public int bass_sream;
        public float volume;
        public string filename;

        public Stream(int stream)
        {
            bass_sream = stream;
            volume = 1f;
            filename = "";
        }

        public void setStream(int stream)
        {
            bass_sream = stream;
        }
    }


    class Player
    {

        const byte FRONT = 0;
        const byte REAR = 1;
        const byte CENTERBASS = 2;
        const byte SIDE = 3;

        static public Dictionary<int, BASSFlag> bass_channels;

        static public Dictionary<int, Stream> streams;

        static private string[] playinfo_channels;

        public static void init()
        {
            Multiroom.addLog("Init bass");
            if (!Bass.BASS_Init(-1, 44100, BASSInit.BASS_DEVICE_DEFAULT, IntPtr.Zero))
            {
                Bass.BASS_SetConfig(BASSConfig.BASS_CONFIG_CURVE_VOL, true);
                Multiroom.addLog("Bass init error " + Bass.BASS_ErrorGetCode());
            }
            else
            {
                Multiroom.addLog("Bass started");
            }
            //Bass.BASS_PluginLoad("basswv.dll");
            bass_channels = new Dictionary<int, BASSFlag>();
            bass_channels.Add(-1, BASSFlag.BASS_DEFAULT | BASSFlag.BASS_SAMPLE_MONO);
            bass_channels.Add(FRONT, BASSFlag.BASS_SPEAKER_FRONT);
            bass_channels.Add(CENTERBASS, BASSFlag.BASS_SPEAKER_CENLFE);
            bass_channels.Add(REAR, BASSFlag.BASS_SPEAKER_REAR);
            bass_channels.Add(SIDE, BASSFlag.BASS_SPEAKER_REAR2);

            streams = new Dictionary<int, Stream>();
            streams.Add(FRONT, new Stream(0));
            streams.Add(CENTERBASS, new Stream(0));
            streams.Add(REAR, new Stream(0));
            streams.Add(SIDE, new Stream(0));

     
        }


        public static void StopStream(int stream)
        {
            Bass.BASS_ChannelStop(stream);
        }

        public static void PauseStream(int stream)
        {
            Bass.BASS_ChannelPause(stream);
        }

        public static void PlayStream(int stream)
        {
            Bass.BASS_ChannelPlay(stream, false);
        }

        public static void SetVolumeStream(int stream, float value)
        {
            Bass.BASS_ChannelSlideAttribute(stream, BASSAttribute.BASS_ATTRIB_VOL, value, 1000);
        }


        public static void PlayChannels(string[] channels)
        {
            for (int i = 0; i < channels.Length; i++)
            {
                if (streams[Convert.ToInt32(channels[i])].bass_sream != 0)
                {
                    Player.PlayStream(streams[Convert.ToInt32(channels[i])].bass_sream);
                }
            }
        }
        public static void StopChannels(string[] channels)
        {
            for (int i = 0; i < channels.Length; i++)
            {
                if (streams[Convert.ToInt32(channels[i])].bass_sream != 0)
                {
                    Player.StopStream(streams[Convert.ToInt32(channels[i])].bass_sream);
                }
            }
        }

        public static void setVolumeChannels(string[] channels, float value)
        {
            for (int i = 0; i < channels.Length; i++)
            {
                if (streams[Convert.ToInt32(channels[i])].bass_sream != 0)
                {
                    Player.SetVolumeStream(streams[Convert.ToInt32(channels[i])].bass_sream, value);
                }
            }
        }


        public static void Stop(string[] channels)
        {
            BASSFlag flags = 0;
            for (int i = 0; i < channels.Length; i++)
            {
                Player.StopStream(streams[Convert.ToInt32(channels[i])].bass_sream);
            }

        }

        public static void Play(string filename, string[] channels)
        {
            BASSFlag flags = 0;
            for (int i = 0; i < channels.Length; i++)
            {
                flags = flags | bass_channels[Convert.ToInt32(channels[i])];
            }

            int stream = Bass.BASS_StreamCreateFile(filename, 0L, 0L, flags);
            for (int i = 0; i < channels.Length; i++)
            {
                if (streams[Convert.ToInt32(channels[i])].bass_sream != 0)
                {
                    Player.StopStream(streams[Convert.ToInt32(channels[i])].bass_sream);
                }
                Stream st = streams[Convert.ToInt32(channels[i])];
                st.bass_sream = stream;
                streams[Convert.ToInt32(channels[i])] = st;
            }
            if (stream != 0)
            {
                Player.PlayStream(stream);
            }
            else
            {
                // error creating the stream 
                Multiroom.addLog("Stream error: " + Bass.BASS_ErrorGetCode());
                throw new Exception("Error " + Bass.BASS_ErrorGetCode());
            }
        }


        public static void PlayInfo(string filename, string[] channels) {


            playinfo_channels = channels;
            BASSFlag flags = 0;
            for (int i = 0; i < channels.Length; i++)
            {
                flags = flags | bass_channels[Convert.ToInt32(channels[i])];
            }
            SYNCPROC _mySync = new SYNCPROC(EndPlayInfo);
            Player.setVolumeChannels(playinfo_channels, 0f);
            Thread.Sleep(1000);
            Player.StopChannels(channels);

            int stream = Bass.BASS_StreamCreateFile(@filename, 0L, 0L, flags);
            if (stream != 0)
            {
                Bass.BASS_ChannelSetSync(stream, BASSSync.BASS_SYNC_END | BASSSync.BASS_SYNC_MIXTIME,
                       0, _mySync, IntPtr.Zero);
                Player.PlayStream(stream);
            }
            else
            {
                // error creating the stream 
                Multiroom.addLog("Stream error: " + Bass.BASS_ErrorGetCode());
                throw new Exception("Error " + Bass.BASS_ErrorGetCode());
            }

        }

        private static void EndPlayInfo(int handle, int channel, int data, IntPtr user)
        {
            // the 'channel' has ended - jump to the beginning
            Player.PlayChannels(playinfo_channels);
            Player.setVolumeChannels(playinfo_channels, 1f);
           // Thread.Sleep(1000);
          
        }
    }
}
