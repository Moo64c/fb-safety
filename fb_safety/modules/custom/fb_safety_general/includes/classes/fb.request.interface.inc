<?php
/**
 * @file
 * Class file to create Hiro JSON requests.
 */

interface FbRequestInterface {

  /**
   *getuser returns required user info
   *$info= '' || 'inbox' || 'feed' || 'notifications'
   *'' means username
   */
  public function request($type, $last_update);
  public function extend();
}
