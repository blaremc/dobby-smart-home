{extends "admin/template/index.tpl"}
{block "container"}
    <div class="container well">
        <div class="row">
            <div class="col-md-3">
                {include "admin/helpers/menu.tpl"}
            </div>
            <div class="col-md-9">
                {block "content"}
                {/block}
            </div>
        </div>
    </div>
{/block}