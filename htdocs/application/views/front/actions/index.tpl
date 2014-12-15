{extends "front/pages/index.tpl"}
{block "content"}
    <div class="actions-block">
        {foreach $actions as $action}
            <div class="action">
                <a href="#" class="app-start-action" data-id="{$action.id_actions}">{$action.name}</a>
            </div>
        {/foreach}
    </div>
{/block}