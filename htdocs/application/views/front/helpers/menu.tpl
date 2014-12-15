<ul class="menu">
    <li><a href="/devices/" class="app-link-devices">Устройства</a></li>
    <li><a href="/scenarios/" class="app-link-scenarios">Сценарии</a></li>
    <li><a href="/actions/" class="app-link-actions">Действия</a></li>
    {foreach $modules as $module}
        <li><a href="/{$module.id}">{$module.name}</a></li>
    {/foreach}
</ul>