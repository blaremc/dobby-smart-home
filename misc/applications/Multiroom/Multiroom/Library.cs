using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MySql.Data.MySqlClient;
using System.Security.Cryptography;

namespace Multiroom
{
    class Library
    {
        private static long insertCount = 0;
        private static long updateCount = 0;
        private static long deleteCount = 0;

        public static void Scan(string path)
        {
           
            string[] files = Directory.GetFiles(path, "*.mp3", SearchOption.AllDirectories);
            MySqlCommand insertcommand = Database.instance().command("INSERT INTO files(id_files, name, is_folder, path, filesize, createdate, id_parent, ext, clevel) VALUES(@id, @name, @is_folder, @path, @filesize, NOW(), @id_parent, @ext, @level)");
            insertcommand.Prepare();
            insertcommand.Parameters.AddWithValue("@id", null);
            insertcommand.Parameters.AddWithValue("@name", null);
            insertcommand.Parameters.AddWithValue("@is_folder", null);
            insertcommand.Parameters.AddWithValue("@path", null);
            insertcommand.Parameters.AddWithValue("@filesize", null);
            insertcommand.Parameters.AddWithValue("@id_parent", null);
            insertcommand.Parameters.AddWithValue("@ext", null);
            insertcommand.Parameters.AddWithValue("@is_exist", 1);
            insertcommand.Parameters.AddWithValue("@level", null);

            MySqlCommand updatecommand = Database.instance().command("UPDATE files SET is_exist=1, ccount=@count WHERE id_files = @id");
            updatecommand.Prepare();
            updatecommand.Parameters.AddWithValue("@id", null);
            updatecommand.Parameters.AddWithValue("@count", 0);


            MySqlCommand selectscancommand = Database.instance().command("SELECT id_files FROM files WHERE id_files=@id");
            selectscancommand.Prepare();
            selectscancommand.Parameters.AddWithValue("@id", null);

            insertCount = 0;
            updateCount = 0;
            deleteCount = 0;


            MySqlCommand cmd = Database.instance().command("UPDATE files SET is_exist = 0");
            cmd.ExecuteNonQuery();


            TreeScan(path, "", 0, insertcommand, updatecommand, selectscancommand);


            cmd = Database.instance().command("SELECT COUNT(*) FROM files WHERE is_exist = 0");
            deleteCount = Convert.ToInt32(cmd.ExecuteScalar());
            Multiroom.addLog("Scan completed. Inserted ("+insertCount.ToString()+ " Updated (" + updateCount.ToString() + ") Deleted (" + deleteCount.ToString() + ")");


            cmd = Database.instance().command("DELETE FROM files WHERE is_exist = 0");
            cmd.ExecuteNonQuery();
            insertcommand = null;
            selectscancommand = null;
            updatecommand = null;
            cmd = null;
        }




        private static int TreeScan(string sDir, string parent, int level, MySqlCommand insertcommand, MySqlCommand updatecommand, MySqlCommand selectscancommand)
        {
            int count = 0;
            string idmd5;
            foreach (string f in Directory.GetFiles(sDir))
            {
                if (Path.GetExtension(f) != ".mp3")
                {
                    continue;
                }
                idmd5 = GenerateMD5(f);
                count++;
                selectscancommand.Parameters["@id"].Value = idmd5;
                string result = Convert.ToString(selectscancommand.ExecuteScalar());
                if (result != "")
                {
                    updatecommand.Parameters["@id"].Value = idmd5;
                    updatecommand.ExecuteNonQuery();
                    updateCount++;
                    continue;
                }
                insertcommand.Parameters["@id"].Value = idmd5;
                insertcommand.Parameters["@name"].Value =Path.GetFileName(f);
                insertcommand.Parameters["@is_folder"].Value = false;
                insertcommand.Parameters["@path"].Value = f;
                insertcommand.Parameters["@filesize"].Value = new FileInfo(f).Length;
                insertcommand.Parameters["@id_parent"].Value = parent;
                insertcommand.Parameters["@ext"].Value = Path.GetExtension(f);
                insertcommand.Parameters["@level"].Value = level;
                insertcommand.ExecuteNonQuery();
                insertCount++;
            }
            foreach (string d in Directory.GetDirectories(sDir))
            {
                idmd5 = GenerateMD5(d);
                count++;
                selectscancommand.Parameters["@id"].Value = idmd5;
                string result = Convert.ToString(selectscancommand.ExecuteScalar());
                if (result != "")
                {
                    updatecommand.Parameters["@id"].Value = idmd5;
                    updatecommand.ExecuteNonQuery();
                    updateCount++;
                }
                else
                {
         
                    insertcommand.Parameters["@id"].Value = idmd5;
                    insertcommand.Parameters["@name"].Value = Path.GetFileName(d);
                    insertcommand.Parameters["@is_folder"].Value = true;
                    insertcommand.Parameters["@path"].Value = d;
                    insertcommand.Parameters["@filesize"].Value = 0;
                    insertcommand.Parameters["@id_parent"].Value = parent;
                    insertcommand.Parameters["@level"].Value = level;
                    insertcommand.ExecuteNonQuery();
                    insertCount++;
                }

                TreeScan(d, GenerateMD5(d), level + 1, insertcommand, updatecommand, selectscancommand);

                selectscancommand.Parameters["@id"].Value = idmd5;
                string id = Convert.ToString(selectscancommand.ExecuteScalar());
                updatecommand.Parameters["@id"].Value = id;
                updatecommand.Parameters["@count"].Value = count;
                updatecommand.ExecuteNonQuery();
                //updateCount++;

            }
     
            return count;
        }

