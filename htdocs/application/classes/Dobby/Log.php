<?php defined('SYSPATH') or die('No direct script access.');


class Dobby_Log {

    const INFO = Log::INFO;
    const ERROR = Log::ERROR;


    /**
     * @param     $message
     * @param int $type
     */
    public function add($message, $type = Dobby_Log::INFO) {

        Database::instance()->prepare('INSERT INTO logs(type, message, create_date) VALUES(:type, :message, NOW())')
            ->bindValue(':type', $type)
            ->bindValue(':message', $message)
            ->execute();

    }

    public function getList($logId) {

        $items =  Database::instance()->prepare('SELECT * FROM logs WHERE id_logs > 0 ORDER BY id_logs LIMIT 30 ')
            ->bindValue(':logid', $logId, PDO::PARAM_INT)
            ->execute()
            ->fetchAll();

        return $items;
    }
}