<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Login extends Controller_Ajax {

    public function action_index() {


        Auth::instance()->login($this->request->post());
        Message::instance()->isempty(true);
        Message::instance(0,'Enter succeed')->out(true);
    }

}