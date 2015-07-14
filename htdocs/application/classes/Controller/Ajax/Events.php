<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Events extends Controller_Ajax
{

    public function action_index()
    {


        //Device::factory();

        Kohana::$log->add(Log::TASK, print_r($_GET, true));
        Kohana::$log->add(Log::TASK, print_r($_SERVER, true));
    }
}