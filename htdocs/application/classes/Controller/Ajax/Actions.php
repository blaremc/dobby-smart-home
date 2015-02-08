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

    public function action_start() {
        Action::factory($this->request->post('id'))->execute();
    }

    public function action_status() {
        $action = Action::factory($this->request->param('id'));
        $values = $action->status();
        $value = 0;
        foreach ($action->data as $key => $item) {
            if (!is_null($item) && $item != -1 && $key != 'name') {
                if ($item != $values[$key]) {
                    $value = 1;;
                }
            }
        }
        echo json_encode(array('value' => $value));
    }

}