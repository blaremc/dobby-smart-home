<?php defined('SYSPATH') or die('No direct script access.');


class EventBus {

    const DEVICE_CHANGE = 1;
    const DEVICE_UPDATE = 2;
    const SCENARIO_COMPLETE = 3;

    /**
     * @var Dobby_Scenario[][]
     */
    protected $_subscribes = array();

    /**
     * @var Dobby_Scenario[]
     */
    protected $_scenarios = array();

    /**
     * @var EventBus
     */
    protected static $_instance = null;

    public function __construct() {

        $scenarios = Scenario::getScenarios();
        foreach ($scenarios as $scenario) {
            $this->_scenarios[] = $scenario->getClass();
        }
        $this->_init();
    }

    /**
     * @return EventBus
     */
    public static function instance() {

        if (!self::$_instance) {
            self::$_instance = new EventBus();
        }
        return self::$_instance;
    }

    protected function _init() {

        foreach ($this->_scenarios as $scenario) {

            foreach ($scenario->events as $key => $event) {
                if (is_array($event)) {
                    foreach ($event as $e) {
                        $this->_subscribes[$key . '_' . $e][] = $scenario;
                    }
                } else {
                    $this->_subscribes[$key . '_' . $event][] = $scenario;
                }
            }
        }
    }

    /**
     * @param int             $event
     * @param Device|Scenario $object
     */
    public function trigger($event, $object) {

        if (empty($this->_subscribes[$object->name . '_' . $event])) {
            return;
        }
        foreach ($this->_subscribes[$object->name . '_' . $event] as $scenario) {
            $scenario->event($event, $object);
        }
    }


    public static function addEvent($event, $scenario) {

    }

}