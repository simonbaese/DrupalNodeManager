<?php

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
function nodemanager_ajax_inspect($form, &$form_state) {

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