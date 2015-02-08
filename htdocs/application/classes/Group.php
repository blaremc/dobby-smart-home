<?php defined('SYSPATH') or die('No direct script access.');


class Group {

    public $data = [];

    public function __construct($data) {

        $this->data = is_array($data) ? $data : array();
    }


    /**
     * @param $data
     *
     * @return Group
     */
    public static function factory($data = null) {

        if (is_numeric($data)) {
            return new Group(self::getById($data));
        }
        if (is_string($data)) {
            return new Group(self::getByName($data));
        }
        if (is_null($data)){
            return new Group($data);
        }
        return new Group($data);
    }


    public function save($values) {
        $valid = Validation::factory($values);
        $valid->rules('name', Rules::instance()->not_empty)
            ->rules('ord', Rules::instance()->digit_not_empty)
            ->check();
        Message::instance($valid->errors());

        if (!Message::instance()->isempty()) {
            return false;
        }

        $this->data = $values;
        $this->data['id_groups'] = $values['id'];
        $this->_save();
        return $this;
    }


    protected function _save() {

        if (!empty($this->data['id_groups'])) {
            return Database::instance()->prepare('UPDATE groups SET name=:name, ord=:ord, WHERE id_groups=:id_groups')
                ->bindValue(':name', $this->data['name'])
                ->bindValue(':id_groups', $this->data['id_groups'])
                ->bindValue(':ord', $this->data['ord'])
                ->execute();
        } else {
            $id = Database::instance()->prepare('INSERT INTO groups(name, ord)  VALUES(:name, :ord)')
                ->bindValue(':ord', $this->data['ord'])
                ->bindValue(':name', $this->data['name'])
                ->execute()
                ->lastInsertId();
            $this->data['id_groups'] = $id;
            return $this->data['id_groups'];
        }
    }

    public function delete() {

        Database::instance()->prepare('DELETE FROM groups WHERE id_groups=:id')
            ->bindValue(':id', $this->data['id_groups'], PDO::PARAM_INT)
            ->execute();
    }



    /**
     * @param $id
     *
     * @return array
     * @throws Kohana_Exception
     */
    public static function getById($id) {
        return Database::instance()->prepare('SELECT * FROM groups WHERE id_groups=:id')
            ->bindValue(':id', $id)
            ->execute()
            ->fetch();

    }

    /**
     * @param $name
     *
     * @return array
     * @throws Kohana_Exception
     */
    public static function getByName($name) {
        return Database::instance()->prepare('SELECT * FROM groups WHERE name=:name')
            ->bindValue(':name', $name)
            ->execute()
            ->fetch();
    }

    /**
     * @return array
     * @throws Kohana_Exception
     */
    public static function getList() {
        return Database::instance()->prepare('SELECT groups.*, actions.count
                                                    FROM `groups`
                                                    LEFT JOIN (SELECT COUNT(*) as count, id_groups FROM actions GROUP BY id_groups) actions USING(id_groups)
                                                    ORDER BY ord
                                                    ')
            ->execute()
            ->fetchAll();
    }
}