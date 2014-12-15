{if $param.type == 'int'}
    <div class="form-group">
        <label for="{$name}">{$param.caption}</label>
        <input class="form-control" name="param_{$name}" id="param_{$name}" placeholder="Укажите целочисленное значение" value="{$value}">
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