{extends "admin/pages/index.tpl"}
{block "content"}
<h1>Действия</h1>
    <a href="/admin/actions/add">Добавить новое</a>
    <table class="table table-bordered">
        <thead>
        <tr>
            <th>Имя</th>
            <th>Пользователь</th>
            <th>Сценарий</th>
            <th>Удален</th>
        </tr>
        </thead>
        <tbody>
    {foreach $actions as $action}
        <tr>
            <td>{$action.name}</td>
            <td>{$action.user}</td>
            <td>{$action.scenario}</td>
            <td>{if $action.is_deleted}Да{else}Нет{/if}</td>
            <td><a href="/admin/actions/{$action.id_actions}">Редактировать</a> </td>
        </tr>
    {/foreach}
        </tbody>
    </table>
{/block}