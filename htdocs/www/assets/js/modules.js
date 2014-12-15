$(function () {


    $(document).on('click', '.js-multiroom-test', function (e) {

        e.preventDefault();
        $.post('/ajax/multiroom/test', {id: $(this).data('id')});

    });
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

