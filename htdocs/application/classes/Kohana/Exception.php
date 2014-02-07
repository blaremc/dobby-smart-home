<?php defined('SYSPATH') or die('No direct script access.');


class Kohana_Exception extends Kohana_Kohana_Exception {

    public static function _handler(Exception $e) {


        if ($e instanceof Dobby_Exception_Unauthorized) {

            die(Request::factory('login')->execute());

        } else {
            return parent::_handler($e);
        }
    }

}