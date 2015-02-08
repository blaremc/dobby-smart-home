{extends "admin/pages/index.tpl"}
{block "content"}
    {if $item.id_groups}
        <h1>Редактирование группу</h1>
    {else}
        <h1>Добавить группу</h1>
    {/if}
    {if $item.id_groups}
        <form role="form" action="#" class="app-edit-group">
    {else}
        <form role="form" action="#" class="app-add-group">
    {/if}

    <div class="form-group">
        <label for="name">Название</label>
        <input id="name" name="name" class="required iname form-control"
               placeholder="Название" value="{$item.name}">
    </div>

    <div class="form-group">
        <label for="ord">Порядок вывода</label>
        <input id="ord" name="ord" class="required iord form-control"
               placeholder="Порядок вывода" value="{$item.ord}">
    </div>
    <button type="submit" class="btn btn-default">Сохранить</button>
    {if $item.id_groups}
        <button type="button" class="btn btn-danger js-delete-group">Удалить</button>
    {/if}

    <input type="hidden" name="id" class="js-id" value="{$item.id_groups}">
    </form>
{/block}