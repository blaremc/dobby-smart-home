<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Admin_Devices extends Controller_Admin {

    public $template = 'admin/devices/index';

    public function action_index() {

        if ($this->request->param('id')) {

            $this->template = 'admin/devices/edit';
            $this->view->set('device', (array)Device::factory($this->request->param('id')));
            $this->view->set('drivers', Dobby_Driver::$drivers);
        } else {
            $this->view->set('devices', Device::getDevicesAsArray());
        }
    }


    public function action_add(){
        $this->template = 'admin/devices/add';
        $this->view->set('drivers', Dobby_Driver::$drivers);
    }

}