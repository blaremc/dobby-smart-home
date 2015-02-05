<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Admin_Schedules extends Controller_Admin {

    public $template = 'admin/schedules/index';

    public function action_index() {

        if ($this->request->param('id')) {

            $this->template = 'admin/schedules/edit';
            $this->view->set('item', Schedule::factory($this->request->param('id'))->data);
            $this->view->set('profiles', Profile::getList());
            $this->view->occur = Schedule::$occur;
        } else {
            $this->view->set('schedules', Schedule::getList());
        }
    }


    public function action_add(){
        $this->template = 'admin/schedules/edit';
        $this->view->set('profiles', Profile::getList());
        $this->view->occur = Schedule::$occur;
    }

}