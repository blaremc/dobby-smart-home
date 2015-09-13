<?php defined('SYSPATH') or die('No direct script access.');


class Module {

    private $name = 'sample';

    private $_module;

    /**
     * @var Module
     */
    protected static $_instance = array();

    public function __construct($name) {

        $this->name = $name;
        $this->_module = Database::instance()->prepare('SELECT id_modules, data FROM modules WHERE name=:name')->bindValue(':name', $this->name)->execute()->fetch();
        if (empty($this->_module['id_modules'])) {
            $this->_module['id_modules'] = Database::instance()->prepare('INSERT INTO modules(name, data, create_date, last_date) VALUES(:name, null, NOW(), NOW())')->bindValue(':name', $this->name)->execute()->lastInsertId();
        }
        if (empty($this->_module['data'])) {
            $this->_module['data'] = array('values' => array(), 'increment' => 1);
        } else {
            $this->_module['data'] = json_decode($this->_module['data'], true);
        }
    }

    /**
     * @param $name
     * @return Module
     */
    public static function instance($name) {

        if (empty(self::$_instance[$name])) {
            self::$_instance[$name] = new Module($name);
        }
        return self::$_instance[$name];
    }


    /**
     * @param $module Dobby_Module
     * @param $values
     */
    public function saveRecord($module, $values = null) {

        if ($module->id == null) {
            $module->id = ++$this->_module['data']['increment'];
        }
        if ($values == null) {
            $values = $module->syncParams;
        }

        $row = array();
        foreach ($values as $value) {
            if (property_exists($module, $value)) {
                $row[$value] = $module->$value;
            }
        }
        $row['id'] = $module->id;
        $this->_appendRow($row);
        $this->_save();
    }


    /**
     * Get record
     * @param $id
     * @return null
     */
    public function getRecord($id) {

        foreach ($this->_module['data']['values'] as $values) {
            if ($values['id'] == $id) {
                return $values;
            }
        }
        return null;
    }

    /**
     * Save module to DB
     */
    private function _save() {

        Database::instance()->prepare('UPDATE modules SET data=:data, last_date = NOW() WHERE id_modules=:id')->bindValue(':id', $this->_module['id_modules'])->bindValue(':data', json_encode($this->_module['data']))->execute();
    }

    /**
     * Append row to values
     *
     * @param $row
     */
    protected function _appendRow($row) {

        $find = false;
        foreach ($this->_module['data']['values'] as &$value) {
            if ($value['id'] == $row['id']) {
                $value = $row;
                $find = true;
                break;
            }
        }
        if (!$find) {
            $this->_module['data']['values'][] = $row;
        }
    }


    /**
     * Return values
     *
     * @return array
     */
    public function getValues() {

        return $this->_module['data']['values'];
    }


    public static function compileModuleJS() {

        $jsSource = array();
        $contents = array();
        foreach (Dobby::$modules as $module) {
            $jsSource = array_merge($jsSource, self::getFiles(MODPATH_DOBBY, $module['id'] . '/js/', 'js'));
        }
        foreach ($jsSource as $file) {
            $contents[] = file_get_contents(MODPATH_DOBBY . $file);
        }
        $out = implode("\n", $contents);
        file_put_contents(DOCROOT . '/assets/js/modules.js', $out);
    }

    public static function compileModuleCSS() {
        $cssSource = array();
        $contents = array();
        foreach (Dobby::$modules as $module) {
            $cssSource = array_merge($cssSource, self::getFiles(MODPATH_DOBBY, $module['id'] . '/css/', 'css'));
        }
        foreach ($cssSource as $file) {
            $contents[] = file_get_contents(MODPATH_DOBBY . $file);
        }
        $out = implode("\n", $contents);
        file_put_contents(DOCROOT . '/assets/css/modules.css', $out);
    }

    public static function copyImg() {
        $destDir = DOCROOT . '/assets/img/';
        foreach (Dobby::$modules as $module) {
            $files =  self::getFiles(MODPATH_DOBBY . '/' . $module['id'] . '/img/', '', '*');
            foreach ($files as $file) {
                copy(MODPATH_DOBBY . '/' . $module['id'] . '/img/' . $file, $destDir . $file);
            }
        }

    }

    public static function getFiles($root, $path, $ext) {

        $files = array();

        $root = rtrim($root, DIRECTORY_SEPARATOR);
        $path = trim($path, DIRECTORY_SEPARATOR);

        if (!is_dir($root . DIRECTORY_SEPARATOR . $path)) {
            return $files;
        }


        if (file_exists($root . DIRECTORY_SEPARATOR . $path . DIRECTORY_SEPARATOR . 'init.js')) {
            $files[] = $path . DIRECTORY_SEPARATOR . 'init.js';
        }

        $dir = new DirectoryIterator($root . DIRECTORY_SEPARATOR . $path);


        foreach ($dir as $file) {
            $filename = $file->getFilename();

            if (($filename == '.') || ($filename == '..') || ($filename[0] == '.') || $filename == 'init.js') {
                continue;
            }

            if ($file->isDir()) {
                $files = array_merge($files, self::getFiles($root, $path . DIRECTORY_SEPARATOR . $filename, $ext));
                continue;
            }
            if ($ext != '*' && substr($filename, -strlen($ext) - 1) != '.' . $ext) {
                continue;
            } else if ($path) {
                $filename = $path . DIRECTORY_SEPARATOR . $filename;
            }

            $files[] = str_replace('//', '/', str_replace('\\', "/", $filename));
        }
        return $files;
    }
}