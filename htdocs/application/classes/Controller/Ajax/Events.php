<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Events extends Controller_Ajax
{

    public function action_index()
    {
        $eventBus = new EventBus();
        $device = Device::factory(Device::getDeviceByAddress('http://' . $_SERVER['REMOTE_ADDR'] . '/' . $_GET['device']));
        $device->setLastValue($_GET['value']);
        Dobby::$log->add('http://' . $_SERVER['REMOTE_ADDR'] . '/' . $_GET['device'] . '  (' . $_GET['value'] . ') Device:' . $device->name);
        if ($device->is_changed) {
            $eventBus->trigger(EventBus::DEVICE_CHANGE, $device);
        }
    }
}