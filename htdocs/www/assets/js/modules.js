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
        $.post('/ajax/multiroom/volume', {volume: volume, channels: $(e.target).closest('.js-room').data('id')});
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

    $(document).on('click', '.js-select-voice', function (e) {
        e.preventDefault();
        $.post('/ajax/multiroom/setvoice', {name: $(e.target).data('name')});
    });

    function getPlaylists() {

        $.post('/ajax/multiroom/playlists', {}, function (items) {
            if (items) {
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
                            $channel.find('.js-duration').html(secondsToHms(items[i].Duration));
                            $channel.find('.js-position').html(secondsToHms(items[i].Position));
                            var current = items[i].Current.split(/[\\\/]/);

                            $channel.find('.js-current-song').html(current[current.length - 1]);
                            $channel.find('.js-current-song-full').html(items[i].Current);
                            $channel.find('.js-playlist').html('Плейлист #' + items[i].PlaylistId);
                            $channel.data('playlist', items[i].PlaylistId);

                            if (parseInt(items[i].IsPlaying)) {
                                $channel.find('.js-play-playlist').removeClass('play').removeClass('js-play-playlist').addClass('pause js-pause');
                            } else {
                                $channel.find('.js-pause').removeClass('pause').removeClass('js-pause').addClass('play js-play-playlist');
                            }
                        }
                    }
                }
            }
            setTimeout(function () {
                getPlaylists()
            }, 1000);
        });
    }

    function getChannels() {

        $.post('/ajax/multiroom/channels', {}, function (items) {
            if (items) {
                items = $.parseJSON(items);
                if (items) {
                    for (var i = 0; i < items.length; i++) {
                            var $channel = $('.js-room-' + items[i].Channel);
                            $channel.find('.js-volume').css('width', (items[i].Volume * 100) + '%');

                    }
                }
            }
            setTimeout(function () {
                getChannels()
            }, 1000);
        });
    }

    function base64_decode( data ) {	// Decodes data encoded with MIME base64
        //
        // +   original by: Tyler Akins (http://rumkin.com)


        var b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
        var o1, o2, o3, h1, h2, h3, h4, bits, i=0, enc='';

        do {  // unpack four hexets into three octets using index points in b64
            h1 = b64.indexOf(data.charAt(i++));
            h2 = b64.indexOf(data.charAt(i++));
            h3 = b64.indexOf(data.charAt(i++));
            h4 = b64.indexOf(data.charAt(i++));

            bits = h1<<18 | h2<<12 | h3<<6 | h4;

            o1 = bits>>16 & 0xff;
            o2 = bits>>8 & 0xff;
            o3 = bits & 0xff;

            if (h3 == 64)	  enc += String.fromCharCode(o1);
            else if (h4 == 64) enc += String.fromCharCode(o1, o2);
            else			   enc += String.fromCharCode(o1, o2, o3);
        } while (i < data.length);

        return enc;
    }



    function secondsToHms(d) {
        d = Number(d);
        var h = Math.floor(d / 3600);
        var m = Math.floor(d % 3600 / 60);
        var s = Math.floor(d % 3600 % 60);
        return ((h > 0 ? h + ":" + (m < 10 ? "0" : "") : "") + m + ":" + (s < 10 ? "0" : "") + s); }
    getChannels();
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

