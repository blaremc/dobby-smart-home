<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Front_Actions extends Controller_Front {

    public $template = 'front/groups/index';

    public function action_index() {
        if ($this->request->param('id')) {
            $this->template = 'front/actions/index';
            $this->view->set('actions', Action::getActionsAsArray($this->request->param('id')));
        } else {
            $this->view->set('groups', Group::getList());
        }
    }

}