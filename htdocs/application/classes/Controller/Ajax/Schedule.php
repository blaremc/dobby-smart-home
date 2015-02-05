<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Schedule extends Controller_Ajax {

    public function action_add() {

        $id = Schedule::factory()->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Расписание добавлено')->setValue(array('id' => $id->data['id_schedules']))->out(true);
    }


    public function action_edit() {

        Schedule::factory($this->request->post('id'))->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Расписание сохранено')->out(true);
    }

    public function action_delete() {
        Schedule::factory($this->request->post('id'))->delete();

        Message::instance()->isempty(true);
        Message::instance(0, 'Расписание удалено')->out(true);
    }

}