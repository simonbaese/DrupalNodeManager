<?php

// Include files.
module_load_include('inc', 'nodemanager', 'includes/nodemanager.helpers');
module_load_include('inc', 'nodemanager', 'includes/nodemanager.common.buttons');
module_load_include('inc', 'nodemanager', 'includes/nodemanager.ajax');

/**
 * This is the main function for the node management tool.
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

  // Add assets.
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
    array(
      'data' => '<div class="nm-header-status"></div>',
      'class' => array('icon-column'),
    ),
    array(
      'data' => '<div class="nm-header-promote"></div>',
      'class' => array('icon-column'),
    ),
    array('data' => t('Title'), 'class' => array('long-column')),
    array('data' => t('Author'), 'class' => array('medium-column')),
    array('data' => '', 'class' => array('options-column')),
  );

  // Count header elements for later use in colspan.
  $column_count = count($header);

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

    // Fetch user name of author.
    $author_name = nodemanager_helpers_get_name($uid);

    // Fill row.
    $rows[] = array(
      'data' => array(
        array('data' => date("m.d.Y", $entry->created)),
        array(
          'data' => $entry->status ?
            '<div data-nid="' . $nid . '" class="nm-status nm-status-true"></div>' :
            '<div data-nid="' . $nid . '" class="nm-status nm-status-false"></div>',
          'class' => array('centered'),
        ),
        array(
          'data' => $entry->promote ?
            '<div data-nid="' . $nid . '" class="nm-promote nm-promote-true"></div>' :
            '<div data-nid="' . $nid . '" class="nm-promote nm-promote-false"></div>',
          'class' => array('centered'),
        ),
        array('data' => l(check_plain($entry->title), '/node/' . $nid)),
        array('data' => l($author_name, '/user/' . $uid)),
        array('data' => '<div id="row-buttons-' . $nid . '" class="options"></div>'),
      ),
      'id' => array('row-' . $nid),
    );

    /* Here we define different options as form elements. These form
    elements will be moved into the table by jQuery on document ready.
    The identifier is the id of the container. */

    // Button to inspect node.
    // We need to call nodemanager_common_submits('inspect_submit') later!
    $buttons .= nodemanager_common_buttons('inspect', $nid);

    // Button for editing options.
    $buttons .= nodemanager_common_buttons('edit', $nid);

    // Button for email options.
    $buttons .= nodemanager_common_buttons('email', $nid);

    /* We will use this hidden container to store information that
    should be displayed when a user clicks an option. The display
    is triggered by jQuery. */
    $hidden .= '<tr id="content-row-' . $nid . '" class="content-row">
			<td colspan="' . $column_count . '"></td>
		</tr>';

    /* We will use this hidden container to store further options that
    should be displayed when a user clicks an option. The display
    is triggered by jQuery. */
    $hidden_options .= '<tr class="border-v row-options" id="edit-row-' . $nid . '">
			<td colspan="' . ($column_count - 1) . '"></td>
			<td colspan="1">
				<span class="left">
					<a href="/node/' . $nid . '/edit" target="_blank">' . t('Edit node') . '</a><br />
					<a href="/user/' . $uid . '/edit" target="_blank">' . t('Edit author') . '</a><br />
					<a href="/node/' . $nid . '/delete" target="_blank">' . t('Delete node') . '</a>
				</span>
			</td>
		</tr>
		<tr class="border-v row-options" id="status-row-' . $nid . '">
			<td colspan="' . ($column_count - 1) . '"></td>
			<td colspan="1">
				<div class="left">
					<div id="option-status-true">' . t('Publish node') . '</div>
					<div id="option-status-false">' . t('Un-publish node') . '</div>
					<div class="nm-status-confirm">' . t('Confirm') . '</div>
				</div>
			</td>
		</tr>
		<tr class="border-v row-options" id="promote-row-' . $nid . '">
			<td colspan="' . ($column_count - 1) . '"></td>
			<td colspan="1">
				<div class="left">  
					<div id="option-promote-true">' . t('Promote node') . '</div>
					<div id="option-promote-false">' . t('Un-promote node') . '</div>
					<div class="nm-promote-confirm">' . t('Confirm') . '</div>
				</div>
			</td>
		</tr>';
  }

  // Add text field for email option.
  $form['text'] = array(
    '#type' => 'textarea',
    '#prefix' => '<div class="left" id="nm-email-text" style="width:100%;">',
    '#suffix' => '</div>',
    '#default_value' => t('Hello there!'),
    '#rows' => 8,
  );

  // Accumulate buttons.
  // Here we deliver the buttons to be rendered.
  $form['buttons'] = array('#markup' => $buttons);

  // Here we call the hidden submit functions!
  // These are not displayed and triggered by there respective buttons via jQuery.
  $form['inspect_submit'] = nodemanager_common_submits('inspect');
  $form['status_submit'] = nodemanager_common_submits('status');
  $form['promote_submit'] = nodemanager_common_submits('promote');
  $form['email_submit'] = nodemanager_common_submits('email');

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

  // We will add further option that are independent of the
  // current row.
  $hidden_options .= '<tr class="border-v row-options" id="email-row">
			<td colspan="' . ($column_count - 1) . '" id="nm-email-option"></td>
			<td colspan="1">
				<span class="left">
					<div>' . t('Send email to author') . '</div>
					<div class="nm-email-confirm">' . t('Confirm') . '</div>
				</span>
			</td>
		</tr>';

  // This form element holds the hidden objects.
  // The first row is used to append content to this table with jQuery.
  $form['hidden'] = array(
    '#markup' => '<table style="display:none;">
      <tr id="nm-hidden-content"></tr>' .
      $hidden .
      $hidden_options . '</table>',
  );

  return $form;
}