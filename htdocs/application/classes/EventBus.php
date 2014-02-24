<?php defined('SYSPATH') or die('No direct script access.');


class EventBus {

    const DEVICE_CHANGE = 1;
    const DEVICE_UPDATE = 2;

    /**
     * @var Dobby_Scenario[][]
     */
    protected $_subscribes = array();

    /**
     * @var Dobby_Scenario[]
     */
    protected $_scenarios = array();

    public function __construct() {

        $scenarios = Scenario::getScenarios();
        foreach ($scenarios as $scenario) {
            $this->_scenarios[] = $scenario->getClass();
        }
        $this->_init();
    }

    protected function _init() {

        foreach ($this->_scenarios as $scenario) {

            foreach ($scenario->events as $key => $event) {
                $this->_subscribes[$key . '_' . $event][] = $scenario;
            }
        }
    }

    /**
     * @param int    $event
     * @param Device $device
     */
    public function trigger($event, $device) {

        if (empty($this->_subscribes[$device->name . '_' . $event])){
            return;
        }
        foreach ($this->_subscribes[$device->name . '_' . $event] as $scenario) {
            $scenario->event($event, $device);
        }
    }


    public static function addEvent($event, $scenario) {

    }

}