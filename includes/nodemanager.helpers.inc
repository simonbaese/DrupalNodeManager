<?php

/**
 * Query for user name with given user id.
 *
 * @param integer $uid
 *   The user id.
 *
 * @return string
 *   Name of user.
 */
function nodemanager_helpers_get_name($uid) {

  $query = db_select('users', 'u')->fields('u', array('name'));
  $query->condition('u.uid', $uid);

  return check_plain($query->execute()->fetchField());
}

/**
 * Compile email header.
 *
 * @param integer $uid
 *   The user id.
 *
 * @return array
 *   Array with email header information.
 */
function nodemanager_helpers_get_email_header($uid) {

  $name = nodemanager_helpers_get_name($uid);
  $email = nodemanager_helpers_get_email($uid);

  return array(
    'Sender' => $name . ' <' . $email . '>',
    'Return-Path' => $email,
    'From' => $name . ' <' . $email . '>',
  );
}

/**
 * Query for user email with given user id.
 *
 * @param integer $uid
 *   The user id.
 *
 * @return string
 *   Email of user.
 */
function nodemanager_helpers_get_email($uid) {

  $query = db_select('users', 'u')->fields('u', array('mail'));
  $query->condition('u.uid', $uid);

  return check_plain($query->execute()->fetchField());
}

/**
 * Builds simple error command.
 *
 * @param $nid
 *   Node identifier.
 *
 * @return array
 *   Ajax command.
 */
function nodemanager_helpers_ajax_attach_error($nid) {
  $command = array();
  $command[] = ajax_command_invoke('#row-buttons-' . $nid . ' span', 'hide');
  $command[] = ajax_command_append('#row-buttons-' . $nid, '<span class="left bold" style="margin: 0 10px;color:red;line-height:15px;">' . t('Error!') . '</span>');
  $command[] = array('command' => 'nodeManagerReattach');

  return $command;
}