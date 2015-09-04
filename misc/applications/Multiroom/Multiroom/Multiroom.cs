﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Configuration;
using System.Speech.Synthesis;
using System.Speech.AudioFormat;
using Newtonsoft.Json;

namespace Multiroom
{
    class Multiroom
    {

        private static Multiroom _instance;

        public Multiroom()
        {

            Player.init();

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
                        return Test(HttpUtility.ParseQueryString(message).Get("channel"));
                    case "scan":
                        return Scan();
                    case "play":
                        return Play(HttpUtility.ParseQueryString(message).Get("id"), HttpUtility.ParseQueryString(message).Get("channels"));
                    case "pause":
                        return Pause(HttpUtility.ParseQueryString(message).Get("id"));
                    case "stop":
                        return Stop( HttpUtility.ParseQueryString(message).Get("channels"));
                    case "say":
                        return Say(HttpUtility.ParseQueryString(message).Get("text"), HttpUtility.ParseQueryString(message).Get("channels"));
                    case "getplaylists":
                        return GetPlayLists();
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
        public string Test(string channel)
        {
            string[] channels = new string[] { channel };
            
            Player.PlayInfo("test.wav", channels);
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

        public string Play(string id, string channels)
        {

            string path = Library.getFile(id);
            string[] paths = Library.getFilesFromParentFolder(id);
            string[] chs = explode(",", channels);
            Playlist pl = Player.CreatePlaylist(paths, chs);
            pl.Play(path);
            Library.savePlaylist(pl);
            return "OK";
        }

        public string Stop(string channels)
        {
            string[] chs = explode(",", channels);
            Player.Stop(chs);
            return "OK";
        }

        public string Pause(string playlist_id)
        {
            Playlist pl = Player.getPlaylist(playlist_id);
            if (pl == null)
            {
                return "This playlist doen't exist";
            }
            pl.Pause();
            return "OK";
        }


        public string Say(string text, string channels)
        {

          
            var guid = Guid.NewGuid();
            string[] chs = explode(",", channels);
            Directory.CreateDirectory("speech");
            string filename = @"speech\"+ guid+".wav";
            using (SpeechSynthesizer synth = new SpeechSynthesizer())
            {
                
                // Configure the audio output. 
                synth.SetOutputToWaveFile(filename);

                // Build a prompt.
                PromptBuilder builder = new PromptBuilder();
                builder.AppendText(text);

                // Speak the string asynchronously.
                synth.Speak(builder);
                builder = null;

            }
            Player.PlayInfo(filename, chs);

            return "OK";
        }

        public string GetPlayLists()
        {
            List<Playlist> list = Player.getActivePlaylists();
            string result = "";

            List <Dictionary <string, string>> res = new List<Dictionary<string, string>>();
            Dictionary<string, string> dic;
            for (int i=0; i<list.Count; i++)
            {
                dic = new Dictionary<string, string>();
                dic.Add("PlaylistId", list[i].getId().ToString());
                dic.Add("Channels", string.Join(",", list[i].getChannels().Select(x => x.ToString()).ToArray()));
                dic.Add("Current", list[i].getCurrentSong());
                res.Add(dic);
            }
   
            string json = JsonConvert.SerializeObject(res);

            return json;
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
