<?php
/**
 * @file
 * Class file to create Hiro JSON requests.
 */

class FbRequestJson implements FbRequestInterface {
  public $businessEntityId;
  private $logger;
  private $node;
  private $cookie;
  private $hasError;
  private $sub_path = "/hiro-management-web/hiro/";
  private $server_name;
  private $full_server_name;

  /**
   * Constructor.
   *
   * @param $node
   *   Media seller being processed.
   * @param $cookie
   *   Cookie string, if exists.
   * @param NULL|String $server_name
   *   Server name dependency injection.
   */
  public function __construct($node, $cookie = NULL, $server_name = NULL) {
    $this->node = $node;
    $this->cookie = $cookie;
    $this->hasError = FALSE;
    $this->full_server_name = $server_name ? $server_name : ($node ?  $node->field_server_name[LANGUAGE_NONE][0]['value'] : NULL);
    $this->server_name = $this->getServerURI();
    $this->businessEntityId = intval(hiro_get_business_entity_id($this->full_server_name));
  }

  /**
   * Use an existing cookie.
   * @param $cookie
   *  Cookie string.
   */
  public function setCookie($cookie) {
    $this->cookie = $cookie;
  }

  /**
   * Check if the requestor is logged in.
   * Attempt to log in if not.
   * @return bool
   */
  public function isLoggedIn() {
    $cookie = $this->login();
    return $cookie != NULL;
  }

  /**
   * Attempt login.
   * @return string|bool
   * Cookie string if successful, FALSE otherwise.
   */
  public function login() {
    if ($this->cookie != NULL) {
      return $this->cookie;
    }

    $logger = $this->logger;
    $nid = $this->node->nid;
    $url = $this->generateUrl("login");

    $options = array(
      'method' => 'POST',
      'timeout' => variable_get('hiro_request_timeout', 300),
      'headers' => array('cookie' => $this->cookie),
      'data' => $this->generateLoginData(),
    );


    $result = $this->httpRequest($url, $options, $nid, "login", $this->businessEntityId);

    if (!$this->responseValid($result)) {
      $logger->addRow("Login failure for $this->server_name", WATCHDOG_ERROR, $nid, "login", $this->businessEntityId);
      $this->hasError = TRUE;
      return;
    }

    $data = json_decode($result->data, TRUE);
    if (empty($data['status']['success']) || $data['status']['success'] == FALSE) {
      $logger->addRow("Login failure for $this->server_name", WATCHDOG_ERROR, $nid, "login", $this->businessEntityId);
      $this->hasError = TRUE;
      return;
    }

    // This had a successful login.
    // Can be accessed only once per requestor.
    $this->hasError = FALSE;
    $logger->addRow("Login success for $this->server_name", WATCHDOG_INFO, $nid, "login", $this->businessEntityId);
    $this->cookie = $result->headers['set-cookie'];
    return $this->cookie;
  }

  /**
   * Get data from the server.
   * @param HiroDateInterface $date
   *  Date requested.
   * @param $request_type
   *  Type of request.
   * @param $unique_id
   *  Unique ID (publisher only) for the request.
   * @return array
   *  Data received from the server.
   */
  public function performRequest($request_type, $unique_id = NULL) {
    // Check log in status and attempt a retry if needed.
    if (!$this->isLoggedIn()) {
      $this->logger("Could not perform request, login failed.");
      $this->hasError = TRUE;
      return;
    }

    $nid = $this->node->nid;
    $url = $this->generateUrl("single");
    $unique_id = empty($unique_id) ? $this->businessEntityId : $unique_id;

    // Adjust date according to the server.
    $date = $date->adjustDate($nid);

    $options = array(
      'method' => 'POST',
      'timeout' => variable_get('hiro_request_timeout', 300),
      'headers' => array('cookie' => $this->cookie),
      'data' => $this->generateRequestData($date, $request_type, $unique_id),
    );

    // Get the data.
    $result = $this->httpRequest($url, $options, $nid, $request_type, $unique_id);

    if (!$this->responseValid($result, $request_type, $unique_id)) {
      $this->hasError = TRUE;
      return FALSE;
    }

    $data = $result->data;
    return $data;
  }

  /**
   * Whether if the request has an error.
   * @return boolean
   */
  public function hasError() {
    return $this->hasError;
  }

  /**
   * Helper function to extract the server path from the node.
   */
  public function getServerURI() {
    $server_name = $this->full_server_name;

    if ($server_name != '[manual]') {
      $server_matches = array();
      preg_match('/(.*)\/hiro\/HiroServlet/i', $server_name, $server_matches);

      // New servers are with an "api" prefix instead of the old ones.
      $servers_prefixes = array(
        "console1",
      );

      if (empty($server_matches[1])) {
        return FALSE;
      }
      $found =  str_replace($servers_prefixes, "api", $server_matches[1]);

      return $found;
    }
  }

  /**
   * Gets the server uri & login details for this requestor.
   * @return array
   */
  public function getCookieCredentials() {
    $server_name = $this->full_server_name;

    if ($server_name == '[manual]') {
      // Manual server - no login.
      return array(
        'uri' => $server_name,
        'username' => 'none',
        'password' => 'none',
      );
    }

    // Get the user name and password from the login field.
    preg_match('/user-name=\"(.*)\" user-password/i', $server_name, $user_matches);
    preg_match('/user-password=\"(.*)\" /i', $server_name, $password_matches);
    $username = $user_matches[1];
    $password = $password_matches[1];

    return array(
      'uri' => $server_name,
      'username' => $username,
      'password' => $password,
    );
  }

