{extends "admin/pages/index.tpl"}
{block "content"}
    <h1>Группы</h1>
    <a href="/admin/groups/add">Добавить новую</a>
    <table class="table table-bordered">
        <thead>
        <tr>
            <th>Идентификатор</th>
            <th>Название</th>
            <th>Порядок</th>
        </tr>
        </thead>
        <tbody>
        {foreach $items as $item}
            <tr>
                <td>{$item.id_groups}</td>
                <td>{$item.name}</td>
                <td>{$item.ord}</td>
                <td><a href="/admin/groups/{$item.id_groups}">Ред</a></td>
            </tr>
        {/foreach}
        </tbody>
    </table>
{/block}