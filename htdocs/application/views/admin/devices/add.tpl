{extends "admin/pages/index.tpl"}
{block "content"}
    <h1>Добавление устройства</h1>
    <form role="form" action="#" class="app-add-device">
        <div class="form-group">
            <label for="name">Название устройства</label>
            <input id="name" name="name" class="form-control required iname" placeholder="Введите название устройства">
        </div>
        <div class="form-group">
            <label for="address">Адрес устройства</label>
            <input id="address" name="address" class="required iaddress form-control"
                   placeholder="Дата">
        </div>
        <div class="form-group">
            <label for="driver">Драйвер</label>
            <select class="form-controlrequired idriver " id="driver" name="driver">
                {foreach $drivers as $driver}
                    <option value="{$driver}">{$driver}</option>
                {/foreach}
            </select>
        </div>
        <div class="checkbox">
            <label>
                <input type="checkbox" id="is_active" name="is_active" checked>
                Опрашивать
            </label>
        </div>
        <button type="submit" class="btn btn-default">Добавить устройство</button>
    </form>
{/block}