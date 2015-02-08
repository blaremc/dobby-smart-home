{extends "admin/pages/index.tpl"}
{block "content"}
    <h1>Редактирование действия</h1>
    <form role="form" action="#" class="app-edit-action">
        <div class="form-group">
            <label for="name">Название дейтсвия</label>
            <input id="name" name="name" class="form-control required iname" placeholder="Введите название устройства"
                   value="{$action.name}">
        </div>
        <div class="form-group">
            <label for="user">Кто видит у себя действие</label>
            <select class="form-control required iuser" name="user">
                <option value="0">Все</option>
                {foreach $users as $user}
                    <option value="{$user.id_users}" {if $action.id_users == $user.id_users}selected="selected" {/if}>{$user.name}</option>
                {/foreach}
            </select>
        </div>
        <div class="form-group">
            <label for="group">Группа</label>
            <select class="form-control required igroup app-action-group" name="group" id="group">
                {foreach $groups as $group}
                    <option value="{$group.id_groups}"
                            {if $group.id_groups==$action.id_groups}selected="selected" {/if}>{$group.name}</option>
                {/foreach}
            </select>
        </div>
        <div class="form-group">
            <label for="ord">Переключатель</label>
            <select class="form-control required iswitcher" name="switcher">

                <option value="0" {if $action.is_switcher == 0}selected="selected" {/if}>Нет</option>
                <option value="1" {if $action.is_switcher == 1}selected="selected" {/if}>Да</option>
            </select>
        </div>
        <div class="form-group">
            <label for="ord">Порядок вывода</label>
            <input id="ord" name="ord" class="required iord form-control"
                   placeholder="Порядок вывода" value="{$action.ord}">
        </div>
        <div class="form-group">
            <label for="scenario">Какой сценарий будет выполняться</label>
            <select class="form-control required iscenario app-action-scenario" name="scenario" id="scenario">
                {foreach $scenarios as $scenario}
                    <option value="{$scenario.id_scenarios}"
                            {if $action.id_scenarios == $scenario.id_scenarios}selected="selected" {/if}>{$scenario.name}</option>
                {/foreach}
            </select>
        </div>
        <div class="js-scenario-params"></div>
        <button type="submit" class="btn btn-default">Сохранить</button>
        <input type="hidden" name="id" value="{$action.id_actions}">
    </form>
    {foreach $scenarios as $scenario}
        <div class="app-scenario app-scenario-{$scenario.id_scenarios}" style="display: none;">
            <div class="well">
                {foreach $scenario.params as $key => $param}
                    {include "admin/helpers/params.tpl" param="$param" name="$key" value="{$action.data[$key]}"}
                {/foreach}
            </div>
        </div>
    {/foreach}
{/block}