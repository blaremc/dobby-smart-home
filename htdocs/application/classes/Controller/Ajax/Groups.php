<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Groups extends Controller_Ajax {

    public function action_add() {

        $id = Group::factory()->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Группа добавлена')->setValue(array('id' => $id->data['id_groups']))->out(true);
    }


    public function action_edit() {

        Group::factory($this->request->post('id'))->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Группа сохранена')->out(true);
    }

    public function action_delete() {
        Group::factory($this->request->post('id'))->delete();

        Message::instance()->isempty(true);
        Message::instance(0, 'Группа удалена')->out(true);
    }

}