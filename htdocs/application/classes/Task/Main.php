<?php defined('SYSPATH') or die('No direct script access.');


/**
 * Main task of Dobby — checking all ports and waiting is changing.
 * Class Task_Main
 */
class Task_Main extends Minion_Task  {

    protected function _execute ( array $params )
    {
        $ports = Database::instance()->prepare('SELECT * FROM monitored_ports')->execute()->fetchAll();
        foreach ($ports as $port){

            $value = Dobby_Driver::factory($port['driver'])->getValue($port['address']);
            Minion_CLI::write($port['address'].' '. $value."\n");
        }

    }
}