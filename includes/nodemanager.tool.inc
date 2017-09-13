<?php

// Include files.
module_load_include('inc', 'nodemanager', 'includes/nodemanager.common.buttons');
module_load_include('inc', 'nodemanager', 'includes/nodemanager.helpers');

/**
 * This is the main function for the node management tool.
 *
 * This function contains the main query for nodes and builds
 * a table filled with information. Each row has attached
 * options that offer different functionality regarding
 * the given node.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array $form
 */
function nodemanager_form($form, &$form_state) {

  // global $user;

  // Make sure that the AJAX library is available.
  $form['#attached']['library'][] = array('system', 'drupal.ajax');

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'nodemanager') . '/css/nodemanager.css',
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'nodemanager') . '/js/nodemanager.js',
  );

  // Safe for nid.
  // We make this a hidden field, so we can modify it with jQuery.
  $form['nid'] = array(
    '#type' => 'hidden',
    '#default_value' => 0,
  );

  // Header.
  $form['header'] = array(
    '#markup' => '<h1 class="centered">' . t('Nodemanager') . '</h1>',
  );

  // Set table header.
  $header = array(
    array('data' => t('Created'), 'class' => array('short-column')),
    array('data' => t('Title'), 'class' => array('long-column')),
    array('data' => t('Author'), 'class' => array('medium-column')),
    array('data' => '', 'class' => array('options-column')),
  );

  // Query database for nodes.
  // At the moment we use a node type called 'project'.
  // We will later add functionality to administer this.
  // Limit range to 10 for development.
  $query = db_select('node', 'n')->fields('n')->extend('TableSort');
  $query->condition('n.type', 'project')->range(0, 10);
  $query->orderByHeader($header)->orderBy('n.created', 'DESC');

  // Execute query.
  $entries = $query->execute()->fetchAll();

  // Initialize variables.
  $buttons = '';
  $rows = array();
  $hidden = '';
  $hidden_options = '';

  // Use this container to hide options on page load. Prefix.
  $form['hide_prefix'] = array('#markup' => '<div class="element-invisible">');

  // We iterate over all query results.
  // Might have to work on variable names here.
  foreach ($entries as $entry) {

    // Node id und Author id.
    $nid = $entry->nid;
    $uid = $entry->uid;

    // Fetch username of author
    $author = nodemanager_helpers_get_author($uid);

    // Fill row.
    $rows[] = array(
      'data' => array(
        array('data' => date("m.d.Y", $entry->created)),
        array('data' => l(check_plain($entry->title), '/node/' . $nid)),
        array('data' => l($author, '/user/' . $uid)),
        array('data' => '<div id="row-buttons-' . $nid . '" class="options"></div>'),
      ),
      'id' => array('row-' . $nid),
    );

    /* Here we define different options as form elements. These form
    elements will be moved into the table by jQuery on document ready.
    The identifier is the id of the container. */

    // Button to inspect node.
    // We need to call youvomanager_common_submits('inspect_submit') later!
    $buttons .= nodemanager_common_buttons('inspect', $nid);

    // Button for editing options.
    $buttons .= nodemanager_common_buttons('edit', $nid);

    /* We will use this hidden container to store information that
    should be displayed when a user clicks an option. The display
    is triggered by jQuery. */
    $hidden .= '<tr id="content-row-' . $nid . '" class="content-row">
			<td colspan="4"></td>
		</tr>';

    /* We will use this hidden container to store futher options that
    should be displayed when a user clicks an option. The display
    is triggered by jQuery. */
    $hidden_options .= '<tr class="border-v row-options" id="edit-row-' . $nid . '">
			<td colspan="3"></td>
			<td colspan="1">
				<span class="left">
					<a href="/node/' . $nid . '/edit" target="_blank">' . t('Edit node') . '</a><br />
					<a href="/node/' . $nid . '/delete" target="_blank">' . t('Delete node') . '</a>
				</span>
			</td>
		</tr>';
  }

  // Accumulate buttons.
  // Here we deliver the buttons to be rendered.
  $form['buttons'] = array('#markup' => $buttons);

  // Here we call the hidden submit functions!
  // These are not displayed and triggered by there respective buttons via jQuery.
  $form['inspect_submit'] = nodemanager_common_submits('inspect');

  // Use this container to hide options on page load. Suffix.
  $form['hide_suffix'] = array('#markup' => '</div>');

  // Draw table.
  $form['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('nm'),
      'id' => array('nm-table'),
    ),
    '#header' => !empty($rows) ? $header : array(),
    '#rows' => $rows,
    '#empty' => t('Nothing to display here!'),
  );

  // This form element holds the hidden objects.
  // The first row is used to append content to this table with jQuery.
  $form['hidden'] = array(
    '#markup' => '<table style="display:none;">
				<tr id="nm-hidden-content">
				</tr>' .
      $hidden .
      $hidden_options .
      '</table>',
  );

  return $form;
}

/**
 * AJAX callback to inspect node.
 * At the moment we load the full view of a node.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array $form
 */
function nodemanager_common_ajax_inspect($form, &$form_state) {

  $nid = $form_state['values']['nid'];

  if ($node = node_load($nid)) {
    $node_view = node_view($node, 'full');
    $node_markup = drupal_render($node_view);
    $command = array();
    $command[] = ajax_command_invoke(NULL, 'customAttach', array($nid, $node_markup, 1));
    $command[] = array('command' => 'nodeManagerReattach');

    return array(
      '#type' => 'ajax',
      '#commands' => $command,
    );
  } else {
    $command = array();
    $command[] = ajax_command_invoke('#row-buttons-' . $nid . ' span', 'hide');
    $command[] = ajax_command_append('#row-buttons-' . $nid, '<span class="left bold" style="margin: 0 10px;color:red;line-height:15px;">' . t('Error!') . '</span>');
    $command[] = array('command' => 'nodeManagerReattach');

    return array(
      '#type' => 'ajax',
      '#commands' => $command,
    );
  }
}