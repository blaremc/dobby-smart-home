{extends "front/template/index.tpl"}
{block "container"}
    <div class="container">
        <h3 class="text-center">{__('welcome login form')}</h3>
        <div class="row">
            <div class="col-md-6 col-md-offset-3">
                <div class="well">
                    <form role="form" action="#" class="app-login">
                        <div class="form-group">
                            <label for="login_email">{__('email or login')}</label>
                            <input id="login_email" name="login_email" class="form-control required ilogin_email" placeholder="{__('enter email or login')}">
                        </div>
                        <div class="form-group">
                            <label for="pass">{__('password')}</label>
                            <input id="pass" name="pass" type="password" class="required ipass form-control"
                                   placeholder="{__('enter password')}">
                        </div>
                        <button type="submit" class="btn btn-default">{__('login')}</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
{/block}