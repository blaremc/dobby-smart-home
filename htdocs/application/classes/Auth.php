<?php defined('SYSPATH') or die('No direct script access.');


class Auth {


    /**
     * @var Auth
     */
    protected static $_instance = null;

    protected $usid;

    protected $_load = false;

    protected $_isLogin = false;

    protected $_data = null;

    /**
     * @return Auth
     */
    public static function instance() {

        if (!self::$_instance) {
            self::$_instance = new Auth();
        }
        return self::$_instance;
    }


    public function registration($values) {

        $valid = Validation::factory($values)
            ->rules('login', Rules::instance()->login)
            ->rules('email', Rules::instance()->email)
            ->rules('pass', Rules::instance()->pass);
        $valid->check();
        Message::instance($valid->errors());

        if (!Message::instance()->isempty()) return false;



        $id = Database::instance()->prepare('INSERT INTO users(email, login, pass) VALUES (:email, :login, :pass)')
            ->bindValue(':email', $values['email'])
            ->bindValue(':login', $values['login'])
            ->bindValue(':pass', 'TEMP')
            ->execute()
            ->lastInsertId();

        Database::instance()->prepare('UPDATE users SET pass=:pass WHERE id_users=:user')
            ->bindValue(':pass', Auth::generateHash($values['pass'], $id))
            ->bindValue(':user', $id, PDO::PARAM_INT)
            ->execute();

        return $id;
    }


    /**
     * User login
     *
     * @param      $values
     * @param bool $cookie
     * @return bool
     */
    public function login($values, $cookie = true) {

        $valid = Validation::factory($values)
            ->rules('login_email', Rules::instance()->login_email)
            ->rules('pass', Rules::instance()->pass);
        $valid->check();
        Message::instance($valid->errors('validation'));

        // If exist validation errors then exit
        if (!Message::instance()->isempty()) return false;

        // Generate hash
        $user = Database::instance()->prepare('
							SELECT id_users, login, email, pass, birth, sex, is_admin, name
							FROM users
							WHERE (lower(login) = lower(:login) OR lower(email) = lower(:login)) AND is_deleted = FALSE')

            ->bindParam(':login', $values['login_email'])
            ->execute()
            ->fetch();
        $pass = self::generateHash($values['pass'], $user['id_users']);
        // If not find then exit
        if ($user['pass'] != $pass) {
            Message::instance(1, 'Wrong login or pass', 'ipass');
            return false;
        }
        $this->forceLogin($user['id_users'], $cookie);

        unset($user['pass']);

        // Save user data
        $this->_data = $user;

        return $this->usid;
    }


    /**
     * Load user data
     *
     * @param bool $force
     * @return array|bool
     */
    public function loadData($force = true) {

        if (!$force) {
            if ($this->_isLogin)
                return true;

            if ($this->_load)
                return false;
        }

        $usid = $this->usid != '' ? $this->usid : Cookie::get('usid', '');
        if ($usid == '') {
            $this->_data = array();
            return $this->_data;
        }
        $user = Database::instance()->prepare('
				SELECT id_users, login, email, sex, birth, avatar, name, is_admin FROM users
				  JOIN tokens USING(id_users)
                    WHERE id_tokens = :token AND
                          is_deleted = FALSE AND
                          DATE_ADD(tokens.createdate, INTERVAL exp_time_sec SECOND) > NOW()')
            ->bindParam(':token', $usid)
            ->execute()
            ->fetch();

        $this->_data = $user;


        // If not find then exit
        if ($user['id_users'] == '') {

            Cookie::delete('usid');
            return $this->loadData($force);
        }
        $this->_load = true;

        $this->_islogin = true;
        return $this->_data;
    }


    /**
     * Get user id
     *
     * @return int
     */
    public function getId() {

        if (!$this->loadData()) return 0; // Anonim
        return $this->_data['id_users'];
    }

    /**
     * Force user login
     *
     * @param int  $id
     * @param bool $cookie
     * @return string
     */
    public function forceLogin($id, $cookie = true) {

        $token = sha1(time() . uniqid('V(&#GJC', true));

        Database::instance()->prepare('INSERT INTO tokens(id_tokens, id_users, createdate) VALUES(:token, :id_user, NOW())')
            ->bindParam(':token', $token)
            ->bindParam(':id_user', $id, PDO::PARAM_INT)
            ->execute();

        if ($cookie) {
            // Insert token in cookie
            Cookie::set('usid', $token);
        }
        $this->usid = $token;
        return $token;
    }

    /**
     * @return bool
     */
    public function isLogin() {
        return $this->getId() != 0;
    }

    /**
     * @return bool
     */
    public function isAdmin() {

        if (!$this->loadData()) return false;
        return $this->_data['is_admin'];
    }

    /**
     * Generate hash
     *
     * @static
     * @param $pass
     * @param $login
     * @return string
     */
    public static function generateHash($pass, $login) {
        return sha1($pass . Cookie::$salt . 'DobbyHASH$3fscc...545gda' . $login);
    }

}