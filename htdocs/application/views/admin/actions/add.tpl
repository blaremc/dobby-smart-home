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
            <label for="scenario">Какой сценарий будет выполняться</label>
            <select class="form-control required iscenario app-action-scenario" name="scenario" id="scenario">
                {foreach $scenarios as $scenario}
                    <option value="{$scenario.id_scenarios}">{$scenario.name}</option>
                {/foreach}
            </select>
        </div>
        {foreach $scenarios as $scenario}
            <div class="app-scenario app-scenario-{$scenario.id_scenarios}" style="display: none;">
                <div class="well">
                {foreach $scenario.params as $key => $param}
                    {include "admin/helpers/params.tpl" param="$param" name="$key"}
                {/foreach}
                </div>
            </div>
        {/foreach}
        <button type="submit" class="btn btn-default">Добавить действие</button>
    </form>
{/block}