{extends "front/template/index.tpl"}
{block "content"}
    <div class="container">
        <h1>Добро пожалвать </h1>
        <div class="row">
            <div class="col-md-6 col-md-offset-3">
                <div class="well">
                    <form role="form" action="#">
                        <div class="form-group">
                            <label for="exampleInputEmail1">Email address</label>
                            <input type="email" class="form-control" id="exampleInputEmail1" placeholder="Enter email">
                        </div>
                        <div class="form-group">
                            <label for="exampleInputPassword1">Password</label>
                            <input type="password" class="form-control" id="exampleInputPassword1"
                                   placeholder="Password">
                        </div>
                        <button type="submit" class="btn btn-default">Submit</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
{/block}