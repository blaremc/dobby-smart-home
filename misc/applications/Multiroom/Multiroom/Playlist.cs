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
        private Stream _stream;
        private string _current;
        private int _repeat;
        private bool _shuffle = false;
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

        public double getCurrentDuration()
        {
            // length in bytes 
            long len = Bass.BASS_ChannelGetLength(_stream.stream, BASSMode.BASS_POS_BYTES);
            // the time length 
            double time = Bass.BASS_ChannelBytes2Seconds(_stream.stream, len);
            return Math.Round(time, 2);
        }

        public double getCurrentPosition()
        {
            // playback duration 
            double time = Bass.BASS_ChannelBytes2Seconds(_stream.stream, Bass.BASS_ChannelGetPosition(_stream.stream));
            return Math.Round(time, 2);
        }



        public void setCurrentPosition(double position)
        {
            Bass.BASS_ChannelSetPosition(_stream.stream, position);
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
            if (path == null && _stream.mix_sream == 0)
            {
                return;
            }
            if ((_stream.mix_sream == 0 || _current != path) && path != null)
            {
                if (play)
                {
                    _stream = Player.Play(path, _channels);
                    _is_playing = true;
                }
                else
                {
                    _stream = Player.CreateStream(path, _channels); 
                }

                Bass.BASS_ChannelSetSync(_stream.stream, BASSSync.BASS_SYNC_END | BASSSync.BASS_SYNC_MIXTIME,
                    0, endSyncProc, IntPtr.Zero);
                this._current = path;
            }
            else
            {
                // Если это воспроизведение того же файла
                Player.PlayStream(_stream.mix_sream);
                _is_playing = true;
            }
           
        }

        public void Pause()
        {
            if (_stream.mix_sream != 0)
            {
                _is_playing = false;
                Player.PauseStream(_stream.mix_sream);
            }
        }

        private void EndOfFile(int syncHandle, int channel, int data, IntPtr user)
        {
            Next();
        }

        public void Stop()
        {
            if (_stream.mix_sream != 0)
            {
                _is_playing = false;
                Player.StopStream(_stream.mix_sream);
                Bass.BASS_ChannelSetPosition(_stream.stream, 0);
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
