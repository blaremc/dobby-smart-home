<?php defined('SYSPATH') or die('No direct script access.');

class Scenario_Living extends Dobby_Scenario {


    public $events = array(
        'LivingMotion' => EventBus::DEVICE_CHANGE,
        'LivingLight' => EventBus::DEVICE_CHANGE,
        'LivingIRReceiver' => EventBus::DEVICE_CHANGE,
        'KitchenMotion' => EventBus::DEVICE_CHANGE,
        'KitchenLight' => EventBus::DEVICE_CHANGE,
    );

    /**
     * Default params value
     *
     * @var array
     */
    public $params = array(
        'name' => 'setLight',
        'center_color' => null,
        'border_color' => null,
        'center_border_color' => null,
        'enable_main_light' => 0,
        'enable_window_light' => 0,

    );

    public $fields = array(
        'name' => array('type' => array('setLight'), 'caption' => 'Операция'),
        'center_color' => array('type' => 'color', 'caption' => 'Цвет центральной люстры'),
        'center_border_color' => array('type' => 'color', 'caption' => 'Цвет центральной ниш'),
        'border_color' => array('type' => 'color', 'caption' => 'Цвет ниш у стен'),
        'enable_main_light' => array('type' => 'bool', 'caption' => 'Включен главный свет на кухне'),
        'enable_window_light' => array('type' => 'bool', 'caption' => 'Включены рабочий свет на кухне'),
    );


    protected function _execute($params, $switcher) {
        switch ($params['name']) {
            case 'setLight':
                $this->setLight($params, $switcher);
                break;
        }
    }

    public function cancel() {
        $this->set('is_execute', false);

    }

    /**
     * @param $event
     * @param $device
     *
     * @return mixed|void
     */
    public function event($event, $device) {

        switch ($device->name) {

            case 'KitchenMotion':
                $this->checkKitchenWindowLight();
                break;
        }
    }

    public function status($params) {
        return array(
            'center_color' => $this->get('center_color'),
            'border_color' => $this->get('border_color'),
            'center_border_color' => $this->get('center_border_color'),
            'enable_main_light' => $this->get('enable_main_light'),
            'enable_window_light' => $this->get('enable_window_light'),
        );
    }


    protected function setLight($params, $switcher) {

        if (!is_null($params['center_color'])) {

            if ($switcher == '1') {
                $value = $this->get('center_color');
                $value = $value == 'rgb(0, 0, 0)' ? $params['center_color'] : 'rgb(0, 0, 0)';
                preg_match_all('/([\d]+)/', $value, $matches);
                $matches = $matches[0];
                $vals = implode(':', $matches);
                $this->device('LivingLeds')->setValue('1:' . $vals);
                $this->set('center_color', $value);
            } else {
                preg_match_all('/([\d]+)/', $params['center_color'], $matches);
                $matches = $matches[0];
                $value = implode(':', $matches);
                $this->device('LivingLeds')->setValue('1:' . $value);
                $this->set('center_color', $params['center_color']);
            }
        }
        if (!is_null($params['border_color'])) {
            preg_match_all('/([\d]+)/', $params['border_color'], $matches);
            $matches = $matches[0];
            $value = implode(':', $matches);
            $this->device('LivingLeds')->setValue('2:' . $value);
            $this->set('border_color', $params['border_color']);
        }
        if ($params['enable_main_light'] != -1) {
            if ($switcher == '1') {
                $value = $this->get('enable_main_light');
                $value = $value == '1' ? '0' : '1';
                $this->device('KitchenLights')->setValue('2:' . $value);
                $this->set('enable_main_light', $value);
            } else {
                $this->device('KitchenLights')->setValue('2:' . $params['enable_main_light']);
                $this->set('enable_main_light', $params['enable_main_light']);
            }
        }
        if ($params['enable_window_light'] != -1) {

            if ($switcher == '1') {
                $value = $this->get('enable_window_light');
                $value = $value == '1' ? '0' : '1';
                $this->device('KitchenLights')->setValue('1:' . $value);
                $this->set('enable_window_light', $value);
            } else {
                $this->device('KitchenLights')->setValue('1:' . $params['enable_window_light']);
                $this->set('enable_window_light', $params['enable_window_light']);
            }
        }
    }


    protected function checkKitchenWindowLight() {

        $profile = Schedule::getCurrentProfile();

        if ($this->device('KitchenMotion')->last_value) {
            $this->device('KitchenLights')->setValue('1:1');
        } else {
            $this->device('KitchenLights')->setValue('1:0');
        }
    }





}