  /**
   * Helper function to create the data sent to the server.
   * @param $date
   *  Date for the request.
   * @param $request_type
   *  Request type.
   * @param null $unique_id
   *  Unique ID if relevant.
   * @return array
   *  Data to be passed to the data request.
   */
  public function generateRequestData(HiroDateInterface $date, $request_type, $unique_id = NULL) {
    // Generate data.
    $data =  array(
      'payload' => array(
        "statisticId" => $this->getHiroRequestType($request_type),
        "startPeriod" => $date->get("first second"),
        "endPeriod" => $date->tomorrow()->get("first second"),
        "extraParam" => "" . $this->businessEntityId,
        "id" => empty($unique_id) ? 0 : $unique_id,
        "dontCache" => FALSE,
      ),
      'requestHeader' => array(
        'businessEntityId' => $this->businessEntityId,
      ),
    );

    // Using normal json_encode and not the Drupal one for the unescaped slashes.
    return "request=" . json_encode($data, JSON_UNESCAPED_SLASHES);
  }

  /**
   * Helper function to generate the login data that is sent to the server.
   * @return string
   *   JSON encoded string that is required.
   */
  public function generateLoginData() {
    $data = $this->getCookieCredentials();

    // Create request according to specification.
    $data = array(
      'payload' => array(
        'userName' => $data['username'],
        'password' => $data['password'],
      ),
      'requestHeader' => array(
        'businessEntityId' => $this->businessEntityId,
      ),
    );

    // Using normal json_encode and not the Drupal one for the unescaped slashes.
    return "request=" . json_encode($data, JSON_UNESCAPED_SLASHES);
  }

  /**
   * Generate request URL.
   * @param $type
   *   Type of request; Either "login" or "single".
   * @return string
   *   Request URL.
   */
  public function generateUrl($type) {
    $type = ($type == "single") ? "report/single" : $type;

    $url = $this->server_name . $this->sub_path
        . "$type?json=true&UTF-8=true&compress=true&businessEntityId="
        . $this->businessEntityId;
    return $url;
  }

  /**
   * Determine request type.
   * @param $type
   *   Type needed.
   * @return string
   *   Hiro request type.
   */
  private function getHiroRequestType($type) {
    // Default type to pass in the url.
    switch ($type) {
      case 'publisher':
        return 'MEDIA_SELLER_PUBLISHER';
      case 'ad_network':
        return 'MEDIA_SELLER_BY_PUBLISHER_AD_NETWORKS';
      case 'product':
        return 'MEDIA_SELLER_BY_PUBLISHER_PRODUCTS';
      case 'processed':
        return 'DAILY_STATUS';
    }

    return 'MEDIA_SELLER_BY_PUBLISHER_MAIN';
  }

  /**
   * Check whether a drupal_http_request is valid.
   * Checks if the result is empty, the response code,
   * if the data is empty, or if it has "error" in it.
   * Handles logging.
   *
   * @param object $result
   *   HTTP request response.
   * @param string $type
   *   Request type.
   * @param $unique_id
   *   Relevant unique ID.
   * @return boolean
   *  FALSE on failed request; True when successful.
   */
  public function responseValid($result, $type = NULL, $unique_id = NULL) {
    $nid = empty($this->node) ? NULL : $this->node->nid;

    if (empty($result) || empty($result->code)) {
      // Failed request - empty object or no response code.
      $add_string = !empty($result->code) ? " code-".$result->code : "";
      $this->logger->addRow("HTTP request error: empty response$add_string.", WATCHDOG_ERROR, $nid);
      return FALSE;
    }

    $result_code = (int)$result->code;
    if (!empty($result->error)) {
      $this->logger->addRow('HTTP request error: ' . $result->error, WATCHDOG_ERROR, $nid);
      return FALSE;
    }

    if ($result_code < 200 || $result_code >= 300) {
      $this->logger->addRow('HTTP request error code:' . $result_code, WATCHDOG_ERROR, $nid, $type, $unique_id);
      return FALSE;
    }

    if (empty($result->data)) {
      // The data is an error.
      $this->logger->addRow('HTTP request error: response data.', WATCHDOG_ERROR, $nid, $type, $unique_id);
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Perform an HTTP request.
   * \Where the magic happens\.
   * @param $url
   *  Url to request from.
   * @param $options
   *  Header data to include.
   * @param $nid
   *  Relevant node ID.
   * @param $request_type
   *  Request type to perform.
   * @param $unique_id
   *   Unique ID to request.
   * @return Object HTTP response object.
   */
  public function httpRequest($url, $options, $nid = NULL, $request_type = NULL, $unique_id = NULL) {
    $logger = $this->logger;
    $logger->addRow('Request URL: ' . $url, WATCHDOG_INFO, $nid, $request_type, $unique_id);
    $logger->setLastRequestURI($url);
    $logger->addRow('Request parameters: ' . var_export($options['data'], TRUE), WATCHDOG_INFO, $nid, $request_type, $unique_id);
    $result = drupal_http_request($url, $options);
    $logger->addRow("Response: " . var_export($result, TRUE), WATCHDOG_INFO, $nid, $request_type, $unique_id);

    return $result;
  }
}
