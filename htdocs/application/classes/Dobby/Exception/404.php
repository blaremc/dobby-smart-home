<?php defined('SYSPATH') or die('No direct script access.');

class Dobby_Exception_404 extends Dobby_Exception {


    public function get_response()
    {
        $response = Response::factory();

        $view = View::factory('errors/404');

        // We're inside an instance of Exception here, all the normal stuff is available.
        $view->message = $this->getMessage();

        $response->body($view->render());

        return $response;
    }
}