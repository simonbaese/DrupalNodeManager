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
function nodemanager_helpers_get_author($uid) {

  $query = db_select('users', 'u')->fields('u', array('name'));
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