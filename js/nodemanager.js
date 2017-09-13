/* jQuery of Nodemanager */

(function ($) {

  $.fn.customAttach = function (nid, nodeview, rotate) {

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
      $('tr#content-row-' + nid + ' td').append(nodeview);
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

  Drupal.Ajax.prototype.commands.nodeManagerReattach = function (ajax, response, status) {
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
          hideContent();
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