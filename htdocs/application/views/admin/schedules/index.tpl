{extends "admin/pages/index.tpl"}
{block "content"}
    <h1>Расписание</h1>
    <a href="/admin/schedules/add">Добавить новое</a>
    <table class="table table-bordered">
        <thead>
        <tr>
            <th>Идентификатор</th>
            <th>Профиль</th>
            <th>Дата</th>
            <th>Когда происходит</th>
            <th>Время старта</th>
            <th>Время остановки</th>
        </tr>
        </thead>
        <tbody>
        {foreach $schedules as $schedule}
            <tr>
                <td>{$schedule.id_schedules}</td>
                <td>{$schedule.profile_name}</td>
                <td>{$schedule.date}</td>
                <td>{$schedule.occur}</td>
                <td>{$schedule.timefrom}</td>
                <td>{$schedule.timeto}</td>
                <td><a href="/admin/schedules/{$schedule.id_schedules}">Ред</a></td>
            </tr>
        {/foreach}
        </tbody>
    </table>
{/block}