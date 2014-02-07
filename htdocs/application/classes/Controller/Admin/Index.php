<?php defined('SYSPATH') or die('No direct script access.');
class Controller_Admin_Index extends Controller_Admin {


    public $template = 'admin/pages/index';

    public function before()
    {
        parent::before();

        if (!Auth::instance()->isLogin()) {
            throw new Dobby_Exception_Unauthorized();
        }

        if (!Auth::instance()->isAdmin()) {
            throw new HTTP_Exception_403();
        }
    }


    public function action_index() {


    }

}