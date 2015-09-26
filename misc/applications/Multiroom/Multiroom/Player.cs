using System;
using System.Collections.Generic;
using Un4seen.Bass;
using System.IO;
using System.Threading;
using MySql.Data.MySqlClient;
using Un4seen.Bass.Misc;
using Un4seen.Bass.AddOn.Mix;
namespace Multiroom
{

    struct Stream
    {
        public int mix_sream;
        public float volume;
        public string filename;

        public int stream;

        public Stream(int _stream, int _matrix_stream)
        {
            mix_sream = _stream;
            volume = 1f;
            filename = "";
            stream = _matrix_stream;
        }
    }

   

    class Player
    {

        const byte FRONT = 0;
        const byte REAR = 1;
        const byte CENTERBASS = 2;
        const byte SIDE = 3;
        const byte numberOfSpeakers = 4;
       
        static public Dictionary<int, BASSFlag> bass_channels;

        static public Dictionary<int, Stream> streams;
        static public List<Playlist> playlists = new List<Playlist>();

        static private Dictionary<int, string[]> playinfos = new Dictionary<int, string[]>();

        public static void init()
        {
            Multiroom.addLog("Init bass");
            if (!Bass.BASS_Init(-1, 44100, BASSInit.BASS_DEVICE_SPEAKERS, IntPtr.Zero))
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
            streams.Add(FRONT, new Stream(0, 0));
            streams.Add(CENTERBASS, new Stream(0, 0));
            streams.Add(REAR, new Stream(0, 0));
            streams.Add(SIDE, new Stream(0, 0));

            
            loadChannels();
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

        public static void SetVolumeStream(int stream, float value, int slide = 1000, bool not_save = false)
        {
            if (!not_save)
            {
                for (int i = 0; i < streams.Count; i++)
                {
                    if (streams[i].stream == stream)
                    {
                        Stream st = streams[i];
                        st.volume = value;
                        streams[i] = st;
                    }
                }
            }
            Bass.BASS_ChannelSlideAttribute(stream, BASSAttribute.BASS_ATTRIB_VOL, value, slide);
        }


        public static void PlayChannels(string[] channels)
        {
            for (int i = 0; i < channels.Length; i++)
            {
                if (streams[Convert.ToInt32(channels[i])].mix_sream != 0)
                {
                    Player.PlayStream(streams[Convert.ToInt32(channels[i])].mix_sream);
                }
            }
        }
        public static void StopChannels(string[] channels)
        {
            for (int i = 0; i < channels.Length; i++)
            {
                if (streams[Convert.ToInt32(channels[i])].mix_sream != 0)
                {
                    Player.StopChannel(channels[i]);
                }
            }
        }

        public static void StopChannel(string channel)
        {

            Player.StopStream(streams[Convert.ToInt32(channel)].mix_sream);
        }

        public static void setVolumeTemp(string[] channels, float value, bool not_save = false)
        {
            for (int i = 0; i < channels.Length; i++)
            {
                if (!not_save)
                {
                    Stream st = streams[Convert.ToInt32(channels[i])];
                    st.volume = value;
                    streams[Convert.ToInt32(channels[i])] = st;
                }
                if (streams[Convert.ToInt32(channels[i])].mix_sream != 0)
                {

                    Player.SetVolumeStream(streams[Convert.ToInt32(channels[i])].mix_sream, value, 1000, not_save);
                }
            }
            saveChannels();
        }

        public static void setVolume(string[] channels, float value)
        {
            for (int i = 0; i < channels.Length; i++)
            {
                Stream st = streams[Convert.ToInt32(channels[i])];
                st.volume = value;
                streams[Convert.ToInt32(channels[i])] = st;
                if (streams[Convert.ToInt32(channels[i])].mix_sream != 0)
                {
                    Player.SetVolumeStream(streams[Convert.ToInt32(channels[i])].mix_sream, value, 500, false);
                }
            }
            saveChannels();
        }



        public static void restoreVolumeChannels(string[] channels)
        {
            for (int i = 0; i < channels.Length; i++)
            {
                if (streams[Convert.ToInt32(channels[i])].mix_sream != 0)
                {
                    Player.SetVolumeStream(streams[Convert.ToInt32(channels[i])].mix_sream, streams[Convert.ToInt32(channels[i])].volume);
                }
            }
        }

        public static void Stop(string[] channels)
        {
            for (int i = 0; i < playlists.Count; i++)
            {
                playlists[i].stopChannels(channels);
            }
        }

        public static Playlist CreatePlaylist(string[] files, string[] channels)
        {
            Playlist pl = new Playlist(files, channels);

            for (int i = 0; i < playlists.Count; i++)
            {
                playlists[i].removeChannels(channels);

            }
            playlists.Add(pl);
            return pl;
        }

        public static List<Playlist> getActivePlaylists()
        {
            List<Playlist> results = new List<Playlist>();
            for (int i = 0; i < playlists.Count; i++)
            {
                if (playlists[i].hasChannels())
                {
                    results.Add(playlists[i]);
                }
            }
            return results;
        }

        public static Playlist getPlaylist(string id)
        {
            for (int i = 0; i < playlists.Count; i++)
            {
                if (playlists[i].getId() == Convert.ToInt32(id))
                {
                    return playlists[i];
                }
            }
            return null;
        }


        public static Stream CreateStream(string filename, string[] channels)
        {
            Stream res = new Stream(0,0);
            int stream = Bass.BASS_StreamCreateFile(filename, 0L, 0L, BASSFlag.BASS_SAMPLE_FLOAT | BASSFlag.BASS_STREAM_DECODE /* | BASSFlag.BASS_SAMPLE_MONO*/);

            if (stream == 0)
            {
                return res;
            }

            for (int i = 0; i < channels.Length; i++)
            {
                int Channel = BassMix.BASS_Split_StreamCreate(stream, BASSFlag.BASS_SAMPLE_FLOAT | bass_channels[Convert.ToInt32(channels[i])], null); ;
                
                if (streams[Convert.ToInt32(channels[i])].mix_sream != 0)
                {
                    Player.StopChannel(channels[i]);
                }
                int temp = Convert.ToInt32(channels[i]);
                Stream st = streams[temp];
                st.mix_sream = Channel;
                st.stream = stream;
                streams[temp] = st;
                res = st;
                Player.SetVolumeStream(streams[temp].mix_sream, streams[temp].volume, 1);

            }
            return res;
        }


        public static Stream Play(string filename, string[] channels)
        {

            Stream result = CreateStream(filename, channels);
            if (result.mix_sream != 0)
            {
                //BassMix.BASS_Mixer_ChannelPlay(outputMixerStream);
                for (int i = 0; i < channels.Length; i++)
                {
                    Player.PlayStream(streams[Convert.ToInt32(channels[i])].mix_sream);
                }
                return result;
            }
            else
            {
                // error creating the stream 
                Multiroom.addLog("Stream error: " + Bass.BASS_ErrorGetCode());
               // throw new Exception("Error " + Bass.BASS_ErrorGetCode());
                return result;
            }
        }



        public static void PlayInfo(string filename, string[] channels) {

            SYNCPROC _mySync = new SYNCPROC(EndPlayInfo);
            Player.setVolumeTemp(channels, 0f, true);
            Thread.Sleep(1000);

            int stream = Bass.BASS_StreamCreateFile(@filename, 0L, 0L, BASSFlag.BASS_SAMPLE_FLOAT | BASSFlag.BASS_STREAM_DECODE /* | BASSFlag.BASS_SAMPLE_MONO*/);
            playinfos[stream] = channels;
            if (stream != 0)
            {
                Bass.BASS_ChannelSetSync(stream, BASSSync.BASS_SYNC_END | BASSSync.BASS_SYNC_MIXTIME,
                       0, _mySync, IntPtr.Zero);

                for (int i = 0; i < channels.Length; i++)
                {
                    int Channel = BassMix.BASS_Split_StreamCreate(stream, BASSFlag.BASS_SAMPLE_FLOAT | bass_channels[Convert.ToInt32(channels[i])], null);
                    Player.SetVolumeStream(Channel, streams[Convert.ToInt32(channels[i])].volume, 1);
                    Player.PlayStream(Channel);
                }
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
            Player.restoreVolumeChannels(playinfos[channel]);
            playinfos.Remove(handle);
        }

        public static void saveChannels()
        {
            MySqlCommand command_truncate = Database.instance().command("TRUNCATE TABLE channels");
            command_truncate.Prepare();
            command_truncate.ExecuteNonQuery();

            MySqlCommand command = Database.instance().command("INSERT INTO channels(speaker, volume) VALUES(@speaker, @volume)");
            command.Prepare();
            command.Parameters.AddWithValue("@speaker", null);
            command.Parameters.AddWithValue("@volume", null);
            foreach (KeyValuePair<int, Stream> entry in streams)
            {
                command.Parameters["@speaker"].Value = entry.Key;
                command.Parameters["@volume"].Value = entry.Value.volume;
                command.ExecuteNonQuery();
            }
           

        }

        public static void loadChannels()
        {
            MySqlCommand command = Database.instance().command("SELECT speaker, volume FROM channels");
            MySqlDataReader reader = command.ExecuteReader();
            int i = 0;
            List<string> result = new List<string>();
            while (reader.Read())
            {
                int channel = Convert.ToInt32(String.Format("{0}", reader[0]));
                float vol = (float)Convert.ToDouble(String.Format("{0}", reader[1]));
                Stream st = streams[channel];
                st.volume = vol;
                streams[channel] = st;
            }
            reader.Close();
        }
    }
}
