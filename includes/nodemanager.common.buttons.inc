<?php 

/**
 * Common trigger buttons.
 */
function nodemanager_common_buttons($type, $nid) {
	
	if($type == 'inspect') {
		return '<span id="nm-inspect-id-'.$nid.'" data-nid="'.$nid.'" class="nm-inspect js-place-btn-row"></span>';		
	}
	
	if($type == 'edit') {
		return '<span id="nm-edit-id-'.$nid.'" data-nid="'.$nid.'" class="nm-edit js-place-btn-row"></span>';
	}
	
	return NULL;
}

/*
 * Common submit buttons.
 */
function nodemanager_common_submits($type) {
	
	if($type == 'inspect_submit') {
		return array(
			'#type' => 'submit',
			'#name' => 'inspect_submit',
			'#value' => 'inspect_submit',
			'#ajax' => array(
				'callback' => 'youvomanager_common_ajax_inspect',
				'effect' => 'none', 
				'progress' => array('type' => 'none'),
				'event' => 'click', // This is important to be able to trigger by jQUery!
			),
			'#attributes' => array('class' => array('element-invisible')),
			'#limit_validation_errors' => array(array('nid')),
			'#submit' => array(),	
		);
	}
	
	return NULL;
}