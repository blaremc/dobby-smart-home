<?php defined('SYSPATH') or die('No direct script access.');


/**
 * Main class application
 * Class Dobby
 */
class Dobby {

    /**
     * @var Dobby_Log
     */
    public static $log = null;

    public static $modules = array();

    public static function init() {

        I18n::lang('ru');
        self::$log = new Dobby_Log();
    }


    /**
     * @param $name
     * @param $id
     */
    public static function registrationModule($name, $id) {

        self::$modules[] = array('name' => $name, 'id' => $id);
    }


}