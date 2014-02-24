(function ($) {


    $(document).ready(function () {

        APP.submit('.app-login', '/ajax/login', {callback: function (res) {
            if (res) {
                APP.redirect('/admin/');
            }
        }});


        APP.submit('.app-edit-device', '/ajax/devices/edit');

        APP.submit('.app-add-device', '/ajax/devices/add', {callback: function (res) {
            if (res) {
                var $res = $.parseJSON(res);
                window.location = '/admin/devices/' + $res.values.id;
            }
        }});

        APP.submit('.app-add-scenario', '/ajax/scenarios/add', {callback: function (res) {
            if (res) {
                var $res = $.parseJSON(res);
                window.location = '/admin/scenarios/' + $res.values.id;
            }
        }});


        $(document).on('keyup', '.app-filename-input', function () {
            $('.app-filename').text($(this).val());
        });
        $('.app-filename-input').trigger('keyup');
    });


})(jQuery);