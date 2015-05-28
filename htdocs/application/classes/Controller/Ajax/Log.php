<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Log extends Controller_Ajax {

    public function action_index() {

        $list = Dobby::$log->getList($this->request->post('logId'));
        foreach ($list as $key => $value) {
            $list[$key]['date'] = date('d.m.Y H:i:s', strtotime($value['create_date']));
        }
        echo json_encode($list);
    }

}