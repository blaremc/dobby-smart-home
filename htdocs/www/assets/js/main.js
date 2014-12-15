(function ($) {


    $(document).ready(function () {

        APP.submit('.app-login', '/ajax/login', {callback: function (res) {
            if (res) {
                APP.redirect('/admin/');
            }
        }});


        APP.submit('.app-add-device', '/ajax/devices/add', {callback: function (res) {
            if (res) {
                var $res = $.parseJSON(res);
                window.location = '/admin/devices/' + $res.values.id;
            }
        }});
        APP.submit('.app-edit-device', '/ajax/devices/edit');

        APP.submit('.app-add-scenario', '/ajax/scenarios/add', {callback: function (res) {
            if (res) {
                var $res = $.parseJSON(res);
                window.location = '/admin/scenarios/' + $res.values.id;
            }
        }});
        APP.submit('.app-edit-scenario', '/ajax/scenario/edit');

        APP.submit('.app-add-actions', '/ajax/actions/add', {callback: function (res) {
            if (res) {
                var $res = $.parseJSON(res);
                window.location = '/admin/actions/' + $res.values.id;
            }
        }});
        APP.submit('.app-edit-action', '/ajax/actions/edit');

        $(document).on('keyup', '.app-filename-input', function () {
            $('.app-filename').text($(this).val());
        });
        $('.app-filename-input').trigger('keyup');

        $(document).on('change', '.app-action-scenario', function () {
            $('.app-scenario').hide();
            $('.app-scenario-' + $(this).val()).show();
        });
        $('.app-action-scenario').trigger('change');


        $(document).on('click', '.app-start-action', function (e) {
            $.post('/ajax/actions/start', {id: $(e.target).data('id')});
            e.preventDefault();
        });


        APP.submit('.app-module-action');
    });


})(jQuery);