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

        if ($logId == 0) {
            $count = Database::instance()->prepare('SELECT COUNT(*) as count FROM logs')
                ->execute()
                ->fetch();

            $count = $count['count'] - 30;

            $items = Database::instance()->prepare('SELECT * FROM logs WHERE id_logs > :logid  ORDER BY id_logs LIMIT ' . $count . ',30')
                ->bindValue(':logid', $logId, PDO::PARAM_INT)
                ->execute()
                ->fetchAll();
        } else {
            $items = Database::instance()->prepare('SELECT * FROM logs WHERE id_logs > :logid  ORDER BY id_logs LIMIT 30')
                ->bindValue(':logid', $logId, PDO::PARAM_INT)
                ->execute()
                ->fetchAll();
        }
        return $items;
    }
}