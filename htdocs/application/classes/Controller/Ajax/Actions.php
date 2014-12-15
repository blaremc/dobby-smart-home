<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Actions extends Controller_Ajax {

    public function action_add() {

        $id = Action::factory()->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Действие добавлено')->setValue(array('id' => $id->id_actions))->out(true);
    }


    public function action_edit() {

        Action::factory($this->request->post('id'))->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Изменения сохранены')->out(true);
    }

    public function action_start(){
        Action::factory($this->request->post('id'))->execute();
    }

}