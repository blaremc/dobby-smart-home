{extends "admin/pages/index.tpl"}
{block "content"}
<h1>Устройства</h1>
    <a href="/admin/devices/add">Добавить новое</a>
    <table class="table table-bordered">
        <thead>
        <tr>
            <th>Имя</th>
            <th>Адрес</th>
            <th>Драйвер</th>
            <th>Значение</th>
            <th>Время проверки</th>
            <th>Опрашивать</th>
            <th>Ошибка</th>
        </tr>
        </thead>
        <tbody>
    {foreach $devices as $device}
        <tr>
            <td>{$device.name}</td>
            <td>{$device.address}</td>
            <td>{$device.driver}</td>
            <td>{$device.last_value}</td>
            <td>{$device.execute_date}</td>
            <td>{if $device.is_active}Да{else}Нет{/if}</td>
            <td>{$device.error}</td>
            <td><a href="/admin/devices/{$device.id_devices}">Ред</a> </td>
        </tr>
    {/foreach}
        </tbody>
    </table>
{/block}