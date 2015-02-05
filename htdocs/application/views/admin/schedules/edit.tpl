{extends "admin/pages/index.tpl"}
{block "content"}
    {if $item.id_schedules}
        <h1>Редактирование расписание</h1>
    {else}
        <h1>Добавить расписание</h1>
    {/if}
    {if $item.id_schedules}
        <form role="form" action="#" class="app-edit-schedule">
    {else}
        <form role="form" action="#" class="app-add-schedule">
    {/if}
    <div class="form-group">
        <label for="name">Профиль</label>
        <select class="form-control required iprofile " id="profile" name="profile">
            {foreach $profiles as $profile}
                <option value="{$profile.id_profiles}"
                        {if $profile.id_profiles==$item.id_profiles}selected="selected"{/if}>{$profile.name}</option>
            {/foreach}
        </select>
    </div>
    <div class="form-group">
        <label for="date">Дата</label>
        <input id="date" name="date" class="required idate form-control js-datepicker"
               placeholder="Дата" value="{$item.date}">
    </div>
    <div class="form-group">
        <label for="name">Когда происходит</label>
        <select class="form-control required ioccur" id="occur" name="occur">
            {foreach $occur as $oc => $val}
                <option value="{$oc}" {if $oc==$item.occur}selected="selected"{/if}>{$val}</option>
            {/foreach}
        </select>
    </div>
    <div class="form-group">
        <label for="timefrom">Время с</label>
        <input id="timefrom" name="timefrom" class="required itimefrom input-small js-timepicker"
               placeholder="Время с 00:00" value="{$item.timefrom}">
    </div>
    <div class="form-group">
        <label for="timeto">Время по</label>
        <input id="timeto" name="timeto" class="required itimeto input-small js-timepicker"
               placeholder="Время по 00:00" value="{$item.timeto}">
    </div>
    <button type="submit" class="btn btn-default">Сохранить</button>
    {if $item.id_schedules}
        <button type="button" class="btn btn-danger js-delete-schedule">Удалить</button>
    {/if}

    <input type="hidden" name="id" class="js-id" value="{$item.id_schedules}">
    </form>
{/block}