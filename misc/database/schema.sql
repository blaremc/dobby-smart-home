
CREATE TABLE IF NOT EXISTS `actions` (
  `id_actions` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(200) NOT NULL,
  `id_users` int(11) NOT NULL,
  `id_scenarios` int(11) NOT NULL,
  `data` text NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_actions`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;

-- --------------------------------------------------------

--
-- Структура таблицы `devices`
--

CREATE TABLE IF NOT EXISTS `devices` (
  `id_devices` int(11) NOT NULL AUTO_INCREMENT,
  `address` varchar(100) NOT NULL,
  `driver` varchar(100) NOT NULL,
  `last_value` varchar(50) DEFAULT NULL,
  `create_date` datetime NOT NULL,
  `last_date` datetime DEFAULT NULL,
  `is_active` smallint(6) NOT NULL DEFAULT '1',
  `name` varchar(100) NOT NULL,
  `error` longtext,
  `execute_date` datetime DEFAULT NULL,
  PRIMARY KEY (`id_devices`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Структура таблицы `device_values`
--

CREATE TABLE IF NOT EXISTS `device_values` (
  `id_devices` int(11) NOT NULL,
  `value` varchar(100) NOT NULL,
  `time` datetime NOT NULL,
  KEY `id_devices` (`id_devices`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Структура таблицы `logs`
--

CREATE TABLE IF NOT EXISTS `logs` (
  `id_logs` int(11) NOT NULL AUTO_INCREMENT,
  `type` smallint(6) NOT NULL,
  `message` text NOT NULL,
  `create_date` datetime NOT NULL,
  PRIMARY KEY (`id_logs`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ;

-- --------------------------------------------------------

--
-- Структура таблицы `modules`
--

CREATE TABLE IF NOT EXISTS `modules` (
  `id_modules` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `data` text,
  `create_date` datetime NOT NULL,
  `last_date` datetime NOT NULL,
  PRIMARY KEY (`id_modules`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ;

-- --------------------------------------------------------

--
-- Структура таблицы `scenarios`
--

CREATE TABLE IF NOT EXISTS `scenarios` (
  `id_scenarios` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `is_active` smallint(6) NOT NULL DEFAULT '1',
  `execute_date` datetime DEFAULT NULL,
  `error` longtext,
  `create_date` datetime NOT NULL,
  `last_date` datetime DEFAULT NULL,
  `data` longtext,
  PRIMARY KEY (`id_scenarios`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ;

-- --------------------------------------------------------

--
-- Структура таблицы `tokens`
--

CREATE TABLE IF NOT EXISTS `tokens` (
  `id_tokens` varchar(100) NOT NULL,
  `id_users` int(11) NOT NULL,
  `exp_time_sec` int(11) NOT NULL DEFAULT '108000',
  `createdate` datetime NOT NULL,
  PRIMARY KEY (`id_tokens`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Структура таблицы `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `id_users` int(11) NOT NULL AUTO_INCREMENT,
  `email` text NOT NULL,
  `login` text NOT NULL,
  `pass` text NOT NULL,
  `sex` enum('male','female') NOT NULL,
  `avatar` varchar(100) NOT NULL,
  `name` varchar(100) NOT NULL,
  `is_admin` tinyint(1) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL,
  `createdate` datetime NOT NULL,
  `birth` date NOT NULL,
  PRIMARY KEY (`id_users`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;