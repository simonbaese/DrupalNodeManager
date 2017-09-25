/* jQuery of Nodemanager */

(function ($) {

  $.fn.customAttach = function (nid, nodeView, rotate) {

    var contentRow = $('tr#content-row-' + nid);
    var hiddenContent = $('tr#nm-hidden-content');

    if (contentRow.is(':visible')) {
      $('#nm-inspect-id-' + nid).removeClass('rotate');
      contentRow.insertAfter(hiddenContent);
      $('tr#content-row-' + nid + ' td').empty();
    } else {
      $('tr.content-row').insertAfter(hiddenContent);
      $('span.rotate').removeClass('rotate');
      $('tr.content-row td').empty();
      $('tr#content-row-' + nid + ' td').append(nodeView);
      if ($('div#row-options-' + nid).is(':visible')) {
        contentRow.insertAfter($('tr#row-' + nid).next()).hide();
      } else {
        contentRow.insertAfter($('tr#row-' + nid));
        $('tr.row-options').insertAfter(hiddenContent);
      }
      contentRow.find('td').wrapInner('<div style="display: none;" />').parent().find('td > div').slideDown(500, function () {
        var $set = $(this);
        $set.replaceWith($set.contents());
      });
      if (rotate) $('#nm-inspect-id-' + nid).addClass('rotate');
    }
  };

  $.fn.youvoManagerFadeOut = function (selector) {

    $(selector).fadeOut(1195);
    setTimeout(remove, 1200);

    function remove() {
      $('#remove_me').remove();
    }
  };

  Drupal.ajax.prototype.commands.nodeManagerReattach = function (ajax, response, status) {
    Drupal.attachBehaviors();
  };

  Drupal.behaviors.nodeManager = {
    attach: function (context) {

      $('.nm-edit').once().on('click', function () {

        var nid = $(this).data('nid');
        var hiddenContent = $('tr#nm-hidden-content');
        var editOptionRow = $('tr#edit-row-' + nid);

        if (editOptionRow.is(':visible')) {
          editOptionRow.insertAfter(hiddenContent);
        } else {
          $('tr.content-row:not(#content-row-' + nid + ')').insertAfter(hiddenContent);
          $('tr.row-options').insertAfter(hiddenContent);
          editOptionRow.insertAfter($('tr#row-' + nid));
        }
      });

      $('.nm-inspect').once().on('click', function () {

        var nid = $(this).data('nid');

        $('input[name=nid]').val(nid);
        $('input[type=submit]#edit-inspect-submit').click();
      });

      $('.nm-status').once().on('click', function () {

        var nid = $(this).data('nid');
        var hiddenContent = $('tr#nm-hidden-content');
        var statusOptionRow = $('tr#status-row-' + nid);

        if (statusOptionRow.is(':visible')) {
          statusOptionRow.insertAfter(hiddenContent);
        } else {
          statusOptionRow.show();
          $('tr.content-row:not(#content-row-' + nid + ')').insertAfter(hiddenContent);
          $('tr.row-options').insertAfter(hiddenContent);
          $('[id^=option-status]').css('display','none');
          if($(this).hasClass('nm-status-true')) $('div#option-status-false').show();
          else $('div#option-status-true').show();
          $('input[name=nid]').val(nid);
          statusOptionRow.insertAfter($('tr#row-' + nid));
        }
      });

      $('.nm-status-confirm').once().on('click', function () {

        $('input[type=submit]#edit-status-submit').click();
      });

      // Hide everything on body click
      $('body').once().on('click', function (event) {
        if ($(event.target).closest('#nm-table').length === 0) {
          hideContent();
          $('tr.row-options').insertAfter($('tr#nm-hidden-content'));
        }
      });

      $(document).ready(function () {
        // Move form elements into table.
        // Move buttons for options into button row of table.
        $('.js-place-btn-row').each(function () {
          var nid = $(this).data('nid');
          if(!$(this).hasClass('js-placed')) {
            $('div#row-buttons-' + nid).prepend(this);
            $(this).addClass('js-placed');
          }
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