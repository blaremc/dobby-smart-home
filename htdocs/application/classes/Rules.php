<?php defined('SYSPATH') or die('No direct script access.');


class Rules {

    static protected $_instance;

    /**
     * @var array Rules for login and email
     */
    public $login_email;

    /**
     * @var array Rules for login
     */
    public $login;

    /**
     * @var array Rules for login
     */
    public $roles;


    /**
     * @var array Rules for email
     */
    public $email;

    /**
     * @var array Rules for email not check unique
     */
    public $email_nu;

    /**
     * @var array Rules for password
     */
    public $pass;

    /**
     * @var array Rules for new pass
     */
    public $npass;

    /**
     * @var array Rules for token
     */
    public $token;

    /**
     * @var array Title
     */
    public $title;

    /**
     * @var array Rules for id
     */
    public $id;

    /**
     * @var array Rules for gender
     */
    public $gender;

    public function __construct() {

        $this->login_email = array(
            array('not_empty'),
            array('min_length', array(':value', 3)),
            array('max_length', array(':value', 32)),

        );

        $this->login = array(
            array('not_empty'),
            array('min_length', array(':value', 3)),
            array('max_length', array(':value', 32)),
            array('alpha_dash'),
            array('Model_Users::isUniqueLogin', array(':value', ':validation', ':field')),
        );

        $this->email = array(
            array('not_empty'),
            array('email'),
            array('Model_Users::isUniqueEmail', array(':value', ':validation', ':field')),
        );

        $this->email_nu = array(
            array('not_empty'),
            array('email'),
        );

        $this->pass = array(
            array('min_length', array(':value', 3)),
            array('not_empty'),
        );

        $this->cpass = array(
            array('min_length', array(':value', 3)),
            array('matches', array(':validation', ':field', 'pass'))
        );

        $this->npass = array(
            array('min_length', array(':value', 3)),
        );

        $this->token = array(
            array('not_empty'),
            array('Auth::secureCheck', array(':value', ':validation', ':field')),
        );

        $this->title = array(
            array('not_empty'),
            array('min_length', array(':value', 3)),
            array('max_length', array(':value', 64)),
        );

        $this->engname = array(
            array('not_empty'),
            array('min_length', array(':value', 3)),
            array('max_length', array(':value', 32)),
            array('alpha_dash'),
        );

        $this->description = array(
            array('not_empty'),
        );

        $this->id = array(
            array('not_empty'),
            array('digit'),
        );

        $this->select_as_checkbox = array(
            array('not_empty'),
            array('in_array', array(':value', array(0, 1)))
        );

        $this->not_empty = array(
            array('not_empty'),
        );

        $this->digit = array(
            array('digit'),
        );

        $this->message = array(
            array('not_empty'),
        );

        $this->color = array(
            array('not_empty'),
            array('color'),
        );

        $this->vote = array(
            array('not_empty'),
        );

    }

    /**
     *
     * @static
     * @return Rules
     */
    public static function instance() {
        if (!isset(self::$_instance)) {


            self::$_instance = new Rules();
        }

        return self::$_instance;
    }

}