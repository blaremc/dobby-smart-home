{extends "front/pages/index.tpl"}
{block "content"}
    <div class="actions-block">
        {foreach $groups as $group}
            <div class="action">
                <a href="/actions/{$group.id_groups} data-id="{$group.id_groups}">{$group.name}<span class="count">{$group.count}</span>
                </a>
            </div>
        {/foreach}
    </div>
{/block}