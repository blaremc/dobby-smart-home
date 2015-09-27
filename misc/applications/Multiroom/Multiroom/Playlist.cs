using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Un4seen.Bass;

namespace Multiroom
{
    struct Song
    {
        public string path;
        public string name;
        public int duration;
        public int order;
        public int index;

        public Song(string filename, int ord, int ind)
        {
            path = filename;
            name = filename;
            duration = 0;
            order = ord;
            index = ind;
        }

    }


    class Playlist
    {
        private SYNCPROC endSyncProc;

        const int REPEAT_NONE = 0;
        const int REPEAT_ALL = 1;
        const int REPEAT_SONG = 2;

        private long id;
        private string[] _channels;
        private Dictionary<string, Song> _songs = new Dictionary<string, Song>();
        private string _current;
        private int _repeat;
        private bool _shuffle = true;
        private string _name;
        private bool _is_playing = false;

        public Playlist(string[] files, string[] channels)
        {
            this._channels = channels;
            Random rnd = new Random();
            for (int i = 0; i < files.Length; i++)
            {
                int ord = i;
                if (_shuffle)
                {
                    bool find = true;

                    while (find)
                    {
                        ord = rnd.Next(0, files.Length);
                        find = false;
                        foreach (KeyValuePair<string, Song> entry in _songs)
                        {
                            if (entry.Value.order == ord)
                            {
                                find = true;
                                break;
                            }
                        }

                    }
                }
                else
                {

                }
                this._songs.Add(files[i], new Song(files[i], ord, i));
            }

            endSyncProc = new SYNCPROC(EndOfFile);

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

        public string getCurrentSong()
        {
            return _current;
        }

        public bool isPlaying()
        {
            return _is_playing;
        }

        public Stream getFirstStream()
        {
            Stream stream = new Stream(0,0);
            for (int i = 0; i < _channels.Length; i++)
            {
                stream = Player.streams[Convert.ToInt32(_channels[i])];
                return stream;
            }
            return stream;
        }

        public double getCurrentDuration()
        {
            Stream stream = getFirstStream();
            if (stream.stream != 0)
            {
                // length in bytes 
                long len = Bass.BASS_ChannelGetLength(stream.stream, BASSMode.BASS_POS_BYTES);
                // the time length 
                double time = Bass.BASS_ChannelBytes2Seconds(stream.stream, len);
                return Math.Round(time, 2);
            }
            else
            {
                return 0;
            }
        }

        public double getCurrentPosition()
        {
            Stream stream = getFirstStream();
            if (stream.stream != 0)
            {
                // playback duration 
                double time = Bass.BASS_ChannelBytes2Seconds(stream.stream, Bass.BASS_ChannelGetPosition(stream.stream));
                return Math.Round(time, 2);
            }
            else
            {
                return 0;
            }
        }



        public void setCurrentPosition(double position)
        {
            for (int i = 0; i < _channels.Length; i++)
            {
                Bass.BASS_ChannelSetPosition(Player.streams[Convert.ToInt32(_channels[i])].stream, position);
            }
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


        public bool stopChannels(string[] channels)
        {
            bool find = false;
            for (int i = 0; i < _channels.Length; i++)
            {
                for (int j = 0; j < channels.Length; j++)
                {
                    if (_channels[i] == channels[j])
                    {
                        Stop();
                        find = true;
                    }
                }
            }
            return find;
        }




        public bool hasChannels()
        {
            return _channels.Length > 0;
        }

        public bool hasChannels(string[] channels)
        {
            bool find = false;
            for (int i = 0; i < _channels.Length; i++)
            {
                for (int j = 0; j < channels.Length; j++)
                {
                    if (_channels[i] == channels[j])
                    {
                        Stop();
                    }
                }
            }
            return find;
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

        public void Play(string path = null, bool play = true)
        {

            Stream stream = getFirstStream();
            if (path == null && stream.stream == 0)
            {
                return;
            }
            if ((stream.stream == 0 || _current != path) && path != null)
            {
                if (play)
                {
                    Player.Play(path, _channels);
                    _is_playing = true;
                }
                else
                {
                    Player.CreateStream(path, _channels); 
                }
                stream = getFirstStream();
                Bass.BASS_ChannelSetSync(stream.stream, BASSSync.BASS_SYNC_END | BASSSync.BASS_SYNC_MIXTIME,
                    0, endSyncProc, IntPtr.Zero);
                this._current = path;
            }
            else
            {
                for (int i = 0; i < _channels.Length; i++)
                {
                    // Если это воспроизведение того же файла
                    Player.PlayStream(Player.streams[Convert.ToInt32(_channels[i])].mix_sream);
                    _is_playing = true;
                }
            }
           
        }

        public void Pause()
        {
            for (int i = 0; i < _channels.Length; i++)
            {
                _is_playing = false;
                Player.PauseStream(Player.streams[Convert.ToInt32(_channels[i])].mix_sream);
            }
        }

        private void EndOfFile(int syncHandle, int channel, int data, IntPtr user)
        {
            Next();
        }

        public void Stop()
        {
            for (int i = 0; i < _channels.Length; i++)
            {
                _is_playing = false;
                Player.StopStream(Player.streams[Convert.ToInt32(_channels[i])].mix_sream);
                Bass.BASS_ChannelSetPosition(Player.streams[Convert.ToInt32(_channels[i])].mix_sream, 0);
            }
        }

        public void Next()
        {
            int ord = this._songs[this._current].order;
            foreach (KeyValuePair<string, Song> entry in _songs)
            {
                if (entry.Value.order == ord + 1)
                {
                    this.Play(entry.Key, _is_playing);
                    return;
                }
            }
            ord = -1;
            foreach (KeyValuePair<string, Song> entry in _songs)
            {
                if (entry.Value.order == ord + 1)
                {
                    this.Play(entry.Key, _is_playing);
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
                    this.Play(entry.Key, _is_playing);
                    return;
                }
            }

            ord = _songs.Count - 1;
            foreach (KeyValuePair<string, Song> entry in _songs)
            {
                if (entry.Value.order == ord + 1)
                {
                    this.Play(entry.Key, _is_playing);
                    return;
                }
            }
        }
    }
}
