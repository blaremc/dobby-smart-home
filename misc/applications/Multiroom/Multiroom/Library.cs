using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MySql.Data.MySqlClient;

namespace Multiroom
{
    class Library
    {
        private static MySqlCommand insertcommand;
        private static MySqlCommand updatecommand;
        private static MySqlCommand selectscancommand;

        private static long insertCount = 0;
        private static long updateCount = 0;
        private static long deleteCount = 0;

        public static void Scan(string path)
        {
            string[] files = Directory.GetFiles(path, "*.mp3", SearchOption.AllDirectories);
            insertcommand = Database.instance().command("INSERT INTO files(name, is_folder, path, filesize, createdate, id_parent, ext, clevel) VALUES(@name, @is_folder, @path, @filesize, NOW(), @id_parent, @ext, @level)");
            insertcommand.Prepare();
            insertcommand.Parameters.AddWithValue("@name", null);
            insertcommand.Parameters.AddWithValue("@is_folder", null);
            insertcommand.Parameters.AddWithValue("@path", null);
            insertcommand.Parameters.AddWithValue("@filesize", null);
            insertcommand.Parameters.AddWithValue("@id_parent", null);
            insertcommand.Parameters.AddWithValue("@ext", null);
            insertcommand.Parameters.AddWithValue("@is_exist", 1);
            insertcommand.Parameters.AddWithValue("@level", null);

            updatecommand = Database.instance().command("UPDATE files SET is_exist=1, ccount=@count WHERE id_files = @id");
            updatecommand.Parameters.AddWithValue("@count", 0);
            updatecommand.Prepare();
            updatecommand.Parameters.AddWithValue("@id", null);

            selectscancommand = Database.instance().command("SELECT id_files FROM files WHERE path=@path");
            selectscancommand.Prepare();
            selectscancommand.Parameters.AddWithValue("@path", null);

            insertCount = 0;
            updateCount = 0;
            deleteCount = 0;


            MySqlCommand cmd = Database.instance().command("UPDATE files SET is_exist = 0");
            cmd.ExecuteNonQuery();
            TreeScan(path, 0, 0);


            cmd = Database.instance().command("SELECT COUNT(*) FROM files WHERE is_exist = 0");
            deleteCount = Convert.ToInt32(cmd.ExecuteScalar());
            Multiroom.addLog("Scan completed. Inserted ("+insertCount.ToString()+ " Updated (" + updateCount.ToString() + ") Deleted (" + deleteCount.ToString() + ")");


            cmd = Database.instance().command("DELETE FROM files WHERE is_exist = 0");
            cmd.ExecuteNonQuery();

        }




        private static int TreeScan(string sDir, long parent, int level)
        {
            int count = 0;
            foreach (string f in Directory.GetFiles(sDir))
            {
                if (Path.GetExtension(f) != ".mp3")
                {
                    continue;
                }
                count++;
                selectscancommand.Parameters["@path"].Value = f;
                string result = Convert.ToString(selectscancommand.ExecuteScalar());
                if (result != "")
                {
                    updatecommand.Parameters["@id"].Value = result;
                    updatecommand.ExecuteNonQuery();
                    updateCount++;
                    continue;
                }

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
                count++;
                selectscancommand.Parameters["@path"].Value = d;
                string result = Convert.ToString(selectscancommand.ExecuteScalar());
                if (result != "")
                {
                    updatecommand.Parameters["@id"].Value = result;
                    updatecommand.ExecuteNonQuery();
                    updateCount++;
                }
                else
                {
                    insertcommand.Parameters["@name"].Value = Path.GetFileName(d);
                    insertcommand.Parameters["@is_folder"].Value = true;
                    insertcommand.Parameters["@path"].Value = d;
                    insertcommand.Parameters["@filesize"].Value = 0;
                    insertcommand.Parameters["@id_parent"].Value = parent;
                    insertcommand.Parameters["@level"].Value = level;
                    insertcommand.ExecuteNonQuery();
                    insertCount++;
                }
                 TreeScan(d, insertcommand.LastInsertedId, level + 1 );

                selectscancommand.Parameters["@path"].Value = d;
                string id = Convert.ToString(selectscancommand.ExecuteScalar());
                updatecommand.Parameters["@id"].Value = id;
                updatecommand.Parameters["@count"].Value = count;
                updatecommand.ExecuteNonQuery();
                updateCount++;

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
    }
}
