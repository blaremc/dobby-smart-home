<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Admin_Actions extends Controller_Admin {

    public $template = 'admin/actions/index';

    public function action_index() {

        if ($this->request->param('id')) {

            $this->template = 'admin/actions/edit';
            $this->view->set('action', (array)Action::factory($this->request->param('id')));
            $this->view->set('users', User::getUsersAsArray());
            $this->view->groups = Group::getList();
            $scenarios = Scenario::getScenarios();
            foreach ($scenarios as &$scenario) {
                $class = $scenario->getClass();
                $scenario = (array)$scenario;
                $scenario['params'] = $class->fields;
            }
            $this->view->set('scenarios', $scenarios);
        } else {
            $actions = Action::getActionsAsArray();
            foreach ($actions as &$action) {
                $action['user'] = $action['id_users'] == 0 ? 'Все' : User::factory($action['id_users'])->name;
                $action['scenario'] = Scenario::factory($action['id_scenarios'])->name;
            }
            $this->view->set('actions', $actions);
        }
    }


    public function action_add() {
        $this->template = 'admin/actions/add';
        $this->view->set('users', User::getUsersAsArray());
        $this->view->groups = Group::getList();
        $scenarios = Scenario::getScenarios();
        foreach ($scenarios as &$scenario) {
            $class = $scenario->getClass();
            $scenario = (array)$scenario;
            $scenario['params'] = $class->fields;
        }
        $this->view->set('scenarios', $scenarios);
    }

}