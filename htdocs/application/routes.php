<?php defined('SYSPATH') or die('No direct script access.');


Route::set('admin', 'admin(/<controller>(/<action>))')
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

Route::set('default', '(<controller>(/<action>))')
    ->defaults(array(
        'controller' => 'index',
        'action' => 'index',
        'directory' => 'Front',
    ));