        public static string getFile(string id)
        {
            MySqlCommand command = Database.instance().command("SELECT path FROM files WHERE id_files=@id");
            command.Prepare();
            command.Parameters.AddWithValue("@id", id);
            return Convert.ToString(command.ExecuteScalar());
        }

        public static string[] getFilesFromParentFolder(string id)
        {
            MySqlCommand command = Database.instance().command("SELECT path FROM files WHERE id_parent=(SELECT id_parent FROM files WHERE id_files=@id) ORDER BY clevel, name");
            command.Prepare();
            command.Parameters.AddWithValue("@id", id);
            MySqlDataReader reader = command.ExecuteReader();
            var i = 0;
            List<string> result = new List<string>();
            while (reader.Read())
            {
                result.Add(String.Format("{0}", reader[0]));
            }
            reader.Close();
            return result.ToArray();
        }


        public static void savePlaylist(Playlist pl)
        {
            long id = pl.getId();
            if (id == 0)
            {
                MySqlCommand command = Database.instance().command("INSERT INTO playlists(name) VALUES(@name)");
                command.Prepare();
                command.Parameters.AddWithValue("@name", pl.getName());
                command.ExecuteNonQuery();
                id = command.LastInsertedId;
                pl.setId(id);

            }
            MySqlCommand delcommand = Database.instance().command("DELETE FROM playlists_files WHERE id_playlists=@id");
            delcommand.Prepare();
            delcommand.Parameters.AddWithValue("@id", id);
            delcommand.ExecuteNonQuery();


            MySqlCommand insert = Database.instance().command("INSERT INTO playlists_files(id_playlists, id_files, ord) VALUES(@id_playlists, @id_songs, @ord)");
            insert.Prepare();
            insert.Parameters.AddWithValue("@id_playlists", id);
            insert.Parameters.AddWithValue("@id_songs", null);
            insert.Parameters.AddWithValue("@ord", 0);
            
            

            Dictionary<string, Song> songs = pl.getSongs();
            foreach (KeyValuePair<string, Song> entry in songs)
            {
                insert.Parameters["@id_songs"].Value = GenerateMD5(entry.Value.path);
                insert.Parameters["@ord"].Value = entry.Value.index;
                insert.ExecuteNonQuery();
            }
            
        }


        private static string GenerateMD5(string str)
        {

            MD5 md5Hash = MD5.Create();
            // Convert the input string to a byte array and compute the hash.
            byte[] data = md5Hash.ComputeHash(Encoding.UTF8.GetBytes(str));

            // Create a new Stringbuilder to collect the bytes
            // and create a string.
            StringBuilder sBuilder = new StringBuilder();

            // Loop through each byte of the hashed data 
            // and format each one as a hexadecimal string.
            for (int i = 0; i < data.Length; i++)
            {
                sBuilder.Append(data[i].ToString("x2"));
            }

            // Return the hexadecimal string.
            return sBuilder.ToString();
        }

    }
}
