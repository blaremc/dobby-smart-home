<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Devices extends Controller_Ajax {

    public function action_add() {

        $id = Device::factory()->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Устройство добавлено')->setValue(array('id' => $id->id_devices))->out(true);
    }


    public function action_edit() {

        Device::factory($this->request->post('id'))->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Изменения сохранены')->out(true);
    }

}