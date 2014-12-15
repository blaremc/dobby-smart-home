<?php defined('SYSPATH') or die('No direct script access.');


/**
 * Main task of Dobby — checking all ports and waiting is changing.
 * Class Task_Main
 */
class Task_Main extends Dobby_Minion_Task {

    protected $_options = array('device' => null);

    protected function _execute(array $params) {

        if ($params['device']) {
            $this->_device($params['device']);
        } else {
            $this->_main();
        }
    }

    /**
     * Main loop
     */
    protected function _main() {
        $devices = Device::getDevices();

        $eventBus = new EventBus();
        while (true) {
            $aPool = array();
            foreach ($devices as $device) {
                if ($device->is_active) {
                    $this->_startProcess($aPool, 'php ' . DOCROOT . '/index.php --task=Main --device=' . $device->id_devices, $device->id_devices);
                }
            }
            $values = $this->_waitProcess($aPool);
            unset($aPool);

            Kohana::$log->add(LOG::TASK, print_r($values,true));
            foreach ($values as $key => $value) {

                $device = Device::factory(intval($key));
                // todo триггеры в отдельных потоках
                $eventBus->trigger(EventBus::DEVICE_UPDATE, $device);

                if (substr($value, 0, 5) == 'VALUE') {
                    $device->setLastValue(trim(substr($value, 6)));
                } elseif (substr($value, 0, 5) == 'ERROR') {
                    $device->setError(trim(substr($value, 6)));
                }

                if ($device->is_changed) {
                    $eventBus->trigger(EventBus::DEVICE_CHANGE, $device);
                }
            }
            unset($device);
            unset($values);
            $this->_checkControlFile();
        }
    }


    /**
     * Checking control files
     */
    protected function _checkControlFile() {
        if (file_exists(DOCROOT . '/.taskstop')) {
            unlink(DOCROOT . '/.taskstop');
            die();
        }
    }

    /**
     * Get device value and send to cli
     *
     * @param $deviceid
     */
    protected function _device($deviceid) {
        $device = Device::factory($deviceid);
        try {
            $value = $device->getValue();
            Minion_CLI::write('VALUE ' . $value);
        } catch (Exception $e) {
            Kohana::$log->add(Log::TASK, Kohana_Exception::text($e));
            if (!empty($device)) {
                Minion_CLI::write('ERROR ' . $e->getMessage());
            }
        }
    }

}