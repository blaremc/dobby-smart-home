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
        $time_start = microtime(true);
        $devices = Device::getDevices();
        $ind = rand(1, 10000);
        Dobby::$log->add('START Main Task [' . $ind . ']');
        $eventBus = new EventBus();


        // первый запуск
        $aPool = array();
        foreach ($devices as $device) {
            if ($device->is_active) {
                $this->_startProcess($aPool, 'php ' . DOCROOT . '/index.php --task=Main --device=' . $device->id_devices, $device->id_devices);
            }
            $eventBus->trigger(EventBus::TIME, $device);
        }


        while (true) {


            // Ожидание получения данных
            usleep(100);

            // Обработка завершённых процессов
            foreach ($aPool as $key => &$aProcess) {

                // Получение информации о процессе
                $aProcStatus = proc_get_status($aProcess['handler']);

                // Процесс завершён
                if (false === $aProcStatus['running']) {

                    // Получение данных от процесса
                    $value = fgets($aProcess['pipes'][1]);
                    // Окончание работы с процессом
                    fclose($aProcess['pipes'][1]);
                    fclose($aProcess['pipes'][2]);
                    proc_close($aProcess['handler']);

                    // Обрабатываем результат

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

                    $this->_startProcess($aPool, 'php ' . DOCROOT . '/index.php --task=Main --device=' . $key, $key);
                } else {

                    // Процесс завис
                    if (time() - $aProcess['time'] > self::POOL_PROC_EXEC_TIME) {
                        Kohana::$log->add(Log::TASK, "Процесс {$key} завис и будет  завершён принудительно");
                        $value = "Process not respond {$key}";
                        proc_terminate($aProcess['handler'], 15);
//                        Dobby::$log->add($value);
                        $device = Device::factory(intval($key));
                        $eventBus->trigger(EventBus::DEVICE_UPDATE, $device);
                        $device->setError($value);

                        $this->_startProcess($aPool, 'php ' . DOCROOT . '/index.php --task=Main --device=' . $key, $key);
                    }
                }
            }

            $this->_checkControlFile();

            $time_end = microtime(true);
            $execution_time = ($time_end - $time_start) / 60;
            if ($execution_time > 10.1) { // 10 минут
                Dobby::$log->add('Terminate task [' . $ind . ']');
                die();
            }
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