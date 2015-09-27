<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Ircommand extends Controller_Ajax {

    public function action_index() {

        $ip = $_SERVER['REMOTE_ADDR'];

        switch($ip){
            case "192.168.1.10":
                echo "1_1597063359 setRele:1:0:|:setRele:2:0\n";
                echo "1_1597095999 setRele:1:1:|:setRele:2:1\n";
                echo "1_1597047039 setRele:1:t\n";
                echo "1_1597079679 setRele:2:t\n";
                break;
            case "192.168.1.15":

                echo "ir2 1_1597063359 setLed:1:0:0:0:20:|:setLed:2:0:0:0:20:|:setLed:5:0:0:0:20\n";
                echo "ir2 1_1597095999 setLed:1:0:0:0:10:|:setLed:2:255:156:86:20:|:setLed:5:255:214:109:20\n";
                echo "ir2 1_1597055199 setLed:1:209:92:31:20:|:setLed:2:214:182:0:20:|:setLed:5:255:248:207:20\n";
                echo "ir2 1_1597087839 setLed:1:135:224:63:20:|:setLed:2:199:207:64:20:|:setLed:5:245:234:162:20\n";
                echo "ir2 1_1597071519 setLed:1:52:47:196:20:|:setLed:2:247:192:114:20:|:setLed:5:72:67:247:20\n";

                echo "ir2 1_1597049079 setLed:1:247:119:40:20:|:setLed:2:255:184:18:20:|:setLed:5:53:143:232:20\n";
                echo "ir2 1_1597059279 setLed:1:232:53:220:20:|:setLed:2:51:190:245:20:|:setLed:5:255:204:0:20\n";
                echo "ir2 1_1597051119 setLed:1:255:235:171:20:|:setLed:2:204:255:102:20:|:setLed:5:181:102:255:20\n";


                echo "ir2 1_1597104159 setLed:1:255:255:255:20:|:setLed:2:255:255:255:20:|:setLed:5:255:255:255:20\n";

                break;
        }


    }

}