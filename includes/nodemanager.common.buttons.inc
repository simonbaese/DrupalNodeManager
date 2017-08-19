<?php 

/**
 * Common trigger buttons.
 */
function nodemanager_common_buttons($type, $nid) {

	return '<span id="nm-'.$type.'-id-'.$nid.'" data-nid="'.$nid.'" class="nm-'.$type.' js-place-btn-row"></span>';		
}

/*
 * Common submit buttons.
 */
function nodemanager_common_submits($type) {
	
	return array(
		'#type' => 'submit',
		'#name' => $type.'_submit',
		'#value' => $type.'_submit',
		'#ajax' => array(
			'callback' => 'nodemanager_common_ajax_'.$type,
			'effect' => 'none', 
			'progress' => array('type' => 'none'),
			'event' => 'click', // This is important to be able to trigger by jQUery!
		),
		'#attributes' => array('class' => array('element-invisible')),
		'#limit_validation_errors' => array(array('nid')),
		'#submit' => array(),	
	);
}