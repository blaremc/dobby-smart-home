<?php defined('SYSPATH') or die('No direct script access.');


abstract class Dobby_Scenario {


    public $events = array();


    /**
     * Get device by name or ID
     *
     * @param string|int $name
     * @return Device
     */
    public function device($name) {

        return Device::factory($name);
    }

    /**
     * Init subscribe event
     */
    public function init() {}

    /**
     * Execute when want user
     */
    public function execute() {}

    /**
     * Execute when trigger event
     */
    public function event($event, $device) {}
}