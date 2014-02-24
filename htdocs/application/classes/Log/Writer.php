<?php defined('SYSPATH') or die('No direct script access.');


abstract class Log_Writer extends Kohana_Log_Writer
{

    protected $_log_levels = array(
        LOG_EMERG   => 'EMERGENCY',
        LOG_ALERT   => 'ALERT',
        LOG_CRIT    => 'CRITICAL',
        LOG_ERR     => 'ERROR',
        LOG_WARNING => 'WARNING',
        LOG_NOTICE  => 'NOTICE',
        LOG_INFO    => 'INFO',
        LOG_DEBUG   => 'DEBUG',
        LOG_CRON    => 'TASK',
    );
    
}