{extends "front/template/index.tpl"}
{block "content"}
    <div class="container">
        <h3 class="text-center">{__('welcome login form')}</h3>
        <div class="row">
            <div class="col-md-6 col-md-offset-3">
                <div class="well">
                    <form role="form" action="#" class="app-login">
                        <div class="form-group">
                            <label for="exampleInputEmail1">{__('email or login')}</label>
                            <input name="login_email" class="form-control required ilogin_email" placeholder="{__('enter email or login')}">
                        </div>
                        <div class="form-group">
                            <label for="exampleInputPassword1">{__('password')}</label>
                            <input name="pass" type="password" class="required ipass form-control"
                                   placeholder="{__('enter password')}">
                        </div>
                        <button type="submit" class="btn btn-default">{__('login')}</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
{/block}