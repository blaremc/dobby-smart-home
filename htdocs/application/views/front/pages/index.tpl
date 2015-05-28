{extends "front/template/index.tpl"}
{block "container"}
    <div class="container well">
        <div class="row">
            <div class="col-md-2 menubar">
                <div class="logo"><a href="/">Dobby</a></div>
                {include "front/helpers/menu.tpl"}
            </div>
            <div class="col-md-10" id="content">
                {block "content"}
                {/block}
            </div>
        </div>
    </div>
{/block}