<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Scenarios extends Controller_Ajax
{

    public function action_add() {

        $id = Scenario::factory()->save($this->request->post());

        Message::instance()->isempty(true);
        Message::instance(0, 'Сценарий добавлено')->setValue(array('id' => $id->id_scenarios))->out(true);
    }
    
}