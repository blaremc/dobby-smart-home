<?php defined('SYSPATH') or die('No direct script access.');


return array(
    'welcome login form' => 'Добро пожаловать в систему умного дома Добби! Для входа представьтесь, пожалуйста',
    'email or login' => 'Электронный адрес или логин',
    'password' => 'Пароль',
    'enter email or login' => 'Введите электронный адрес или логин',
    'enter password' => 'Введите пароль',
    'login' => 'Войти',



    /*
      * Validation
      */

    'login email must not be empty'                          => 'Укажите логин или почтовый адрес',

    'pass must not be empty'                                => 'Укажите пароль',

    'Wrong login or pass'                                   => 'Неправильный логин или пароль',

    'title must not be empty'                               => 'Введите название',

    'description must not be empty'                         => 'Укажите описание',

    'title must be at least 3 characters long'              => 'Название должно быть не короче 3-х символов',

    'engname must not be empty'                             => 'Введите идентификатор',

    'Name must be unique'                                   => 'Название должно быть уникальным',

    'color must not be empty'                               => 'Укажите цвет',

    'color must be a color'                                 => 'Цвет должен быть вида #ffffff',

    'fontcolor must not be empty'                           => 'Укажите цвет шрифта',

    'fontcolor must be a color'                             => 'Цвет шрифта должен быть вида #ffffff',

    'background must not be empty'                          => 'Укажите цвет фона',

    'background must be a color'                            => 'Цвет фона должен быть вида #ffffff',

    'User login must be unique'                             => 'Логин пользователя должен быть уникальным',

    'User email must be unique'                             => 'Почтовый адрес пользователя должен быть уникальным',

    'email must not be empty'                               => 'Укажите почтовый адрес',

    'ulogin must not be empty'                              => 'Укажите логин',

    'Project name must be unique'                           => 'Имя проекта должно быть уникальным',

    'Platform name must be unique'                          => 'Имя площадки должно быть уникальным',

    'Ticket is linked with another ticket'                  => 'Обращение связано с другим обращением',

    'Email is not unique'                                   => 'Электронная почта не уникальна',

    'Information saved'                                     => 'Информация сохранена',

    'Password changed'                                      => 'Пароль изменен',


    'valid'                                    => array(
        'alpha'         => 'Поле :field должно содержать только буквы',
        'alpha_dash'    => 'Поле :field должно содержать только буквы, цифры, тире и знак подчеркивания',
        'alpha_numeric' => 'Поле :field должно содержать только буквы и цифры',
        'color'         => 'Поле :field должно содержать цветовой код',
        'credit_card'   => 'Поле :field должно содержать действительный номер платежной карточки',
        'date'          => 'Поле :field должно содержать дату',
        'decimal'       => array(
            'one'   => 'Поле :field должно содержать число с :param2 десятичным местом',
            'other' => 'Поле :field должно содержать число с :param2 десятичными местами',
        ),
        'digit'         => 'Поле :field должно содержать целое число',
        'email'         => 'Поле :field должно содержать адрес электронной почты',
        'email_domain'  => 'Поле :field должно содержать действительный адрес электронной почты',
        'equals'        => 'Значение поля :field должно быть равно :param2',
        'exact_length'  => array(
            'one'   => 'Поле :field должно быть длиной в :param2 знак',
            'few'   => 'Поле :field должно быть длиной в :param2 знака',
            'other' => 'Поле :field должно быть длиной в :param2 знаков',
        ),
        'in_array'      => 'Поле :field должно содержать один из вариантов на выбор',
        'ip'            => 'Поле :field должно содержать действительный ip адрес',
        'match'         => 'Поле :field должно быть равно значению поля :param2',
        'max_length'    => array(
            'one'   => 'Поле :field должно иметь длину максимум :param2 знак',
            'few'   => 'Поле :field должно иметь длину максимум :param2 знака',
            'other' => 'Поле :field должно иметь длину максимум :param2 знаков',
        ),
        'min_length'    => array(
            'one'   => 'Поле :field должно иметь длину хотя бы :param2 знак',
            'few'   => 'Поле :field должно иметь длину хотя бы :param2 знака',
            'other' => 'Поле :field должно иметь длину хотя бы :param2 знаков',
        ),
        'not_empty'     => 'Поле :field должно быть заполнено',
        'numeric'       => 'Поле :field должно иметь численное значение',
        'phone'         => 'Поле :field должно содержать действительный номер телефона',
        'range'         => 'Величина поля :field должна быть в интервале между :param2 и :param3',
        'regex'         => 'Поле :field должно соответствовать заданному формату',
        'url'           => 'Поле :field должно содержать действительный адрес URL',
        'not_unique'    => 'Значение :value поля :field уже есть в системе',
        'matches'       => 'Поле :field должно совпадать с полем :param3',
    ),
);