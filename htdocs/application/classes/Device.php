<?php defined('SYSPATH') or die('No direct script access.');


class Device {


    public $id_devices = null;
    public $address = null;
    public $driver = null;
    public $last_value = null;
    public $create_date = null;
    public $last_date = null;
    public $is_active = null;
    public $execute_date = null;
    public $error = null;
    public $name = null;
    public $is_changed = false;

    protected static $_devices = array();

    public function __construct($data) {

        $data = is_array($data) ? $data : array();
        $this->id_devices = isset($data['id_devices']) ? $data['id_devices'] : null;
        $this->address = isset($data['address']) ? $data['address'] : null;
        $this->driver = isset($data['driver']) ? $data['driver'] : null;
        $this->last_value = isset($data['last_value']) ? $data['last_value'] : null;
        $this->create_date = isset($data['create_date']) ? $data['create_date'] : null;
        $this->last_date = isset($data['last_date']) ? $data['last_date'] : null;
        $this->is_active = isset($data['is_active']) ? $data['is_active'] : null;
        $this->name = isset($data['name']) ? $data['name'] : null;
        $this->error = isset($data['error']) ? $data['error'] : null;
        $this->execute_date = isset($data['execute_date']) ? $data['execute_date'] : null;
    }


    /**
     * Save device from form
     *
     * @param $values
     * @return Device
     */
    public function save($values) {
        $valid = Validation::factory($values);
        $valid->rules('address', Rules::instance()->not_empty)
            ->rules('name', Rules::instance()->engname)
            ->rules('driver', Rules::instance()->not_empty)
            ->check();
        Message::instance($valid->errors());

        if (!Message::instance()->isempty()) return false;

        $this->name = $values['name'];
        $this->driver = $values['driver'];
        $this->address = $values['address'];
        $this->is_active = isset($values['is_active']) ? 1 : 0;

        $this->_save();
        return $this;
    }

    /**
     * Save data to DB
     */
    protected function _save() {

        if ($this->id_devices) {
            Database::instance()->prepare('UPDATE devices SET name=:name, driver=:driver, address=:address, is_active=:is_active, last_value=:value,
                                            last_date=NOW(), execute_date = :execute_date, error = :error
                                           WHERE id_devices=:id')
                ->bindValue(':driver', $this->driver)
                ->bindValue(':address', $this->address)
                ->bindValue(':is_active', $this->is_active)
                ->bindValue(':name', $this->name)
                ->bindValue(':id', $this->id_devices)
                ->bindValue(':value', $this->last_value)
                ->bindValue(':execute_date', $this->execute_date)
                ->bindValue(':error', $this->error)
                ->execute();
        } else {
            $id = Database::instance()->prepare('INSERT INTO devices(name, driver, address, is_active, create_date)  VALUES(:name, :driver, :address, :is_active, NOW())')
                ->bindValue(':driver', $this->driver)
                ->bindValue(':address', $this->address)
                ->bindValue(':is_active', $this->is_active)
                ->bindValue(':name', $this->name)
                ->execute()
                ->lastInsertId();
            $this->id_devices = $id;
        }
    }

    /**
     * @return mixed
     */
    public function getValue() {
        try {
            return Dobby_Driver::factory($this->driver)->getValue($this->address);
        } catch (Exception $e) {

        }
    }

    /**
     * Set value
     *
     * @param mixed $value
     */
    public function setValue($value) {
        try {
            Dobby_Driver::factory($this->driver)->setValue($this->address, $value);
        } catch (Exception $e) {

        }
    }


    /**
     * Set last value
     *
     * @param $value
     */
    public function setLastValue($value) {
        $value = $value=='' ? $this->last_value : $value;
        $this->error = null;
        $this->is_changed = $value != $this->last_value;
        $this->last_value = $value;
        $this->execute_date = date('Y-m-d H:i:s');
        $this->saveValue();
        Database::instance()->prepare('UPDATE devices SET last_value=:value,
                                            last_date=NOW(), execute_date = :execute_date
                                           WHERE id_devices=:id')
            ->bindValue(':id', $this->id_devices)
            ->bindValue(':value', $this->last_value)
            ->bindValue(':execute_date', $this->execute_date)
            ->execute();
    }

    /**
     * Save value log
     */
    protected function saveValue() {
        Database::instance()->prepare('INSERT INTO device_values(id_devices, value,time) VALUES(:id, :value, NOW())')
            ->bindValue(':id', $this->id_devices)
            ->bindValue(':value', $this->last_value)
            ->execute();
    }

    /**
     * Set error
     *
     * @param string $error
     */
    public function setError($error) {
        $this->is_changed = false;
        $this->error = iconv('windows-1251', 'utf-8', $error);
        $this->execute_date = date('Y-m-d H:i:s');
        $this->_save();
    }

    /**
     * Factory method
     *
     * @param array|null $value Single port from DB or array of ports or id of device or device name
     * @return Device
     * @throws Dobby_Exception_404
     */

    public static function factory($value = null) {

        if (is_array($value) && isset($value['id_devices'])) { // If one row from DB

            $device = self::getCache($value['id_devices']);
            if (!$device) {
                self::setCache(new Device($value));
            }
            return self::$_devices[$value['id_devices']];

        } elseif (is_array($value)) { // Some rows from DB

            foreach ($value as &$item) {
                $item = Device::factory($item);
            }
            return $value;

        } elseif (is_numeric($value)) { // For ID Device

            $device = self::getCache($value);
            if (!$device) {
                $device = self::getDeviceById($value);
                if (!$device) {
                    throw new Dobby_Exception_404();
                }
                self::setCache(new Device($device));
            }
            return self::$_devices[$value];

        } elseif (is_string($value)) {
            $device = self::getCache($value);
            if (!$device) {
                $device = self::getDeviceByName($value);
                if (!$device) {
                    throw new Dobby_Exception_404();
                }
                self::setCache(new Device($device));
            }
            return self::$_devices[$value];
        }

        return new Device(null);
    }


    /**
     * Set cache of device
     *
     * @param Device $device
     */
    protected static function setCache($device) {
        self::$_devices[$device->id_devices] = $device;
        self::$_devices[$device->name] = $device;
    }

    /**
     * Get cached device
     *
     * @param $key
     * @return null|Device
     */
    protected static function getCache($key) {

        return isset(self::$_devices[$key]) ? self::$_devices[$key] : null;
    }

    /**
     * Get device by id
     *
     * @param $id
     * @return array
     */
    public static function getDeviceById($id) {

        return Database::instance()->prepare('SELECT * FROM devices WHERE id_devices=:id')
            ->bindValue(':id', $id)
            ->execute()
            ->fetch();
    }


    /**
     * Ged device by name
     *
     * @param $name
     * @return array
     */
    public static function getDeviceByName($name) {
        return Database::instance()->prepare('SELECT * FROM devices WHERE name=:name')
            ->bindValue(':name', $name)
            ->execute()
            ->fetch();
    }

    /**
     * Ged device by address
     *
     * @param $address
     * @return array
     * @throws Kohana_Exception
     */
    public static function getDeviceByAddress($address) {
        return Database::instance()->prepare('SELECT * FROM devices WHERE address=:address')
            ->bindValue(':address', $address)
            ->execute()
            ->fetch();
    }

    /**
     * Get all active ports from DB
     *
     * @return Device[]
     */
    public static function getDevices() {

        return Device::factory(self::getDevicesAsArray());
    }

    public static function getDevicesAsArray() {
        return Database::instance()->prepare('SELECT * FROM devices')->execute()->fetchAll();
    }
}