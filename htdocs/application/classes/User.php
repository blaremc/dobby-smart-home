<?php defined('SYSPATH') or die('No direct script access.');


class User {


    public $email = null;
    public $login = null;
    public $pass = null;
    public $sex = null;
    public $avatar = null;
    public $name = null;
    public $is_admin = null;
    public $is_deleted	 = null;
    public $createdate = null;
    public $birth = null;


    public function __construct($data){

        $data = is_array($data) ? $data : array();
        $this->email = isset($data['email']) ? $data['email'] : null;
        $this->login = isset($data['login']) ? $data['login'] : null;
        $this->pass = isset($data['pass']) ? $data['pass'] : null;
        $this->sex = isset($data['sex']) ? $data['sex'] : null;
        $this->avatar = isset($data['avatar']) ? $data['avatar'] : null;
        $this->name = isset($data['name']) ? $data['name'] : null;
        $this->is_admin = isset($data['is_admin']) ? $data['is_admin'] : null;
        $this->is_deleted = isset($data['is_deleted']) ? $data['is_deleted'] : null;
        $this->createdate = isset($data['createdate']) ? $data['createdate'] : null;
        $this->birth = isset($data['birth']) ? $data['birth'] : null;

    }

    public static function factory($data) {

        if (is_numeric($data)) {
            return new User(self::getById($data));
        }
    }

    /**
     * Get user from DB by Id
     * @param $id
     * @return array
     */
    public static function getById($id) {
        return Database::instance()->prepare('SELECT * FROM users WHERE id_users=:user')
            ->bindValue(':user', $id)
            ->execute()
            ->fetch();
    }

    /**
     * Get all users from DB
     * @return array
     */
    public static function getUsersAsArray() {

        return Database::instance()->prepare('SELECT * FROM users')->execute()->fetchAll();
    }

}