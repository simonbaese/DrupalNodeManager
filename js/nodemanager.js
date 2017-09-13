/* jQuery of Nodemanager */

(function ($) {

    $.fn.customAttach = function (nid, nodeview, rotate) {
        if ($('tr#content-row-' + nid).is(':visible')) {
            $('#nm-inspect-id-' + nid).removeClass('rotate');
            $('tr#content-row-' + nid).insertAfter($('tr#nm-hidden-content'));
            $('tr#content-row-' + nid + ' td').empty();
        } else {
            $('tr.content-row').insertAfter($('tr#nm-hidden-content'));
            $('span.rotate').removeClass('rotate');
            $('tr.content-row td').empty();
            $('tr#content-row-' + nid + ' td').append(nodeview);
            if ($('div#row-options-' + nid).is(':visible')) {
                $('tr#content-row-' + nid).insertAfter($('tr#row-' + nid).next()).hide();
            } else {
                $('tr#content-row-' + nid).insertAfter($('tr#row-' + nid));
                $('tr.row-options').insertAfter($('tr#nm-hidden-content'));
            }
            $('tr#content-row-' + nid).find('td').wrapInner('<div style="display: none;" />').parent().find('td > div').slideDown(500, function () {
                var $set = $(this);
                $set.replaceWith($set.contents());
            });
            if (rotate) $('#nm-inspect-id-' + nid).addClass('rotate');
        }
    };

    Drupal.ajax.prototype.commands.nodeManagerReattach = function (ajax, response, status) {
        Drupal.attachBehaviors();
    };

    Drupal.behaviors.nodeManager = {
        attach: function (context) {

            $('.nm-edit').once().on('click', function () {
                var nid = $(this).data('nid');
                if ($('tr#edit-row-' + nid).is(':visible')) {
                    $('tr#edit-row-' + nid).insertAfter($('tr#nm-hidden-content'));
                } else {
                    hideContent();
                    $('tr.content-row:not(#content-row-' + nid + ')').insertAfter($('tr#nm-hidden-content'));
                    $('tr.row-options').insertAfter($('tr#nm-hidden-content'));
                    $('tr#edit-row-' + nid).insertAfter($('tr#row-' + nid));
                }
            });

            $('.nm-inspect').once().on('click', function () {
                var nid = $(this).data('nid');
                $('input[name=nid]').val(nid);
                $('input[type=submit]#edit-inspect-submit').click();
            });

            // Hide everything on body click
            $('body').once().on('click', function (event) {
                if ($(event.target).closest('#nm-table').length == 0) {
                    hideContent();
                    $('tr.row-options').insertAfter($('tr#nm-hidden-content'));
                }
            });

            $(document).ready(function () {
                // Move form elements into table.
                // Move buttons for options into button row of table.
                $('.js-place-btn-row').each(function () {
                    var nid = $(this).data('nid');
                    $('div#row-buttons-' + nid).prepend(this);
                });

            });

            function hideContent() {
                $('tr.content-row').insertAfter($('tr#nm-hidden-content'));
                $('span.rotate').removeClass('rotate');
                $('tr.content-row td').empty();
            }
        }
    }

}(jQuery));