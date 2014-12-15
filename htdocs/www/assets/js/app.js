(function ($) {
    function APP() {

        var at = [
                'bottom center',
                'top center',
                'left center',
                'right center'
            ],
            my = [
                'top center',
                'bottom center',
                'right center',
                'left center'
            ];

        var app = this;


        this.regFuncCheckHash = function () {};

        // Флаги
        this.f = {
            notCloseTagAdd: false,
            notClosePopover: false,
            notAddToFilter: false,
            notCloseSearchBlock: false,
            suggestFilterWait: false,
            filterLoad: false,
            updateFilter: false,
            forcePageNum: false
        };

        // Дефолтные состояния
        this.d = {
            timeoutSuggest: 500
        };

        // Различные параметры
        this.p = {
            timeoutSuggest: 0,
            filterLoad: 0,
            globalHash: '',
            page: 0
        };

        this._spage = null;

        this.submit = function (selector, url, opt) {

            var form = new this.form(selector, url, opt);
            form.submit();
            return form;
        };

        /**
         * Singleton for page
         * @param opt
         * @param forms Bind forms
         */
        this.page = function (opt, forms) {

            if (!this._spage) {

                this._spage = new this._page(opt, forms);
                this._spage.init();
            }
            return this._spage;
        };

        // Singleton for page
        this.delrest = function (name, callbackadd, callbackdel) {

            return new this._delrest(name, callbackadd, callbackdel);
        };


        // Singleton for tooglebutton
        this.toggleButton = function (name, addFunction, delFunction) {

            return new this._toggleButton(name, addFunction, delFunction);
        };

        this._toggleButton = function (name, addFunction, delFunction) {


            $(name).each(function () {
                var mode = $(this).attr('data-mode');
                var cl = '';
                if (!mode) {
                    if ($(this).hasClass('btn-danger')) {
                        mode = 'del';
                        cl = 'app-added';
                    }
                    if ($(this).hasClass('btn-success')) {
                        mode = 'add';
                        cl = 'app-deleted';
                    }
                }
                $(this).attr('data-mode', mode).parents('tr').addClass(cl);
            });

            $(document).on('click', name, function () {


                var mode = $(this).attr('data-mode');

                var delicon = $(this).attr('data-delicon') ? $(this).attr('data-delicon') : 'icon-remove-sign';

                var addicon = $(this).attr('data-addicon') ? $(this).attr('data-addicon') : 'icon-plus-sign';


                var element = this;
                switch (mode) {
                    case 'add':

                        addFunction(this, function () {
                            $(element).attr('data-mode', 'del').addClass('btn-danger').removeClass('btn-success')
                                .find('i').addClass(delicon).removeClass(addicon).end().find('span').text($(element).attr('data-messdel'))
                                .parents('tr').removeClass('app-deleted').removeClass('deleted').addClass('app-added');
                        });
                        break;

                    case 'del':


                        delFunction(this, function () {
                            $(element).attr('data-mode', 'add').removeClass('btn-danger').addClass('btn-success')
                                .find('i').removeClass(delicon).addClass(addicon).end().find('span').text($(element).attr('data-messadd'))
                                .parents('tr').addClass('app-deleted').removeClass('app-added');

                            if ($(element).attr('data-hidedel'))
                                $(element).addClass('deleted');
                        });
                        break;

                    default:
                        return;
                }

                return false;
            });


        };


        // Class Page
        this._page = function (opt, forms) {

            var papp = this;
            // Default settings
            this.opt = {

                url: location.hash.length == 0 ? location.href : location.href.substring(0, location.href.length - location.hash.length),

                ajaxcontent: '#ajaxcontent',

                showdeleted: '#app-showdel',

                refreshButtom: '#app-refreshpage',

                searchInput: '#app-search-input',

                pagination: '.pagination',

                page: 1,

                post: {}

            };
            this.forms = forms ? forms : [];

            this.opt = $.extend(this.opt, opt);
            // Убираем все лишнее
            this.opt.url = this.opt.url.replace(/(\?.*)/, '');
            // Убираем page
            this.opt.url = this.opt.url.replace(/(\/page[0-9]+)/, '');
            // убираем завершающий слеш
            if (this.opt.url.substring(this.opt.url.length - 1) == '/') this.opt.url = this.opt.url.substring(0, this.opt.url.length - 1);

            this.refresh = function () {

                this.opt.post.showdel = $(this.opt.showdeleted + ':checked').length;

                if (window.APP.f.forcePageNum)
                    this.opt.page = window.APP.p.page;

                if (this.forms.length > 0) {
                    if (this.forms.length == 1) {
                        // Если форма одна, то отправляем ее данные прям в пост
                        this.opt.post = $.extend(this.opt.post, $(this.forms[0]).serializeJSON());
                    }
                    else {
                        // Есди не одна, то в массивах
                        for (var i = 0; i < this.forms.length; i++) {
                            this.opt.post.forms[i] = $(this.forms[i]).serializeJSON();
                        }
                    }
                }

                this.opt.post.search = $(this.opt.searchInput).val();
                console.log(this.opt.url);
                app.loadContent(this.opt.ajaxcontent, this.opt.url + '/page' + this.opt.page, this.opt.post);
                this.opt.post = {};
            };


            this.init = function () {

                $(document).on('change', this.opt.showdeleted, function () {
                    papp.refresh();
                });

                $(document).on('click', this.opt.refreshButtom, function () {
                    papp.refresh();
                    return false;
                });

                $(document).on('click', this.opt.pagination + ' a', function () {

                    papp.opt.page = $(this).attr('data-page');

                    window.APP.p.page = papp.opt.page;
                    var h = location.hash;


                    h = h.replace(/page=[\d]+/, '');

                    var sep = h[h.length - 1] == '/' ? '' : '/';
                    h += sep + 'page=' + papp.opt.page;

                    location.hash = location.hash.length <= 1 ? '!' + h : h;
                    papp.refresh();
                    return false;
                });

                papp.opt.page = $('.active a', papp.opt.pagination).attr('data-page');
                papp.opt.page = papp.opt.page ? papp.opt.page : 1;
            };


        };

        this._delrest = function (name, callbackadd, callbackdel) {

            callbackadd = callbackadd ? callbackadd : function () {};

            callbackdel = callbackdel ? callbackdel : function () {};

            $('.app-restore-' + name).each(function () {

                $(this).parents('tr').addClass('app-deleted');
            });

            $('.app-del-' + name).each(function () {

                $(this).parents('tr').addClass('app-added');
            });

            $(document).on('click', '.app-del-' + name, function () {
                var but = this;

                var url = '/admin/ajax/' + name + 'Del';

                $.appPost(url, {id: $(this).parents('tr').attr('data-id'), token: $('#' + name + 'DelToken').val()}, function (data) {

                    if (data) {
                        app.showError(data, null, function () {

                            callbackadd(but);
                            $(but).removeClass('btn-danger').addClass('btn-success').removeClass('app-del-' + name).addClass('app-restore-' + name)
                                .find('i').removeClass('icon-remove-sign').addClass('icon-share').end().find('span').text('Восстановить').
                                parents('tr').addClass('app-deleted').addClass('deleted').removeClass('app-added');

                            app.whenload();
                        });
                    }
                });
                return false;
            });

            $(document).on('click', '.app-restore-' + name, function () {

                var but = this;

                var url = '/admin/ajax/' + name + 'Rest';

                $.appPost(url, {id: $(this).parents('tr').attr('data-id'), token: $('#' + name + 'RestToken').val()}, function (data) {

                    if (data) {
                        app.showError(data, null, function () {

                            callbackadd(but);
                            $(but).addClass('btn-danger').removeClass('btn-success').addClass('app-del-' + name).removeClass('app-restore-' + name)
                                .find('i').addClass('icon-remove-sign').removeClass('icon-share').end().find('span').text('Удалить').
                                parents('tr').removeClass('app-deleted').removeClass('deleted').addClass('app-added');
                            app.whenload();
                        });
                    }
                });
                return false;
            });
        };

        this.delImage = function(selector, url){

            $(selector).on('click',function(){

                $.appPost(url,{token:$(this).attr('data-token'), id:$(this).attr('data-id')}, function (data) {
                        window.APP.showError(data,null, function(){
                            window.location.reload();
                        });

                });

                return false;
            });

        };


        /**
         * Class form for submit
         * @param selector String Form selector
         * @param url String Url for request
         * @param callback Function Calling function when request is success
         * @param cancel Function Bind on "Cancel" with class "btn-cancel"
         */
        this.form = function (selector, url, o) {


            o = o ? o : {};
            var fapp = this;
            var messcallback;
            this._selector = selector;
            this._url = url ? url : $(this._selector).attr('action');

            this._callback = o.callback ? o.callback : function () {};

            this._cancel = o.cancel ? o.cancel : function () {

                if (window.history.length > 1)
                    window.history.back();
                else

                    window.location = '/';

            };

            this._submited = false;

            this.opt = {};
            this.opt.clearform = o.clearform ? o.clearform : false;

            $(document).on('keypress', selector + ' .app-ctrlenter', function (e) {


                // У хрома это 10 символ
                if ((e.which == 13 || e.which == 10) && e.ctrlKey)
                    $(this).closest(selector).submit();
            });

            this.init = function () {

                this._form = null;

                this._filecount = 0;
                this._fileuploads = 0;

                this._data = '';

                this.values = {};
                this.message = {};

                this._callbackCall = false;

                messcallback = function () {};

            };

            /**
             * Submit post withs callback

             */
            this.submit = function () {

                $(document).on('submit', fapp._selector, function () {

                    fapp.init();

                    fapp._form = this;
                    if (fapp._submited) return false;

                    fapp.clearFormError();
                    fapp.formDisabled();

                    var url = fapp._url;

                    $.appPost(url, $(fapp._form).serializeArray(), function (data) {

                        try {
                            fapp._data = data;
                            var $data = $.parseJSON(data);
                            $data = $data ? $data : {};
                            fapp.values = $data.values ? $data.values : {};
                            fapp.message = $data.messages ? $data.messages : {};
                            fapp.showError();
                        }
                        catch (e) {

                        }
                        fapp.formEnabled();

                    });
                    return false;
                });


                $(fapp._selector).find('.btn-cancel').on('click', document, this._cancel);
            };

            this.formDisabled = function () {

                this._submited = true;
                $(this._form).find('button').not('[type=button]').attr('disabled', 'disabled').find('i[class^=icon]').addClass('icon-load');
            };

            this.formEnabled = function () {

                this._submited = false;
                $(this._form).find('button').not('[type=button]').removeAttr('disabled').find('i[class^=icon]').removeClass('icon-load');
            };

            this.fileupload = function (file) {

                var $file = $(file),
                    $parent = $file.parent();

                fapp.formDisabled();
                var id = fapp.values.id;
                if (parseInt(id) == 0) {
                    app.showGeneralError('Ошибка загрузки файла');
                }

                var idframe = 'file' + parseInt(Math.random() * 1000);

                // Create Iframe
                var $iframe = $('<iframe>').attr({id: idframe, name: idframe, src: '#'}).addClass('hide').load(function () {

                    var text = $(this).contents().text();
                    app.showError(text, null);
                    fapp.formEnabled();

                    fapp._fileuploads++;
                    $file.appendTo($parent);
                    $form.remove();

                    // Хук, в фоксе если сразу удалить, то будет иконка в виде загрузки страницы
                    setTimeout(function () {$(this).remove();}, 100);
                });

                // Create form
                var $form = $('<form>').attr({action: this._url, enctype: "multipart/form-data", target: idframe, method: 'POST'  }).addClass('hide').submit(function () {
                });

                // Add new id
                $form.append($('<input>').attr({type: 'hidden', name: 'id'}).val(id))
                    .append($('<input>').attr({type: 'hidden', name: 'token'}).val($('#upload-token').val()))
                    .append($file);

                // Append Form and Iframe to Body
                $('body').append($iframe).append($form);

                $form.submit();
            };

            /**
             * Callback
             */
            this.callback = function () {


                if (this._callbackCall) return;
                this._callbackCall = true;

                $('input[type=file]', this._form).each(function () {

                    if ($(this).val() != '') {
                        fapp._filecount++;
                        fapp.fileupload(this);
                    }
                });
                fapp.waitupload();
            };

            /**
             * Wait upload all files
             */
            this.waitupload = function () {

                if (this._filecount == this._fileuploads) {

                    messcallback();
                    this._callback(this._data, fapp._form);
                    this.clearForm();
                }
                else
                    setTimeout(function () {fapp.waitupload()}, 500);

            };

            this.clearForm = function () {


                if (!this.opt.clearform) return;
                $('input, textarea', this._form).not('*[type=hidden]').val('');
                $('*[id$=filename]').text('');
            };

            /**
             * Убирает все ошибки у формы
             */
            this.clearFormError = function () {

                $('.control-group', fapp._form).removeClass('error').find('input, select, textarea');
                $('.required', fapp._form).qtip('hide');
            };


            /**
             * Обработка пришедших через аякс данных
             */
            this.showError = function () {

                this._callback = this._callback ? this._callback : function () { };

                if (this._data != '') {

                    try {

                        //Пробегаемся по массиву JSON
                        for (var i = 0; i < this.message.length; i++) {

                            // Если передано сообщение и это ошибка (code>0)
                            if (this.message[i].code && this.message[i].code != 0) {

                                this._callbackCall = true;
                                if (!this._form) {
                                    // Обычная ошибка
                                    app.showGeneralError(this.message[i].mess);
                                }
                                else {
                                    // Ошибка заполнения формы
                                    fapp.showFormError(this.message[i].type, this.message[i].mess);
                                }
                            }
                            else {

                                var mess = this.message[i].mess;
                                messcallback = function () {

                                    // Если есть сообщение, то отображаем
                                    if (mess) {
                                        app.showGeneralMessage(mess);
                                    }
                                };
                                //Вызываем callback
                                fapp.callback();
                            }
                        }
                    }
                    catch ($e) {
                        //Пришел не JSON или что-то случилось
                    }
                }
                fapp.callback();
            };

            /**
             * Показывает сообщение об ошибке в форме
             * @param type - класс поля, которое ищем в форму
             * @param message - сообщение, которое показывается у поля
             */
            this.showFormError = function (type, message) {

                if (type == 'igeneral') {
                    app.showGeneralError(message);
                    return;
                }
                var ormass;

                // Устанавливаем позицию подсказок, в зависимости от класс формы
                var formstyle;
                if ($(this._form).hasClass('qtip-bottom'))  formstyle = 1;
                if ($(this._form).hasClass('qtip-top'))  formstyle = 2;
                if ($(this._form).hasClass('qtip-left'))  formstyle = 3;
                if ($(this._form).hasClass('qtip-right')) formstyle = 4;
                formstyle = formstyle || (app.external ? 1 : 4);

                if ($('.required.' + type, this._form).length == 0) {

                    app.showGeneralError(message);
                    return;
                }

                $('.required.' + type, this._form).each(function () {

                    var $input = $(this);
                    // Устанавливаем позицию подсказаок, в зависимости от класса поля
                    var inputstyle;
                    if ($input.hasClass('qtip-bottom'))  inputstyle = 1;
                    if ($input.hasClass('qtip-top'))  inputstyle = 2;
                    if ($input.hasClass('qtip-left'))  inputstyle = 3;
                    if ($input.hasClass('qtip-right')) inputstyle = 4;
                    inputstyle = inputstyle || formstyle;
                    var id = Math.floor(Math.random() * 100);

                    // Добавляем класс error для bootstrap
                    $input.parents('.control-group').addClass('error');

                    // Добавляем ошибку
                    var $api = $input.qtip({
                        content: {
                            text: message
                        },
                        position: {
                            my: my[inputstyle - 1],
                            at: at[inputstyle - 1]
                        },
                        style: {
                            // tip: true,
                            classes: 'ui-tooltip-shadow ui-tooltip-red ui-tooltip-rounded'
                        },
                        show: {
                            event: false,
                            ready: true,
                            effect: 'fade'
                        },
                        hide: {
                            event: 'focus',

                            target: $input.parent().find('.required')
                        },
                        id: 'hint' + id
                    });

                });

            };


        }

        /**
         * When DOM is ready
         */
        this.ready = function () {
            // Событие на фокус элементов с проверкой
            $(document).on('focus', '.required', (function () {
                var $parent = $(this).parents('.control-group');
                $parent.removeClass('error');
            }));
        };

        /**
         * Обработка пришедших через аякс данных
         * @param data - Данные из аякса
         * @param form=null - Форма, где необходимо отобразить подсказки (может быть null)
         * @param callback - Функция, выполняемая в случае отсутствия ошибок (по умолчанию перезагрузка страницы)
         *
         */
        this.showError = function (data, form, callback) {

            // Флаг для единственного вызова callback
            var callbackcall = false;

            // Если не прислали функцию, назначаем сами
            callback = callback ? callback : function () {
//            location.reload();
            };
            try {

                var $data = $.parseJSON(data);
                //Пробегаемся по массиву JSON
                for (var i = 0; i < $data.messages.length; i++) {

                    // Если передано сообщение и это ошибка (code>0)
                    if ($data.messages[i].code && $data.messages[i].code != 0) {


                        callbackcall = true;
                        if (!form) {
                            // Обычная ошибка
                            app.showGeneralError($data.messages[i].mess);
                        }
                        else {
                            // Ошибка заполнения формы
                            app.showFormError(form, $data.messages[i].type, $data.messages[i].mess);
                        }
                    }
                    else {


                        // Если есть сообщение, то отображаем
                        if ($data.messages[i].mess) {
                            app.showGeneralMessage($data.messages[i].mess);
                        }
                        //Вызываем callback
                        if (!callbackcall) {
                            callback(data);
                            callbackcall = true;
                        }
                    }
                }
            }
            catch ($e) {
                //Пришел не JSON или что-то случилось

            }
            if (!callbackcall) {
                callback(data);

            }
        };


        /**
         * Показывает обычное сообщение
         * @param message - текст сообщения
         */
        this.showGeneralMessage = function (message) {
            if (message == '') return;
            $.gritter.add({
                // (string | обязательно) заголовок сообщения
                title: 'Сообщение!',
                // (string | обязательно) текст сообщения
                text: message,
                time: 5000,
                class_name: 'gritter-message'
            });
        };

        /**
         * Показывает сообщение об ошибке
         * @param message
         */
        this.showGeneralError = function (message) {
            if (message == '') return;
            $.gritter.add({
                // (string | обязательно) заголовок сообщения
                title: 'Ошибка!',
                // (string | обязательно) текст сообщения
                text: message,
                time: 5000,
                class_name: 'gritter-error'
            });
        };


        /**
         * Показывает сообщение об ошибке в форме
         * @param form - форма в которой осуществляется поиск поля
         * @param type - класс поля, которое ищем в форму
         * @param message - сообщение, которое показывается у поля
         */
        this.showFormError = function (form, type, message) {


            if (type == 'igeneral') {
                app.showGeneralError(message);
                return;
            }
            var ormass;

            // Устанавливаем позицию подсказок, в зависимости от класс формы
            var formstyle;
            if ($(form).hasClass('qtip-bottom'))  formstyle = 1;
            if ($(form).hasClass('qtip-top'))  formstyle = 2;
            if ($(form).hasClass('qtip-left'))  formstyle = 3;
            if ($(form).hasClass('qtip-right')) formstyle = 4;
            formstyle = formstyle || (app.external ? 1 : 4);

            var maxsize = 0;

            if ($('.required.' + type, form).length == 0) {

                app.showGeneralError(message);
                return;
            }

            $('.required.' + type, form).each(function () {

                var $input = $(this);
                // Устанавливаем позицию подсказаок, в зависимости от класса поля
                var inputstyle;
                if ($input.hasClass('qtip-bottom'))  inputstyle = 1;
                if ($input.hasClass('qtip-top'))  inputstyle = 2;
                if ($input.hasClass('qtip-left'))  inputstyle = 3;
                if ($input.hasClass('qtip-right')) inputstyle = 4;
                inputstyle = inputstyle || formstyle;
                var id = Math.floor(Math.random() * 100);

                // Добавляем класс error для bootstrap
                $input.parents('.control-group').addClass('error');

                // Добавляем ошибку
                var $api = $input.qtip({
                    content: {
                        text: message
                    },
                    position: {
                        my: my[inputstyle - 1],
                        at: at[inputstyle - 1]
                    },
                    style: {
                        // tip: true,
                        classes: 'ui-tooltip-shadow ui-tooltip-red ui-tooltip-rounded'
                    },
                    show: {
                        event: false,
                        ready: true,
                        effect: 'fade'
                    },
                    hide: {
                        event: 'focus',
                        target: $input.parent().find('.required')
                    },
                    id: 'hint' + id
                });

            });

        };


        this.loadContent = function (selector, url, post, callback) {


            post = post ? post : {};
            callback = callback ? callback : function () {};
            if (post.length) {
                post.push({name: 'ajax', value: true});
            }
            else {
                post.ajax = true;
            }


            var $div = $('<div>').addClass('animated');

            $(selector).animate({opacity: 0.2}, 500).wrap($div);


            $.appPost(url, post, function (data) {
                $(selector).html(data).stop(true).animate({opacity: 1}, 200).unwrap($div);

                callback();
                app.whenload();
            });

        };

        this.showWindow = function (url, data, complete, whenclose, element) {


            var post = 'ajax=true';
            if (typeof data == 'object') {
                for (var key in data) {

                    post += '&' + key + '=' + data;
                }
            }
            else
                post += '&' + data;


            var urls = [];
            var index = 0;
            var type = '';

            if ($(element).hasClass('img')) {
                var rel = $(element).attr('rel');
                var i = 0;
                $('a[rel=' + rel + ']').each(function () {
                    if (url == $(this).attr('href'))
                        index = i;
                    urls[urls.length] = $(this).attr('href');
                    i++;
                });
                var title = $(element).attr('title');
                type = 'image';

            }
            else {
                urls = [url];
                type = 'ajax';

            }

            if (type == 'image') {

                $.fancybox(urls, {
                    centerOnScroll: true,
                    type: 'image',
                    index: index,
                    titleShow: true,
                    titlePosition: 'inside',
                    'titleFormat': function (title, currentArray, currentIndex, currentOpts) {
                        return '<span id="fancybox-title">Снимок ' + (currentIndex + 1) + ' / ' + currentArray.length + ' ' + title + '</span>';
                    },
                    onClosed: whenclose ? whenclose : function () {}
                });

            }
            else {
                $.fancybox(url, {
                    centerOnScroll: true,
                    ajax: {
                        type: "POST",
                        data: post

                    },
                    padding: 0,
                    onComplete: function () {

                        if (complete)complete();
                        $('#fancybox-content input:first').focus();

                    },
                    type: 'ajax',
                    onClosed: whenclose ? whenclose : function () {}
                });

            }
        };


        this.redirect = function (url) {

            this.changeHash(url);

            window.location = url;
        };

        this.changeHash = function (url) {


            var host = 'http://' + window.location.hostname;

            if (url.indexOf(host) !== -1)
                url = url.substring(host.length);

            window.parent.location.hash = url;
        };


        this.whenload = function () {

            $(document).on('click', '.app-btn-close', function () {
                if (typeof $.fancybox != 'undefined')
                    $.fancybox.close();
                return false;
            });

//        $('.app-ajax-textarea').markItUp(myBbcodeSettings).wrap($('<div>').addClass('row-fluid'));

        };

        /**
         * Меняем якорь без обновления страницы
         * @param hash
         */
        this.changeHash = function (hash) {
            if ('#' + hash != location.hash) {
                location.hash = '!/' + hash;
                window.APP.p.globalHash = location.hash;
            }
        };
        /**
         * Проверяем якорь
         */
        this.checkChangeHashe = function () {

            if (window.APP.p.globalHash != location.hash) {
                window.APP.p.globalHash = location.hash;
                window.APP.checkHashe();
            }
            setTimeout(window.APP.checkChangeHashe, 500);
        };

        /**
         * регистрация функции, которая будет вызываться при изменении хэша
         * @param f
         */
        this.registerCheckhash = function (f) {

            var o = this.regFuncCheckHash;

            this.regFuncCheckHash = function () {
                o();
                f();
            };
        }

        /**
         * изменился якорь, выполняем некоторые функции
         */
        this.checkHashe = function () {

            this.regFuncCheckHash();
        };

    }

    window.APP = new APP();


    $(document).ready(function () {
        window.APP.ready();
        window.APP.whenload();
        window.APP.checkChangeHashe();
    });


    $.fn.serializeJSON = function () {
        var json = {};
        jQuery.map($(this).serializeArray(), function (n, i) {
            if (json[n['name']]) {

                if (typeof json[n['name']] == 'string') {
                    var a = [json[n['name']], n['value']];
                    json[n['name']] = a;
                }
                else
                    json[n['name']][json[n['name']].length] = n['value'];
            }
            else
                json[n['name']] = n['value'];

        });
        return json;
    };


    $.appPost = function (url, data, callback) {

        $.ajax(url, {
            data: data,
            method: 'post',
            complete: function (xhr, text) {

                switch (xhr.status) {

                    case 500:
                        window.APP.showGeneralError('Ошибка сервера');
                        break;
                    case 403:
                        window.APP.showGeneralError('Нет доступа');
                        break;
                    case 404:
                        window.APP.showGeneralError('Страница не найдена');
                        break;
                }
                if (xhr.status != 200) {
                    callback(false, xhr);
                } else {
                    callback(xhr.responseText, xhr);
                }
            }
        });
    }


    function checkFlash() {

        var flashinstalled = false;
        if (navigator.plugins) {
            if (navigator.plugins["Shockwave Flash"]) {
                flashinstalled = true;
            }
            else if (navigator.plugins["Shockwave Flash 2.0"]) {
                flashinstalled = true;
            }
        }
        else if (navigator.mimeTypes) {
            var x = navigator.mimeTypes['application/x-shockwave-flash'];
            if (x && x.enabledPlugin) {
                flashinstalled = true;
            }
        }
        else {
            // на всякий случай возвращаем true в случае некоторых экзотических браузеров
            flashinstalled = true;
        }
        return flashinstalled;
    }

})(jQuery);


