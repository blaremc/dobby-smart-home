using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MySql.Data.MySqlClient;


namespace Multiroom
{
    class Database
    {

        private static Database _instance = null;
        private MySqlConnection mysql;

        public Database()
        {

        }


        public void connect(string url)
        {
            try
            {
                mysql = new MySqlConnection(url);
                mysql.Open();
            }
            catch (Exception ex)
            {
                Multiroom.addLog("Ошибка: " + ex.Message);
                return;
            }
        }

        public void close()
        {
            mysql.Close();
        }
              

        public static Database instance()
        {
            if (_instance == null)
            {
                _instance = new Database();
            }

            return _instance;
        }


        public MySqlCommand command(string query)
        {
            MySqlCommand mysqlcommand = new MySqlCommand(query, mysql);
            return mysqlcommand;

        }


    }
}
