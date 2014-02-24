<?php defined('SYSPATH') or die('No direct script access.');


class Scenario {

    protected $_path = '';

    public $id_scenarios = null;
    public $filename = null;
    public $execute_date = null;
    public $create_date = null;
    public $last_date = null;
    public $is_active = null;
    public $error = null;

    protected static $_devices = array();

    public function __construct($data) {

        $data = is_array($data) ? $data : array();
        $this->id_scenarios = isset($data['id_scenarios']) ? $data['id_scenarios'] : null;
        $this->filename = isset($data['filename']) ? $data['filename'] : null;
        $this->create_date = isset($data['create_date']) ? $data['create_date'] : null;
        $this->last_date = isset($data['last_date']) ? $data['last_date'] : null;
        $this->is_active = isset($data['is_active']) ? $data['is_active'] : null;
        $this->error = isset($data['error']) ? $data['error'] : null;
        $this->execute_date = isset($data['execute_date']) ? $data['execute_date'] : null;

        $this->_path = APPPATH . 'classes/Scenario/';
    }


    /**
     * @param $id
     * @return array
     */
    public static function getScenarioById($id) {

        return Database::instance()->prepare('SELECT * FROM scenarios WHERE id_scenarios=:id')
            ->bindValue(':id', $id)
            ->execute()
            ->fetch();
    }

    /**
     * @param null $value
     * @return null|Scenario
     * @throws Dobby_Exception_404
     */
    public static function factory($value = null) {

        if (is_array($value) && isset($value['id_scenarios'])) { // If one row from DB
            return new Scenario($value);

        } elseif (is_array($value)) { // Some rows from DB

            foreach ($value as &$item) {
                $item = Scenario::factory($item);
            }
            return $value;
        } elseif (is_numeric($value)) {
            $device = self::getScenarioById($value);
            if (!$device) {
                throw new Dobby_Exception_404();
            }
            return new Scenario($device);
        } elseif (is_null($value)) {
            return new Scenario(null);
        }

        return new Scenario($value);
    }

    /**
     * Save scenario from form
     *
     * @param $values
     * @return Scenario
     */
    public function save($values) {
        $valid = Validation::factory($values);
        $valid->rules('code', Rules::instance()->not_empty)
            ->rules('name', Rules::instance()->engname)
            ->check();
        Message::instance($valid->errors());

        if (!Message::instance()->isempty()) return false;

        $this->filename = $values['name'];
        $this->is_active = isset($values['is_active']) ? 1 : 0;

        if ($this->_save()) {

            $this->_saveFile($values['code']);
        }

        return $this;
    }

    public function getClass() {
        $class = 'Scenario_' . ucfirst($this->filename);
        return new $class();
    }


    protected function _saveFile($code) {

        $code = "<?php defined('SYSPATH') or die('No direct script access.'); // Don't touch this line" . PHP_EOL .
            "class Scenario_Sample extends Dobby_Scenario { // Don't touch this line" . PHP_EOL . $code . PHP_EOL . "}// Don't touch this line";

        file_put_contents($this->_path . ucfirst($this->filename) . EXT, $code, 0x777);
    }

    /**
     * Save data to DB
     */
    protected function _save() {

        if ($this->id_scenarios) {
            return Database::instance()->prepare('UPDATE scenarios SET filename=:filename, is_active=:is_active,
                                            last_date=NOW(), execute_date = :execute_date, error = :error
                                           WHERE id_scenarios=:id')
                ->bindValue(':filename', $this->filename)
                ->bindValue(':is_active', $this->is_active)
                ->bindValue(':id', $this->id_scenarios)
                ->bindValue(':execute_date', $this->execute_date)
                ->bindValue(':error', $this->error)
                ->execute();
        } else {
            $id = Database::instance()->prepare('INSERT INTO scenarios(filename, is_active, create_date)  VALUES(:filename, :is_active, NOW())')
                ->bindValue(':is_active', $this->is_active)
                ->bindValue(':filename', $this->filename)
                ->execute()
                ->lastInsertId();
            $this->id_scenarios = $id;
            return $this->id_scenarios;
        }
    }

    /**
     * Get all active ports from DB
     *
     * @return Scenario[]
     */
    public static function getScenarios() {

        return Scenario::factory(self::getScenariosAsArray());
    }

    /**
     * @return array
     */
    public static function getScenariosAsArray() {
        return Database::instance()->prepare('SELECT * FROM scenarios')->execute()->fetchAll();
    }


}