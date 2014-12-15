<?php defined('SYSPATH') or die('No direct script access.');


class Controller_Smarty extends Controller {

    public $user = null;

    /**
     * @var  Smarty_View  page template
     */
    public $view;


    public $theme = '';


    public $template = 'template';


    /**
     * @var  boolean  auto render template
     **/
    public $auto_render = true;

    /**
     * Loads the template [Smarty_View] object.
     */
    public function before() {
        parent::before();

        if ($this->auto_render === true) {
            // Load the template
            $this->view = Smarty_View::factory();
        }
        $this->view->set('modules', Dobby::$modules);
    }

    /**
     * Assigns the template [Smarty_View] as the request response.
     */
    public function after() {
        if ($this->auto_render === true) {

            $t = Profiler::start('index', 'view');
            $this->response->body($this->view->render($this->template . '.tpl'));
            Profiler::stop($t);
        }

        parent::after();
        Module::compileModuleJS();
        Module::compileModuleCSS();
    }


}