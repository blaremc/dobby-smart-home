<?php defined('SYSPATH') or die('No direct script access.');


abstract class Dobby_Module {

    const CAPTION = 'Sample';
    const NAME = 'sample';

    public $id;

    public $syncParams = array();

    public function __construct($data) {

        $this->init($data);
    }

    public function setValue($key, $value) {
        Module::instance(self::NAME)->setValue($key, $value);
    }

    public function getValue($key) {
        Module::instance(self::NAME)->getValue($key);
    }

    abstract public function init($data);
}