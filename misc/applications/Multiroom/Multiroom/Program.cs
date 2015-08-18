using System;
using System.Configuration;

namespace Multiroom
{
    class Program
    {
        static void Main(string[] args)
        {
            Multiroom.instance();
            Network.StartListening(ConfigurationSettings.AppSettings["host"], Convert.ToInt32(ConfigurationSettings.AppSettings["port"]));
        }
    }
}
