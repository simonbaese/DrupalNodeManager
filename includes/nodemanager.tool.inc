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
		array('data' => t('Events'), 'class' => array('short-column')),
		array('data' => t('Title'), 'class' => array('long-column')),
		array('data' => t('Author'), 'class' => array('medium-column')),
		array('data' => '', 'class' => array('options-column')),
	);

	// Query database for nodes.
	// At the moment we use a node type called 'project'. We will later make this administerable.
	$query = db_select('node','n')->fields('n')->extend('TableSort');
	$query->condition('n.type','project');
	$query->orderByHeader($header)->orderBy('n.created','DESC');
	
	// Execute query.
	$entries = $query->execute()->fetchAll();
	
	// Initialize rows.
	$rows = array();
	$hidden = '';
	$hiddenoptions = '';
	
	// We iterate over all query results.
	// Might have to work on variable names here.
	foreach($entries as $entry) {
		
		// Fetch username of author
		$query = db_select('users','u')->fields('u',array('name'));
		$query->condition('u.uid',$entry->uid);
		$author = $query->execute()->fetchField();
		
		// Fill row.
		$rows[] = array(
			'data' => array(
				array('data' => date("m.d.Y",$entry->created)),
				array('data' => l(check_plain($entry->title),'/node/'.$entry->nid)),
				array('data' => l(check_plain($author),'/user/'.$entry->uid)),
				array('data' => 'Display options here'),
				),
			'id' => array('row-'.$entry->nid),
		);
		
		/* We will use this hidden container to store information that 
		should be displayed when a user clicks an option. The display
		is triggered by jQuery. */
		$hidden .= '<tr id="content-row-'.$entry->nid.'">
			<td></td>
		</tr>';
		
		/* We will use this hidden container to store futher options that 
		should be displayed when a user clicks an option. The display
		is triggered by jQuery. */
		$hiddenoptions .= '<tr class="row-options" id="option1-row-'.$nid.'">
			<td>
			</td>
		</tr>';
	}
	
	// Draw table. 
	$form['table'] = array(
		'#theme' => 'table',
		'#attributes' => array(
			'class' => array('nodemanager-table'),
			'id' => array('nodemanager-table'),
		),
		'#header' => !empty($rows) ? $header : array(),
		'#rows' => $rows,
		'#empty' => t('Nothing to display here!'),
	);
	
	// This form element holds the hidden objects.
	$form['hidden'] = array(
		'#markup' => '<table style="display:none;">
				<tr id="operator-hidden-content">
				</tr>'.
				$hidden.
				$hiddenoptions.
			'</table>',
	);

	return $form;
}