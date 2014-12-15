<?php defined('SYSPATH') or die('No direct script access.');


abstract class Dobby_Module {

    const CAPTION = 'Sample';
    const NAME = 'sample';

    public $id;

    public $syncParams = array();

    public function __construct($data) {

        $this->init($data);
    }

    abstract public function init($data);
}