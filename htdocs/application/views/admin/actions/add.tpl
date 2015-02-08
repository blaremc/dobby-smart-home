{extends "admin/pages/index.tpl"}
{block "content"}
    <h1>Добавление действия</h1>
    <form role="form" action="#" class="app-add-actions">
        <div class="form-group">
            <label for="name">Название действия</label>
            <input id="name" name="name" class="form-control required iname" placeholder="Введите название действия">
        </div>
        <div class="form-group">
            <label for="user">Кто видит у себя действие</label>
            <select class="form-control required iuser" name="user">
                <option value="0">Все</option>
                {foreach $users as $user}
                    <option value="{$user.id_users}">{$user.name}</option>
                {/foreach}
            </select>
        </div>
        <div class="form-group">
            <label for="group">Группа</label>
            <select class="form-control required igroup app-action-group" name="group" id="group">
                {foreach $groups as $group}
                    <option value="{$group.id_groups}">{$group.name}</option>
                {/foreach}
            </select>
        </div>
        <div class="form-group">
            <label for="ord">Переключатель</label>
            <select class="form-control required iswitcher" name="switcher">
                <option value="0">Нет</option>
                <option value="1">Да</option>
            </select>
        </div>
        <div class="form-group">
            <label for="ord">Порядок вывода</label>
            <input id="ord" name="ord" class="required iord form-control"
                   placeholder="Порядок вывода" value="{$item.ord}">
        </div>
        <div class="form-group">
            <label for="scenario">Какой сценарий будет выполняться</label>
            <select class="form-control required iscenario app-action-scenario" name="scenario" id="scenario">
                {foreach $scenarios as $scenario}
                    <option value="{$scenario.id_scenarios}">{$scenario.name}</option>
                {/foreach}
            </select>
        </div>
        <div class="js-scenario-params"></div>

        <button type="submit" class="btn btn-default">Добавить действие</button>
    </form>
    {foreach $scenarios as $scenario}
        <div class="app-scenario app-scenario-{$scenario.id_scenarios}" style="display: none;">
            <div class="well">
                {foreach $scenario.params as $key => $param}
                    {include "admin/helpers/params.tpl" param="$param" name="$key"}
                {/foreach}
            </div>
        </div>
    {/foreach}
{/block}