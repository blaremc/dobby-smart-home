$(function () {

    $(document).on('click', '.js-multiroom-test', function (e) {

        e.preventDefault();
        $.post('/ajax/multiroom/test', {id: $(this).data('id')});

    });

    $(document).on('click', '.js-multiroom-scan', function (e) {
        e.preventDefault();
        $.post('/ajax/multiroom/scan');
    });

    $(document).on('click', '.js-play', function (e) {
        e.preventDefault();
        var channels = $('[name="room"]:checked').map(function () {
            return $(this).val()
        }).get().join(',');
        $.post('/ajax/multiroom/play', {id: $(e.target).data('id'), channels: channels});
    });

    $(document).on('click', '.js-multiroom-stop', function (e) {
        e.preventDefault();
        var channels = $('[name="room"]:checked').map(function () {
            return $(this).val()
        }).get().join(',');
        $.post('/ajax/multiroom/stop', {channels: channels});
    });


    $(document).on('click', '.js-multiroom-say', function (e) {
        e.preventDefault();
        var channels = $('[name="room"]:checked').map(function () {
            return $(this).val()
        }).get().join(',');
        $.post('/ajax/multiroom/say', {channels: channels});
    });

    $(document).on('click', '.js-tracker', function (e) {

        var position = (e.offsetX / $('.js-tracker').width()) * 100;
        $.post('/ajax/multiroom/position', {
            position: position,
            playlist: $(e.target).closest('.js-room').data('playlist')
        });

    });

    $(document).on('click', '.js-volume-container', function (e) {

        var volume = (e.offsetX / $('.js-volume-container').width());
        $.post('/ajax/multiroom/volume', {volume: volume, playlist: $(e.target).closest('.js-room').data('playlist')});

    });
    $(document).on('click', '.js-stop', function (e) {
        e.preventDefault();
        $.post('/ajax/multiroom/stop', {playlist: $(e.target).closest('.js-room').data('playlist')});
    });

    $(document).on('click', '.js-play-playlist', function (e) {
        e.preventDefault();
        $.post('/ajax/multiroom/play', {playlist: $(e.target).closest('.js-room').data('playlist')});
    });


    $(document).on('click', '.js-pause', function (e) {
        e.preventDefault();
        $.post('/ajax/multiroom/pause', {playlist: $(e.target).closest('.js-room').data('playlist')});
    });
    $(document).on('click', '.js-prev', function (e) {
        e.preventDefault();
        $.post('/ajax/multiroom/prev', {playlist: $(e.target).closest('.js-room').data('playlist')});
    });

    $(document).on('click', '.js-next', function (e) {
        e.preventDefault();
        $.post('/ajax/multiroom/next', {playlist: $(e.target).closest('.js-room').data('playlist')});
    });

    function getPlaylists() {

        $.post('/ajax/multiroom/playlists', {}, function (items) {
            items = $.parseJSON(items);
            items = $.parseJSON(items);
            if (items) {
                for (var i = 0; i < items.length; i++) {
                    var channels = items[i].Channels.split(',');
                    for (var j = 0; j < channels.length; j++) {
                        var $channel = $('.js-room-' + channels[j]);
                        if (items[i].Duration != 0) {
                            $channel.find('.js-track').css('width', (items[i].Position / items[i].Duration * 100) + '%');
                        } else {
                            $channel.find('.js-track').css('width', 0);
                        }
                        $channel.find('.js-duration').html(items[i].Duration);
                        $channel.find('.js-position').html(items[i].Position);
                        $channel.find('.js-current-song').html(items[i].Current);
                        $channel.find('.js-playlist').html('Плейлист #' + items[i].PlaylistId);
                        $channel.data('playlist', items[i].PlaylistId);
                        $channel.find('.js-volume').css('width', (items[i].Volume * 100) + '%');

                        if (parseInt(items[i].IsPlaying)) {
                            $channel.find('.js-play-playlist').removeClass('play').removeClass('js-play-playlist').addClass('pause js-pause');
                        } else {
                            $channel.find('.js-pause').removeClass('pause').removeClass('js-pause').addClass('play js-play-playlist');
                        }
                    }

                }
            }
            setTimeout(function () {
                getPlaylists()
            }, 1000);
        });
    }

    getPlaylists();
});
$(function () {

    var r = $(".hsl").data('r') || 0;
    var g = $(".hsl").data('g') || 0;
    var b = $(".hsl").data('b') || 0;


    function post(url, params) {

        console.log('post', arguments);
        $.post(url, params);

    }

    var ledPost = _.throttle(post, 300);

    $(".hsl").ColorPickerSliders({
        flat: true,
        swatches: false,
        color: tinycolor("rgb " + r + ' ' + g + ' ' + b),
        order: {
            hsl: 1,
            preview: 2
        },
        onchange: function (container, color) {
            ledPost('/ajax/light/set', {
                smooth: $('#smooth').val(),
                r: color.rgba.r,
                g: color.rgba.g,
                b: color.rgba.b,
                id: $('#id').val()
            });

        }
    });


    $(".picker-bright").ColorPickerSliders({
        flat: true,
        swatches: false,
        color: tinycolor("rgb " + r + ' ' + g + ' ' + b),
        order: {
            hsl: 1
        },
        onchange: function (container, color) {

            ledPost( '/ajax/light/set', {
                smooth: $('#smooth').val(),
                l: color.hsla.l,
                id: $('#id').val()
            });
        }
    });
    $('.picker-bright .cp-hslhue').hide();
    $('.picker-bright .cp-hslsaturation').hide();

    $(document).on('click', '.js-light-rgb-off', function () {

        //$.post('/ajax/light/set', {r: 0, g: 0, b: 0, smooth: 1, id: $('#id').val()});
        $(".hsl").trigger("colorpickersliders.updateColor", "#000000");
    });

    $(document).on('click', '.js-light-light-off', function () {

        //$.post('/ajax/light/set', {l: 0, smooth: 1, id: $('#id').val()});
        $(".picker-bright").trigger("colorpickersliders.updateColor", "#000000");
    });
});

