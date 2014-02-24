{extends "admin/pages/index.tpl"}
{block "content"}
    <h1>Редактирование устройства</h1>
    <form role="form" action="#" class="app-edit-device">
        <div class="form-group">
            <label for="name">Название устройства</label>
            <input id="name" name="name" class="form-control required iname" placeholder="Введите название устройства"
                   value="{$device.name}">
        </div>
        <div class="form-group">
            <label for="address">Адрес устройства</label>
            <input id="address" name="address" class="required iaddress form-control"
                   placeholder="Введите адрес устройства" value="{$device.address}">
        </div>
        <div class="form-group">
            <label for="driver">Драйвер</label>
            <select class="form-controlrequired idriver " id="driver" name="driver">
                {foreach $drivers as $driver}
                    <option value="{$driver}" {if $device.driver==$driver}selected="selected"{/if}>{$driver}</option>
                {/foreach}
            </select>
        </div>
        <div class="checkbox">
            <label>
                <input type="checkbox" id="is_active" name="is_active" {if $device.is_active}checked="checked" {/if}>
                Опрашивать
            </label>
        </div>
        <button type="submit" class="btn btn-default">Сохранить</button>
        <input type="hidden" name="id" value="{$device.id_devices}">
    </form>
{/block}