<?php

/**
 * Common trigger buttons.
 *
 * @param $type
 *   Type of requested button.
 * @param $nid
 *   Node identifier.
 *
 * @return string
 *   Holds markup for button.
 */
function nodemanager_common_buttons($type, $nid) {

  return '<span id="nm-' . $type . '-id-' . $nid . '" data-nid="' . $nid . '" class="nm-' . $type . ' js-place-btn-row"></span>';
}

/*
 * Common submit buttons.
 *
 * @param $type
 *   Type of requested submit button.
 *
 * @return array
 *   Array that will render submit button with Drupal Form API.
 */
function nodemanager_common_submits($type) {

  return array(
    '#type' => 'submit',
    '#name' => $type . '_submit',
    '#value' => $type . '_submit',
    '#ajax' => array(
      'callback' => 'nodemanager_ajax_' . $type,
      'effect' => 'none',
      'progress' => array('type' => 'none'),
      'event' => 'click', // This is important to be able to trigger by jQuery!
    ),
    '#attributes' => array('class' => array('element-invisible')),
    '#limit_validation_errors' => array(array('nid')),
    '#submit' => array(),
  );
}