<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Admin_Scenarios extends Controller_Admin {

    public $template = 'admin/scenarios/index';

    public function action_index() {

        if ($this->request->param('id')) {

            $this->template = 'admin/scenarios/edit';
            $this->view->set('device', (array)Device::factory($this->request->param('id')));
            $this->view->set('drivers', Dobby_Driver::$drivers);
        } else {
            $this->view->set('scenarios', Scenario::getScenariosAsArray());
        }
    }


    public function action_add(){
        $this->template = 'admin/scenarios/add';
        $this->view->set('drivers', Dobby_Driver::$drivers);
    }

}