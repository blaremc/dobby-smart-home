(function ($) {


    $(document).ready(function () {

        APP.submit('.app-login', '/ajax/login', {callback: function (res) {
            if (res) {
                APP.redirect('/admin/');
            }
        }});

    });


})(jQuery);