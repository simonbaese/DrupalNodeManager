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