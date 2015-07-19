<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Ajax_Events extends Controller_Ajax
{

    public function action_index()
    {
        $eventBus = new EventBus();
        if (!is_array($_GET['device'])){
            $devices = array($_GET['device']);
        }else{
            $devices = $_GET['device'];
        }
        foreach ($devices as $key => $dev) {
            $device = Device::factory(Device::getDeviceByAddress('http://' . $_SERVER['REMOTE_ADDR'] . '/' . $dev));
            $device->setLastValue($_GET['value'][$key]);
            Dobby::$log->add('http://' . $_SERVER['REMOTE_ADDR'] . '/' . $dev . '  (' . $_GET['value'][$key] . ') Device:' . $device->name);
            $eventBus->trigger(EventBus::DEVICE_CHANGE, $device);
        }
    }
}