{if $param.type == 'int'}
    <div class="form-group">
        <label for="{$name}">{$param.caption}</label>
        <input class="form-control" name="param_{$name}" id="param_{$name}" placeholder="Укажите целочисленное значение" value="{$value}">
    </div>
{elseif $param.type == 'color'}
    <div class="form-group">

        <div class="checkbox">
            <label>
                <input type="checkbox" id="param_{$name}-ch" name="param_{$name}-ch" {if $value == null}checked="checked" {/if} class="js-color-clear">
                Пусто
            </label>
        </div>
        <label for="{$name}">{$param.caption}</label>
        <input class="form-control js-color" name="param_{$name}" id="param_{$name}" placeholder="Выберите цвет" value="{$value}">
    </div>
{elseif $param.type == 'bool'}
    <div class="form-group">
        <label for="{$name}">{$param.caption}</label>
        <select class="form-control" name="param_{$name}" id="param_{$name}">
            <option value="-1" {if $value == -1}selected="selected" {/if}>Пусто</option>
            <option value="1" {if $value == 1}selected="selected" {/if}>Да</option>
            <option value="0" {if $value == 0}selected="selected" {/if}>Нет</option>
        </select>
    </div>
{else}
    <div class="form-group">
        <label for="{$name}">{$param.caption}</label>
        <select class="form-control" name="param_{$name}" id="param_{$name}">
            {foreach $param.type as $item}
                <option value="{$item}" {if $value == $item}selected="selected" {/if}>{$item}</option>
            {/foreach}
        </select>
    </div>
{/if}