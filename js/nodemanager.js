/* jQuery of Nodemanager */

(function ($) {

  $.fn.customAttach = function (nid, nodeView, rotate) {

    var contentRow = $('tr#content-row-' + nid);
    var hiddenContent = $('tr#nm-hidden-content');

    if (contentRow.is(':visible')) {
      $('#nm-inspect-id-' + nid).removeClass('rotate');
      contentRow.insertAfter(hiddenContent);
      $('tr#content-row-' + nid + ' td').empty();
    }
    else {
      $('tr.content-row').insertAfter(hiddenContent);
      $('span.rotate').removeClass('rotate');
      $('tr.content-row td').empty();
      $('tr#content-row-' + nid + ' td').append(nodeView);
      if ($('div#row-options-' + nid).is(':visible')) {
        contentRow.insertAfter($('tr#row-' + nid).next()).hide();
      }
      else {
        contentRow.insertAfter($('tr#row-' + nid));
        $('tr.row-options').insertAfter(hiddenContent);
      }
      contentRow.find('td').wrapInner('<div style="display: none;" />').parent().find('td > div').slideDown(500, function () {
        var $set = $(this);
        $set.replaceWith($set.contents());
      });
      if (rotate) {
        $('#nm-inspect-id-' + nid).addClass('rotate');
      }
    }
  };

  $.fn.nodeManagerFadeOut = function (selector) {

    $(selector).fadeOut(1195);
    setTimeout(remove, 1200);

    function remove() {
      $('#remove-me').remove();
    }
  };

  Drupal.ajax.prototype.commands.nodeManagerReattach = function (ajax, response, status) {
    Drupal.attachBehaviors();
  };

  Drupal.behaviors.nodeManager = {
    attach: function (context) {

      // Functionality for edit option
      $('.nm-edit').once().on('click', function () {
        var nid = $(this).data('nid');
        var hiddenContent = $('tr#nm-hidden-content');
        var editOptionRow = $('tr#edit-row-' + nid);

        if (editOptionRow.is(':visible')) {
          editOptionRow.insertAfter(hiddenContent);
        }
        else {
          $('tr.content-row:not(#content-row-' + nid + ')').insertAfter(hiddenContent);
          $('tr.row-options').insertAfter(hiddenContent);
          editOptionRow.insertAfter($('tr#row-' + nid));
        }
      });

      // Functionality for email option
      $('.nm-email').once().on('click', function () {
        var nid = $(this).data('nid');
        var hiddenContent = $('tr#nm-hidden-content');
        var emailOptionRow = $('tr#email-row');
        var nidVault = $('input[name=nid]');

        if (emailOptionRow.is(':visible') && nidVault.val() === nid.toString()) {
          emailOptionRow.insertAfter(hiddenContent);
        }
        else {
          emailOptionRow.show();
          $('tr.content-row:not(#content-row-' + nid + ')').insertAfter(hiddenContent);
          $('tr.row-options').insertAfter(hiddenContent);
          nidVault.val(nid);
          emailOptionRow.insertAfter($('tr#row-' + nid));
        }
      });

      // Click submit button for AJAX email send
      $('.nm-email-confirm').once().on('click', function () {
        $('input[type=submit]#edit-email-submit').click();
      });

      // Functionality for inspect option
      $('.nm-inspect').once().on('click', function () {
        var nid = $(this).data('nid');

        $('input[name=nid]').val(nid);
        $('input[type=submit]#edit-inspect-submit').click();
      });

      // Functionality for status option
      $('.nm-status').once().on('click', function () {
        var nid = $(this).data('nid');
        var hiddenContent = $('tr#nm-hidden-content');
        var statusOptionRow = $('tr#status-row-' + nid);

        if (statusOptionRow.is(':visible')) {
          statusOptionRow.insertAfter(hiddenContent);
        }
        else {
          statusOptionRow.show();
          $('tr.content-row:not(#content-row-' + nid + ')').insertAfter(hiddenContent);
          $('tr.row-options').insertAfter(hiddenContent);
          $('[id^=option-status]').css('display', 'none');
          if ($(this).hasClass('nm-status-true')) {
            $('div#option-status-false').show();
          }
          else {
            $('div#option-status-true').show();
          }
          $('input[name=nid]').val(nid);
          statusOptionRow.insertAfter($('tr#row-' + nid));
        }
      });

      // Click submit button for AJAX publish
      $('.nm-status-confirm').once().on('click', function () {
        $('input[type=submit]#edit-status-submit').click();
      });

      // Functionality for promote option
      $('.nm-promote').once().on('click', function () {
        var nid = $(this).data('nid');
        var hiddenContent = $('tr#nm-hidden-content');
        var promoteOptionRow = $('tr#promote-row-' + nid);

        if (promoteOptionRow.is(':visible')) {
          promoteOptionRow.insertAfter(hiddenContent);
        }
        else {
          promoteOptionRow.show();
          $('tr.content-row:not(#content-row-' + nid + ')').insertAfter(hiddenContent);
          $('tr.row-options').insertAfter(hiddenContent);
          $('[id^=option-promote]').css('display', 'none');
          if ($(this).hasClass('nm-promote-true')) {
            $('div#option-promote-false').show();
          }
          else {
            $('div#option-promote-true').show();
          }
          $('input[name=nid]').val(nid);
          promoteOptionRow.insertAfter($('tr#row-' + nid));
        }
      });

      // Click submit button for AJAX promote
      $('.nm-promote-confirm').once().on('click', function () {
        $('input[type=submit]#edit-promote-submit').click();
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
          if (!$(this).hasClass('js-placed')) {
            $('div#row-buttons-' + nid).prepend(this);
            $(this).addClass('js-placed');
          }
        });

        // Move email text field to option panel
        $('#nm-email-text').appendTo('td#nm-email-option');

      });

      function hideContent() {
        $('tr.content-row').insertAfter($('tr#nm-hidden-content'));
        $('span.rotate').removeClass('rotate');
        $('tr.content-row td').empty();
      }
    }
  }

}(jQuery));