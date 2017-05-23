<?php 

/**
 * This is the main function for the node management tool
 */
function nodemanager_form($form, &$form_state) {
    
	// global $user;
	
	$form['#attached']['css'] = array(
	  drupal_get_path('module', 'nodemanager') . '/css/nodemanager.css',
	);	
	$form['#attached']['js'] = array(
		drupal_get_path('module', 'nodemanager') . '/js/nodemanager.js',
	);
	
	// Header.
	$form['header'] = array(
		'#markup' => '<h1 class="centered">'.t('Nodemanager').'</h1>',
	);
   
	// Set table header.
	$header = array(
		array('data' => ''),
	);
    
	// Edit rows.
	$rows = array();
	$hidden = '';
	$hiddenoptions = '';

	return $form;
}