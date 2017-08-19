(function ($) {
	
	$.fn.customAttach = function(nid,nodeview,rotate) {
		if($('tr#content-row-'+nid).is(':visible')) {
			$('#operator-inspect-id-'+nid).removeClass('rotate');
			$('tr#content-row-'+nid).insertAfter($('tr#operator-hidden-content'));
			$('tr#content-row-'+nid+' td').empty();
		} else {
			$('tr.content-row').insertAfter($('tr#operator-hidden-content'));
			$('span.rotate').removeClass('rotate');
			$('tr.content-row td').empty();
			$('tr#content-row-'+nid+' td').append(nodeview);
			if($('div#row-options-'+nid).is(':visible')){
				$('tr#content-row-'+nid).insertAfter($('tr#row-'+nid).next()).hide();
			} else {
				$('tr#content-row-'+nid).insertAfter($('tr#row-'+nid));
				$('tr.row-options').insertAfter($('tr#operator-hidden-content'));
			}
			$('tr#content-row-'+nid).find('td').wrapInner('<div style="display: none;" />').parent().find('td > div').slideDown(500, function(){
				var $set = $(this);
				$set.replaceWith($set.contents());
			});
			if(rotate) $('#operator-inspect-id-'+nid).addClass('rotate');
		}
	};
	
	Drupal.ajax.prototype.commands.youvoManagerReattach = function(ajax, response, status) {
		Drupal.attachBehaviors();
	};
	
	Drupal.behaviors.youvoManagerCommon = {
	  attach: function (context) {
			
			$('form input').keypress(function (e) {
				if (e.which == 13) {
					$('input[type=submit]#edit-search-submit').click();
					return false;
				}
			});

			$('.operator-log').once().on('click', function () {	
				var nid = $(this).data('nid');
				$('input[name=nid]').val(nid);
				$('input[type=submit]#edit-log-submit').click();
			});	
			
			$('.operator-inspect').once().on('click', function () {	
				var nid = $(this).data('nid');
				$('input[name=nid]').val(nid);
				$('input[type=submit]#edit-inspect-submit').click();
			});	
		
			$(document).ready(function() {
		
				// Move buttons for options into button row of table.
				$('.js-place-btn-row').each(function () {
					if($(this).attr('data-nid')) var id = $(this).data('nid');
					else var id = $(this).data('sid');
					$('div#row-buttons-'+id).prepend(this);
				}); 
				
				// Move search fields to bottom right
				$('#search').prepend($('#searchfields'));
 
			});
		}
  }
}(jQuery));
		