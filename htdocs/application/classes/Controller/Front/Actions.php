<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Front_Actions extends Controller_Front {

    public $template = 'front/actions/index';
    public function action_index() {

        $this->view->set('actions', Action::getActionsAsArray());
    }

}