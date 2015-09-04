using System;
using System.Collections.Generic;
using Un4seen.Bass;
using System.IO;
using System.Threading;
using MySql.Data.MySqlClient;
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

    struct Song
    {
        public string path;
        public string name;
        public int duration;
        public int order;

        public Song(string filename, int ord)
        {
            path = filename;
            name = filename;
            duration = 0;
            order = ord;
        }

    }

    class Playlist
    {
        const int REPEAT_NONE = 0;
        const int REPEAT_ALL = 1;
        const int REPEAT_SONG = 2;

        private long id;
        private string[] _channels;
        private Dictionary<string, Song> _songs = new Dictionary<string, Song>();
        private int _stream;
        private string _current;
        private int _repeat;
        private bool _shuffle = true;
        private string _name;

        public Playlist(string[] files, string[] channels)
        {
            this._channels = channels;

            for (int i = 0; i < files.Length; i++)
            {
                int ord = i;
                if (_shuffle)
                {
                    bool find = true;
                    Random rnd = new Random();
                    while (find)
                    {
                        ord = rnd.Next(0, files.Length);
                        foreach (KeyValuePair<string, Song> entry in _songs)
                        {
                            if (entry.Value.order == ord)
                            {
                                find = true;
                                break;
                            }
                        }
                        find = false;
                    }
                }
                else
                {

                }
                this._songs.Add(files[i], new Song(files[i], ord));
            }
        }
        public long getId()
        {
            return this.id;
        }

        public void setId(long id)
        {
            this.id = id;
        }

        public string getName()
        {
            return this._name;
        }

        public string[] getChannels()
        {
            return _channels;
        }

        public bool removeChannels(string[] channels)
        {
            bool find = false;
            for (int i = 0; i < _channels.Length; i++)
            {
                for (int j = 0; j < channels.Length; j++)
                {
                    if (_channels[i] == channels[j])
                    {
                        find = true;
                        int numIdx = Array.IndexOf(_channels, _channels[i]);
                        List<string> tmp = new List<string>(_channels);
                        tmp.RemoveAt(numIdx);
                        _channels = tmp.ToArray();
                        i--;
                        break;
                    }
                }
            }
            return find;
        }

        public bool hasChannels()
        {
            return _channels.Length > 0;
        }

        public Dictionary<string, Song> getSongs()
        {
            return _songs;

        }
        public bool getShuffle()
        {
            return _shuffle;
        }

        public int getRepeat()
        {
            return _repeat;
        }

        public void Play(string path)
        {
            this._current = path;
            if (this._stream == 0) {
                this._stream = Player.Play(path, _channels);
            }
            else
            {
                // Если это воспроизведение того же файла
                Player.PlayStream(this._stream);
            }
        }

        public void Next()
        {
            int ord = this._songs[this._current].order;
            foreach (KeyValuePair<string, Song> entry in _songs)
            {
                if (entry.Value.order == ord + 1)
                {
                    Player.Play(entry.Key, this._channels);
                    return;
                }
            }

        }

        public void Prev()
        {
            int ord = this._songs[this._current].order;
            foreach (KeyValuePair<string, Song> entry in _songs)
            {
                if (entry.Value.order == ord - 1)
                {
                    Player.Play(entry.Key, this._channels);
                    return;
                }
            }

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
        static public List<Playlist> playlists = new List<Playlist>();

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
                    Player.StopChannel(channels[i]);
                }
            }
        }

        public static void StopChannel(string channel)
        {

            Player.StopStream(streams[Convert.ToInt32(channel)].bass_sream);
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


        public static int Play(string filename, string[] channels)
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
                    Player.StopChannel(channels[i]);
                }
                Stream st = streams[Convert.ToInt32(channels[i])];
                st.bass_sream = stream;
                streams[Convert.ToInt32(channels[i])] = st;
            }
            if (stream != 0)
            {
                Player.PlayStream(stream);
                return stream;
            }
            else
            {
                // error creating the stream 
                Multiroom.addLog("Stream error: " + Bass.BASS_ErrorGetCode());
                throw new Exception("Error " + Bass.BASS_ErrorGetCode());
                return 0;
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
