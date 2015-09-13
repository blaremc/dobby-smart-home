<?php defined('SYSPATH') or die('No direct script access.');


abstract class Dobby_Scenario {


    protected $silence = false;

    /**
     * Subscribed events
     *
     * @var array
     */
    public $events = array();

    /**
     * Fields for form and validation rules
     *
     * @var array
     */
    public $fields = array();

    /**
     * Default values
     *
     * @var array
     */
    public $params = array();

    /**
     * @var Scenario
     */
    protected $_scenario = null;


    public function execute($params, $switcher) {
        $params = array_merge($this->params, $params);
        $this->_execute($params, $switcher);
    }

    /**
     * @param Scenario $scenario
     */
    public function __construct($scenario) {
        $this->_scenario = $scenario;
    }

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
     * Execute when want user
     * @param $params
     * @return mixed
     */
    abstract protected function _execute($params, $switcher);

    abstract public function status($params);

    /**
     * Execute when trigger event
     * @param $event
     * @param $device
     * @return mixed
     */
    abstract public function event($event, $device);

    /**
     * When cancel execution
     */
    abstract public function cancel();


    protected function set($name, $value) {
        $this->_scenario->setData($name, $value);
    }
    protected function saveData() {
        $this->_scenario->saveData();
    }

    protected function get($name) {
        return $this->_scenario->getData($name);
    }

    public function updateData(){
        $this->_scenario->reloadData();
    }
}