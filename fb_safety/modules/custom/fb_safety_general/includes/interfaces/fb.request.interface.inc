<?php
/**
 * @file
 * Class file to create Hiro JSON requests.
 */

interface FbRequestInterface {

  /**
   * Attempt login.
   * @return string|bool
   * Cookie string if successful, FALSE otherwise.
   */
  public function login();


  /**
   * Check if the requestor is logged in.
   * Attempt to log in if not.
   * @return bool
   */
  public function isLoggedIn();

  /**
   * Whether if the request has an error.
   * @return boolean
   */
  public function hasError();

  /**
   * Get data from the server.
   *
   * @param $unique_id
   *  Unique ID (publisher only) for the request.
   * @param int $last_timestamp
   * @return array Data received from the server.
   * Data received from the server.
   */
  public function performRequest($unique_id, $last_timestamp = 0);

}
