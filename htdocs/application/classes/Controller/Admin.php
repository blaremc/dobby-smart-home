<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Admin extends Controller_Smarty {

    public function after() {
        $this->view->profile = Schedule::getCurrentProfile();
        parent::after();
    }

    public function action_index() {
    }

}