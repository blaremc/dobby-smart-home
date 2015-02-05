<?php defined('SYSPATH') or die('No direct script access.');


class Profile {

    public $data = [];

    public function __construct($data) {

        $this->data = is_array($data) ? $data : array();


    }


    /**
     * @param $data
     *
     * @return Profile
     */
    public static function factory($data) {

        if (is_numeric($data)) {
            return new Profile(self::getById($data));
        }
        if (is_string($data)) {
            return new Profile(self::getByName($data));
        }
        return new Profile($data);
    }


    /**
     * @param $id
     *
     * @return array
     * @throws Kohana_Exception
     */
    public static function getById($id) {
        return Database::instance()->prepare('SELECT * FROM profiles WHERE id_profiles=:id')
            ->bindValue(':id', $id)
            ->execute()
            ->fetch();

    }

    /**
     * @param $name
     *
     * @return array
     * @throws Kohana_Exception
     */
    public static function getByName($name) {
        return Database::instance()->prepare('SELECT * FROM profiles WHERE name=:name')
            ->bindValue(':name', $name)
            ->execute()
            ->fetch();
    }

    /**
     * @return array
     * @throws Kohana_Exception
     */
    public static function getList() {
        return Database::instance()->prepare('SELECT * FROM profiles ORDER BY id_profiles')
            ->execute()
            ->fetchAll();
    }
}