<?php defined('SYSPATH') or die('No direct script access.');


abstract class Dobby_Minion_Task extends Minion_Task {

    const POOL_PROC_EXEC_TIME = 1;


    public function _startProcess(array &$aPool, $cmd, $key) {

        // Инициализация данных для запуска процесса

        $aDescriptors = array(
            1 => array("pipe", "w"),
            2 => array("pipe", "w")
        );
        $aPipes = array();

        $bSuccess = true;
        $rProcess = proc_open($cmd, $aDescriptors, $aPipes);
        if (is_resource($rProcess)) {
            $aPool[$key] = array(
                'handler' => $rProcess,
                'pipes' => $aPipes,
                'cmd' => $cmd,
                'time' => time()
            );
        } else {
            $bSuccess = false;
            Kohana::$log->add(Log::TASK, "Не удалось запустить процесс " . $cmd);
        }

        return $bSuccess;
    }

    protected function _waitProcess($aPool) {

        $results = array();
        while (!empty($aPool)) {

            // Ожидание получения данных
            usleep(100);

            // Обработка завершённых процессов
            foreach ($aPool as $key => &$aProcess) {

                // Получение информации о процессе
                $aProcStatus = proc_get_status($aProcess['handler']);

                // Процесс завершён
                if (false === $aProcStatus['running']) {

                    // Получение данных от процесса
                    $results[$key] = fgets($aProcess['pipes'][1]);
                    // Окончание работы с процессом
                    fclose($aProcess['pipes'][1]);
                    fclose($aProcess['pipes'][2]);
                    proc_close($aProcess['handler']);

                    unset($aPool[$key]);

                    // Процесс работает
                } else {

                    // Процесс завис
                    if (time() - $aProcess['time'] > self::POOL_PROC_EXEC_TIME) {
                        Kohana::$log->add(Log::TASK, "Процесс {$key} завис и будет  завершён принудительно");
                        $results[$key] = "Процесс {$key} завис и будет  завершён принудительно";
                        proc_terminate($aProcess['handler'], 15);
                    }
                }
            }
            unset($aProcess);
        }
        return $results;
    }

}