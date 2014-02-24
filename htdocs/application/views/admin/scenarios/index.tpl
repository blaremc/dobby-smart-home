{extends "admin/pages/index.tpl"}
{block "content"}
<h1>Сценарии</h1>
    <a href="/admin/scenarios/add">Добавить новый</a>
    <table class="table table-bordered">
        <thead>
        <tr>
            <th>Имя файла</th>
            <th>Время проверки</th>
            <th>Работает</th>
            <th>Ошибка</th>
        </tr>
        </thead>
        <tbody>
    {foreach $scenarios as $scenario}
        <tr>
            <td>{$device.filename}</td>
            <td>{$device.execute_date}</td>
            <td>{if $device.is_active}Да{else}Нет{/if}</td>
            <td>{$device.error}</td>
            <td><a href="/admin/scenarios/{$device.id_scenarios}">Редактировать</a> </td>
        </tr>
    {/foreach}
        </tbody>
    </table>
{/block}