<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Admin_Groups extends Controller_Admin {

    public $template = 'admin/groups/index';

    public function action_index() {

        if ($this->request->param('id')) {

            $this->template = 'admin/groups/edit';
            $this->view->set('item', Group::factory($this->request->param('id'))->data);
        } else {
            $this->view->set('items', Group::getList());
        }
    }


    public function action_add(){
        $this->template = 'admin/groups/edit';
    }

}