<?php defined('SYSPATH') or die('No direct script access.');


class Schedule {

    public $data = [];

    public static $occur = [
        'once' => 'Однажды',
        'every day' => 'Каждый день',
        'every weekday' => 'Каждый рабочий день',
        'every dayname' => 'Каждый %dayname%',
        'every daynum of month' => 'Каждый %daynum% день месяца',
        'every date of year' => 'Каждый %datenum%',
    ];

    public function __construct($data) {
        $this->data = is_array($data) ? $data : array();
    }


    public function save($values) {
        $valid = Validation::factory($values);
        $valid->rules('profile', Rules::instance()->digit)
            ->rules('occur', Rules::instance()->not_empty)
            ->rules('date', Rules::instance()->not_empty)
            ->rules('timefrom', Rules::instance()->not_empty)
            ->rules('timeto', Rules::instance()->not_empty)
            ->check();
        Message::instance($valid->errors());

        if (!Message::instance()->isempty())
            return false;

        $this->data = $values;
        $this->data['id_profiles'] = $values['profile'];
        $this->data['id_schedules'] = $values['id'];

        $this->_save();
        return $this;
    }

    /**
     * Save data to DB
     */
    protected function _save() {

        if (!empty($this->data['id_schedules'])) {
            Database::instance()->prepare('UPDATE schedules SET id_profiles=:id_profiles, date=:date, occur=:occur, timefrom=:timefrom, timeto=:timeto
                                           WHERE id_schedules=:id')
                ->bindValue(':id_profiles', $this->data['id_profiles'], PDO::PARAM_INT)
                ->bindValue(':date', date("Y-m-d H:i:s", strtotime($this->data['date'])))
                ->bindValue(':occur', $this->data['occur'])
                ->bindValue(':timefrom', date("H:i:s", strtotime($this->data['timefrom'])))
                ->bindValue(':timeto', date("H:i:s", strtotime($this->data['timeto'])))
                ->bindValue(':id', $this->data['id_schedules'], PDO::PARAM_INT)
                ->execute();
        } else {
            $id = Database::instance()->prepare('INSERT INTO schedules(id_profiles, date, occur, timefrom, timeto)
                                                            VALUES(:id_profiles, :date, :occur, :timefrom, :timeto)')
                ->bindValue(':id_profiles', $this->data['id_profiles'], PDO::PARAM_INT)
                ->bindValue(':date', date("Y-m-d H:i:s", strtotime($this->data['date'])))
                ->bindValue(':occur', $this->data['occur'])
                ->bindValue(':timefrom', date("H:i:s", strtotime($this->data['timefrom'])))
                ->bindValue(':timeto', date("H:i:s", strtotime($this->data['timeto'])))
                ->execute()
                ->lastInsertId();
            $this->data['id_schedules'] = $id;
        }
    }

    public function delete() {

        Database::instance()->prepare('DELETE FROM schedules WHERE id_schedules=:id')
            ->bindValue(':id', $this->data['id_schedules'], PDO::PARAM_INT)
            ->execute();
    }

    /**
     * @param $data
     *
     * @return Schedule
     */
    public static function factory($data = null) {

        if (is_numeric($data)) {
            return new Schedule(self::getById($data));
        }
        if (is_null($data)) {
            return new Schedule($data);
        }
        return new Schedule($data);
    }


    /**
     * @param $id
     *
     * @return array
     * @throws Kohana_Exception
     */
    public static function getById($id) {
        return Database::instance()->prepare('SELECT * FROM schedules WHERE id_schedules=:id')
            ->bindValue(':id', $id)
            ->execute()
            ->fetch();

    }

    /**
     * @return array
     * @throws Kohana_Exception
     */
    public static function getList() {
        $items = Database::instance()->prepare('SELECT schedules.*, profiles.name as profile_name
                                                FROM schedules
                                                JOIN profiles USING(id_profiles)
                                                ORDER BY id_schedules')
            ->execute()
            ->fetchAll();
        foreach ($items as $key => $value) {
            $date = strtotime($value['date']);
            $items[$key]['occur'] = str_replace(
                array(
                    '%dayname%',
                    '%daynum%',
                    '%datenum%'
                ),
                array(
                    date('l', $date),
                    date('j', $date),
                    date('j F', $date),
                ), Schedule::$occur[$value['occur']]);
        }

        return $items;
    }

    /**
     * @return array
     * @throws Kohana_Exception
     */
    public static function getCurrentProfile() {

        $date = time();
        $items = Database::instance()->prepare('
                            SELECT schedules.*, profiles.*
                            FROM schedules
                            JOIN profiles USING(id_profiles)
                            WHERE
                            (occur = "once" and date=:date and timefrom<=:time and timeto>=:time) or
                            (occur = "every day" and timefrom<=:time and timeto>=:time) or
                            (occur = "every weekday" and DAYOFWEEK(NOW()) > 1 and DAYOFWEEK(NOW()) < 7 and timefrom<=:time and timeto>=:time) or
                            (occur = "every dayname" and DAYOFWEEK(NOW()) = DAYOFWEEK(date) and timefrom<=:time and timeto>=:time) or
                            (occur = "every daynum of month" and DAYOFMONTH(NOW()) = DAYOFMONTH(date)  and timefrom<=:time and timeto>=:time) or
                            (occur = "every date of year"  and MONTH(NOW()) = MONTH(date) and DAYOFMONTH(NOW()) = DAYOFMONTH(date)  and timefrom<=:time and timeto>=:time)
                                ')
            ->bindValue(':date', date("Y-m-d", $date))
            ->bindValue(':time', date("H:i:s", $date))
            ->execute()
            ->fetchAll();

        return count($items) ? $items[0] : null;
    }
}