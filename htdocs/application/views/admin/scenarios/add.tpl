{extends "admin/pages/index.tpl"}
{block "content"}
    <h1>Добавление сценария</h1>
    <form role="form" action="#" class="app-add-scenario">
        <div class="form-group">
            <label for="name">Название файла</label>
            <input id="name" name="name" class="form-control required iname app-filename-input" placeholder="Введите название файла"
                   value="Sample">
        </div>

        <div class="form-group">
            <label for="code">Код сценария</label>

            <div>
                <code>
                    <?php defined('SYSPATH') or die('No direct script access.');<br>
                    class Scenario_<span class="app-filename">Sample</span> extends Dobby_Scenario {
                </code>
                <textarea id="code" name="code" class="form-control required icode" rows="10">
public function init(){

}

public function execute() {

}
                </textarea>
                <code>
                    }
                </code>
            </div>
        </div>
        <div class="checkbox">
            <label>
                <input type="checkbox" id="is_active" name="is_active" checked>
                Работает
            </label>
        </div>
        <button type="submit" class="btn btn-default">Добавить сценарий</button>
    </form>
{/block}