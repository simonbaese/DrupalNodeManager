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

  // Load node from nid.
  if ($node = node_load($nid)) {
    $node_view = node_view($node, 'full');
    $node_markup = drupal_render($node_view);
    $command = array();
    $command[] = ajax_command_invoke(NULL, 'customAttach',
      array($nid, $node_markup, 1));
    $command[] = array('command' => 'nodeManagerReattach');

    return array(
      '#type' => 'ajax',
      '#commands' => $command,
    );
  }
  // If node can not be loaded display error.
  else {

    return array(
      '#type' => 'ajax',
      '#commands' => nodemanager_helpers_ajax_attach_error($nid),
    );
  }
}

/**
 * AJAX callback to send email.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array $form
 */
function nodemanager_ajax_email($form, &$form_state) {

  $nid = $form_state['values']['nid'];

  // Load node from nid.
  if ($node = node_load($nid)) {

    // Get text
    $text = $form_state['values']['text'];

    // Check whether email will have content
    if (empty($text)) {

      // Set form error
      $command = array();
      $command[] = ajax_command_invoke('#edit-text', 'addClass', array('error'));
      $command[] = array('command' => 'nodeManagerReattach');
    }

    // If content is not empty send email to author
    else {

      // Get current user
      global $user;

      // Get information for email
      $email = nodemanager_helpers_get_email($node->uid);
      $subject = t('Message from Nodemanager');

      // Send email to organisation
      drupal_mail('nodemanager', 'nodemanager_mail', check_plain($email),
        language_default(), array(
          'subject' => $subject,
          'body' => array($text),
          'headers' => nodemanager_helpers_get_email_header($user->uid),
        ), NULL);

      $command = array();
      $command[] = ajax_command_invoke('.nm-email-confirm', 'append',
        array('<span id="remove-me">&nbsp;OK!</span>'));
      $command[] = ajax_command_invoke(NULL, 'nodeManagerFadeOut', array('tr#email-row'));
      $command[] = array('command' => 'nodeManagerReattach');
    }

    return array(
      '#type' => 'ajax',
      '#commands' => $command,
    );
  }
  // If node can not be loaded display error.
  else {

    return array(
      '#type' => 'ajax',
      '#commands' => nodemanager_helpers_ajax_attach_error($nid),
    );
  }
}

/**
 * AJAX callback to publish node.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array $form
 */
function nodemanager_ajax_status($form, &$form_state) {

  $nid = $form_state['values']['nid'];

  // Load node from nid.
  if ($node = node_load($nid)) {

    $command = array();
    $command[] = ajax_command_invoke(NULL, 'nodeManagerFadeOut', array('tr#status-row-' . $nid));

    if ($node->status) {
      $node->status = 0;
      $command[] = ajax_command_invoke('div.nm-status[data-nid=' . $nid . ']', 'removeClass', array('nm-status-true'));
      $command[] = ajax_command_invoke('div.nm-status[data-nid=' . $nid . ']', 'addClass', array('nm-status-false'));
    }
    else {
      $node->status = 1;
      $command[] = ajax_command_invoke('div.nm-status[data-nid=' . $nid . ']', 'removeClass', array('nm-status-false'));
      $command[] = ajax_command_invoke('div.nm-status[data-nid=' . $nid . ']', 'addClass', array('nm-status-true'));
    }

    node_save($node);
    $command[] = array('command' => 'nodeManagerReattach');

    return array(
      '#type' => 'ajax',
      '#commands' => $command,
    );
  }
  // If node can not be loaded display error.
  else {

    return array(
      '#type' => 'ajax',
      '#commands' => nodemanager_helpers_ajax_attach_error($nid),
    );
  }
}

/**
 * AJAX callback to promote node.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array $form
 */
function nodemanager_ajax_promote($form, &$form_state) {

  $nid = $form_state['values']['nid'];

  // Load node from nid.
  if ($node = node_load($nid)) {

    $command = array();
    $command[] = ajax_command_invoke(NULL, 'nodeManagerFadeOut', array('tr#promote-row-' . $nid));

    if ($node->promote) {
      $node->promote = 0;
      $command[] = ajax_command_invoke('div.nm-promote[data-nid=' . $nid . ']', 'removeClass', array('nm-promote-true'));
      $command[] = ajax_command_invoke('div.nm-promote[data-nid=' . $nid . ']', 'addClass', array('nm-promote-false'));
    }
    else {
      $node->promote = 1;
      $command[] = ajax_command_invoke('div.nm-promote[data-nid=' . $nid . ']', 'removeClass', array('nm-promote-false'));
      $command[] = ajax_command_invoke('div.nm-promote[data-nid=' . $nid . ']', 'addClass', array('nm-promote-true'));
    }

    node_save($node);
    $command[] = array('command' => 'nodeManagerReattach');

    return array(
      '#type' => 'ajax',
      '#commands' => $command,
    );
  }
  // If node can not be loaded display error.
  else {

    return array(
      '#type' => 'ajax',
      '#commands' => nodemanager_helpers_ajax_attach_error($nid),
    );
  }
}