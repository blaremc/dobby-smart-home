<?php defined('SYSPATH') or die('No direct script access.');


class Dobby_Driver {

    protected $_timeout = 1;

    public static $drivers;

    /**
     * Factory for create Dobby drivers
     *
     * @param $name
     * @return Dobby_Driver
     * @throws Dobby_Exception
     */
    public static function factory($name) {

        $class = 'Driver_' . ucfirst($name);
        if (class_exists($class)) {
            return new $class;
        }
        throw new Dobby_Exception("Driver not found: " . ucfirst($name));
    }

    public function getValue($address) {
        return null;
    }

    public function setValue($address, $value) {
    }


    /**
     * Parse device address
     *
     * Example: http://192.168.3.1:distance  => Array('http', '192.168.3.1', 'distance')
     *
     * @param $address
     * @return array
     */
    protected function parseAddress($address) {
        $parts = explode('://', $address);
        $protocol = $parts[0];
        $parts = explode(':', $parts[1]);
        array_unshift($parts, $protocol);
        return $parts;
    }

    public static function registerDrivers($name) {
        self::$drivers[] = $name;
    }

}