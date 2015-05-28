<?php defined('SYSPATH') or die('No direct script access.');

class Controller_Front extends Controller_Smarty {

    public function after() {
        $this->view->profile = Schedule::getCurrentProfile();
        parent::after();
    }
}