<?php defined('SYSPATH') or die('No direct script access.');


class Action {

    public $id_actions = null;
    public $name = null;
    public $id_users = null;
    public $id_scenarios = null;
    public $data = null;
    public $is_deleted = null;
    public $id_groups = null;
    public $ord = null;
    public $is_switcher = null;


    public function __construct($data) {

        $data = is_array($data) ? $data : array();
        $this->id_actions = isset($data['id_actions']) ? $data['id_actions'] : null;
        $this->id_users = isset($data['id_users']) ? $data['id_users'] : null;
        $this->id_scenarios = isset($data['id_scenarios']) ? $data['id_scenarios'] : null;
        $this->data = isset($data['data']) ? json_decode($data['data'], true) : null;
        $this->is_deleted = isset($data['is_deleted']) ? $data['is_deleted'] : null;
        $this->name = isset($data['name']) ? $data['name'] : null;
        $this->id_groups = isset($data['id_groups']) ? $data['id_groups'] : null;
        $this->ord = isset($data['ord']) ? $data['ord'] : null;
        $this->is_switcher = isset($data['is_switcher']) ? $data['is_switcher'] : null;

    }

    public function execute() {
        $scenario = Scenario::factory($this->id_scenarios);
        $scenario->execute($this->data, $this->is_switcher);
    }

    public function status() {
        $scenario = Scenario::factory($this->id_scenarios);
        return $scenario->status($this->data);
    }

    /**
     * Save device from form
     *
     * @param $values
     *
     * @return Action
     */
    public function save($values) {
        $valid = Validation::factory($values);
        $valid->rules('user', Rules::instance()->digit_not_empty)
            ->rules('name', Rules::instance()->not_empty)
            ->rules('scenario', Rules::instance()->digit_not_empty)
            ->rules('group', Rules::instance()->digit_not_empty)
            ->rules('ord', Rules::instance()->digit_not_empty)
            ->rules('switcher', Rules::instance()->digit_not_empty)
            ->check();
        Message::instance($valid->errors());

        if (!Message::instance()->isempty()) {
            return false;
        }

        $this->name = $values['name'];
        $this->id_users = $values['user'];
        $this->id_scenarios = $values['scenario'];
        $this->id_groups = $values['group'];
        $this->is_deleted = 0;
        $this->ord = $values['ord'];
        $this->is_switcher = $values['switcher'];

        //todo добавить валидацию параметров сценария
        $scenario = Scenario::factory($this->id_scenarios);
        $class = $scenario->getClass();
        $this->data = array();
        foreach ($class->fields as $key => $field) {
            $this->data[$key] = isset($values['param_' . $key]) ? $values['param_' . $key] : null;
        }

        $this->_save();
        return $this;
    }

    /**
     * Save data to DB
     */
    protected function _save() {

        if ($this->id_actions) {
            Database::instance()->prepare('UPDATE actions SET name=:name, id_users=:user, id_scenarios=:scenario, data=:data, is_deleted=:is_deleted,
                                          id_groups=:id_groups, ord=:ord, is_switcher=:is_switcher
                                           WHERE id_actions=:id')
                ->bindValue(':user', $this->id_users, PDO::PARAM_INT)
                ->bindValue(':scenario', $this->id_scenarios, PDO::PARAM_INT)
                ->bindValue(':is_deleted', $this->is_deleted, PDO::PARAM_INT)
                ->bindValue(':data', json_encode($this->data))
                ->bindValue(':name', $this->name)
                ->bindValue(':id', $this->id_actions, PDO::PARAM_INT)
                ->bindValue(':id_groups', $this->id_groups, PDO::PARAM_INT)
                ->bindValue(':ord', $this->ord, PDO::PARAM_INT)
                ->bindValue(':is_switcher', $this->is_switcher, PDO::PARAM_INT)
                ->execute();
        } else {
            $id = Database::instance()->prepare('INSERT INTO actions(name, id_users, id_scenarios, data, is_deleted, id_groups, ord, is_switcher)
                                                 VALUES(:name, :user, :scenario, :data, :is_deleted, :id_groups, :ord, :is_switcher)')
                ->bindValue(':user', $this->id_users, PDO::PARAM_INT)
                ->bindValue(':scenario', $this->id_scenarios, PDO::PARAM_INT)
                ->bindValue(':is_deleted', $this->is_deleted, PDO::PARAM_INT)
                ->bindValue(':name', $this->name)
                ->bindValue(':data', json_encode($this->data))
                ->bindValue(':id_groups', $this->id_groups, PDO::PARAM_INT)
                ->bindValue(':ord', $this->ord, PDO::PARAM_INT)
                ->bindValue(':is_switcher', $this->is_switcher, PDO::PARAM_INT)
                ->execute()
                ->lastInsertId();
            $this->id_actions = $id;
        }
    }


    /**
     * Factory method
     *
     * @param array|null $data Single port from DB or array of ports or id of device or device name
     *
     * @return Action
     * @throws Dobby_Exception_404
     */

    public static function factory($data = null) {

        if (is_numeric($data)) {
            return new Action(self::getById($data));
        }
        return new Action(null);
    }

    /**
     * Read action by d
     *
     * @param $id
     *
     * @return array
     */
    public static function getById($id) {
        return Database::instance()->prepare('SELECT * FROM actions WHERE id_actions=:id')
            ->bindValue(':id', $id)
            ->execute()
            ->fetch();
    }


    public static function getActionsAsArray($group = null) {
        if (is_null($group)) {
            return Database::instance()->prepare('SELECT * FROM actions ORDER BY ord')
                ->execute()->fetchAll();
        } else {
            return Database::instance()->prepare('SELECT * FROM actions WHERE id_groups=:group ORDER BY ord')
                ->bindValue(':group', $group)
                ->execute()->fetchAll();
        }
    }
}