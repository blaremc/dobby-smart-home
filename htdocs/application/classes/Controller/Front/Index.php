<?php defined('SYSPATH') or die('No direct script access.');

class Controller_Front_Index extends Controller_Front {


    public function action_index() {

        $ports = Database::instance()->prepare('SELECT * FROM monitored_ports')->execute()->fetchAll();
        foreach ($ports as $port) {

            $value = Dobby_Driver::factory($port['driver'])->getValue($port['address']);
        }
    }
}