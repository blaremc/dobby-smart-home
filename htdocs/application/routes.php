<?php defined('SYSPATH') or die('No direct script access.');

Route::set('admin_devices', 'admin/devices(/<action>)(/<id>)', array('action' => 'add', 'id' => '[0-9]+'))
    ->defaults(array(
        'controller' => 'Devices',
        'action' => 'index',
        'directory' => 'Admin',
    ));

Route::set('admin_actions', 'admin/actions(/<action>)(/<id>)', array('action' => 'add', 'id' => '[0-9]+'))
    ->defaults(array(
        'controller' => 'Actions',
        'action' => 'index',
        'directory' => 'Admin',
    ));

Route::set('admin_scenarios', 'admin/scenarios(/<action>)(/<id>)', array('action' => 'add', 'id' => '[0-9]+'))
    ->defaults(array(
        'controller' => 'Scenarios',
        'action' => 'index',
        'directory' => 'Admin',
    ));


Route::set('admin', 'admin(/<controller>(/<action>(/<id>)))')
    ->defaults(array(
        'controller' => 'Index',
        'action' => 'index',
        'directory' => 'Admin',
    ));


Route::set('login', 'login')
    ->defaults(array(
        'controller' => 'login',
        'action' => 'index',
        'directory' => 'Front',
    ));

Route::set('ajax', 'ajax(/<controller>(/<action>))')
    ->defaults(array(
        'controller' => 'index',
        'action' => 'index',
        'directory' => 'Ajax',
    ));

Route::set('default', '(<controller>(/<action>))')
    ->defaults(array(
        'controller' => 'index',
        'action' => 'index',
        'directory' => 'Front',
    ));