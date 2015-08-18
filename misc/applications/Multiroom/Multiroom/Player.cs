using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Un4seen.Bass;

namespace Multiroom
{
    class Player
    {

        public static void StopStream(int stream)
        {
            Bass.BASS_ChannelStop(stream);
        }

    }
}
