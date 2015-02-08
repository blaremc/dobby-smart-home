<ul>
    <li><a href="/admin/devices">Устройства</a></li>
    <li><a href="/admin/scenarios">Сценарии</a></li>
    <li><a href="/admin/actions">Действия</a></li>
    <li><a href="/admin/schedules">Расписание профилей</a></li>
    <li><a href="/admin/groups">Группы</a></li>
    {foreach $modules as $module}
        <li><a href="/admin/{$module.id}">{$module.name}</a></li>
    {/foreach}
</ul>

<div>Текущий профиль: <a href="/admin/schedules/{$profile.id_schedules}">{$profile.name}</a></div>
