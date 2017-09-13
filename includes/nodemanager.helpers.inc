<?php

/*
 * Get user name of node author.
 */
function nodemanager_helpers_get_author($uid) {

  $query = db_select('users', 'u')->fields('u', array('name'));
  $query->condition('u.uid', $uid);

  return check_plain($query->execute()->fetchField());
}