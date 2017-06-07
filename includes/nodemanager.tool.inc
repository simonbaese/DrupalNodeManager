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
	// Limit range to 10 for development.
	$query = db_select('node','n')->fields('n')->extend('TableSort');
	$query->condition('n.type','project')->range(0,10);
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
		
		// Node id und Author id.
		$nid = $entry->nid;
		$uid = $entry->uid;
		
		// Fetch username of author
		$query = db_select('users','u')->fields('u',array('name'));
		$query->condition('u.uid',$entry->uid);
		$author = $query->execute()->fetchField();
		
		// Fill row.
		$rows[] = array(
			'data' => array(
				array('data' => date("m.d.Y",$entry->created)),
				array('data' => l(check_plain($entry->title),'/node/'.$nid)),
				array('data' => l(check_plain($author),'/user/'.$uid)),
				array('data' => '<div id="row-buttons-'.$nid.'" class="options"></div>'),
				),
			'id' => array('row-'.$nid),
		);
		
		/* Here we define different options as form elements. These form 
		elements will be moved into the table by jQuery on document ready. 
		The identifier is the id of the container. */
		
		// Button to inspect node.
		$form['inspect_'.$nid] = array(
			'#type' => 'submit',
			'#name' => 'inspect_'.$nid,
			'#value' => $nid,
			'#prefix' => '<span id="option-inspect-id-'.$nid.'" data-nid="'.$nid.'">',
			'#suffix' => '</span>',
			'#ajax' => array(
				'callback' => 'nodemanager_options_inspect_ajax',
				'effect' => 'none', 
				'progress' => array('type' => 'none'),
				'event' => 'click',
			),
			'#attributes' => array('class' => array('arrow-down-btn')),
			'#limit_validation_errors' => array(),
			'#submit' => array(),	
		);
		
		/* We will use this hidden container to store information that 
		should be displayed when a user clicks an option. The display
		is triggered by jQuery. */
		$hidden .= '<tr id="content-row-'.$entry->nid.'">
			<td colspan="4"></td>
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
			'class' => array('nm'),
			'id' => array('nodemanager-table'),
		),
		'#header' => !empty($rows) ? $header : array(),
		'#rows' => $rows,
		'#empty' => t('Nothing to display here!'),
	);
	
	// This form element holds the hidden objects.
	$form['hidden'] = array(
		'#markup' => '<table style="display:none;">
				<tr id="nm-hidden-content">
				</tr>'.
				$hidden.
				$hiddenoptions.
			'</table>',
	);

	return $form;
}

/**
 * AJAX callback to inspect node.
 * At the moment we load the full view of a node.
 */
function nodemanager_options_inspect_ajax($form, &$form_state){
  
	$nid = $form_state['triggering_element']['#value'];
	
	// Check if node can be loaded
	if($node = node_load($nid)){  	
		// We get node view here and render markup.
		$nodeview = node_view($node,'full');
		$nodemarkup = drupal_render($nodeview);
		// Return AJAX response.
		// Use custom attach function to move rendered node into table.
		// We need to reattach Drupal behaviors. Otherwise some functionality might be blocked.
		return array(
			'#type' => 'ajax',
			'#commands' => array(
				ajax_command_invoke(NULL, 'customAttach', array($nid, $nodemarkup)),
				array('command' => 'nodeManagerReattach'),
			),
		);		
		
	// If node can not be loaded return error.
	} else {
		// Return AJAX response.
		// Hide options row and display error.
		// We need to reattach Drupal behaviors. Otherwise some functionality might be blocked.
		return array(
			'#type' => 'ajax',
			'#commands' => array(		
				ajax_command_invoke('#row-buttons-'.$nid.' span','hide'),
				ajax_command_append('#row-buttons-'.$nid, '<span class="error">'.t('Error!').'</span>'),
				array('command' => 'nodeManagerReattach'),
			),
		);
	}

}