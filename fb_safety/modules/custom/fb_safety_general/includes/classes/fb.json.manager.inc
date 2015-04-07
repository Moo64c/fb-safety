<?php
/**
 * @file
 * Class file to parse Hiro JSON requests.
 */

class HiroJsonManager {
  private $logger;
  private $parsers;
  private $requestors;
  private $nodes;
  // Hold on to cookies to prevent repeated login requests.
  private $cookies;
  private $date;

  /**
   * Create a new process manager.
   *
   * @param array|bool $node_ids
   *   Node IDs to process.
   * @param int $range
   *   Amount of media sellers to process if $nids is empty.
   * @param array $parsers
   *  Array of parsers to use (by type).
   */
  public function __construct($node_ids = FALSE, $range = 20, $parsers = array()) {
    // Dependency injection.
    $this->logger = $logger ? $logger : new HiroLogger();
    $this->parsers = $parsers + array(
      'media_seller' => new HiroParserJsonMediaSeller($this->logger),
      'product' => new HiroParserJsonProduct($this->logger),
      'publisher' => new HiroParserJsonPublisher($this->logger),
      'ad_network' => new HiroParserJsonAdNetwork($this->logger),
    );

    $this->servers = array();
    $this->requestors = array();
    $this->date = $date ? $date : new HiroDate($this->logger);

    // Check for media seller IDs to process.
    if (!$node_ids) {
      if (!$node_ids = hiro_get_media_sellers_nids($range)) {
        $this->logger->addRow('No media sellers found', WATCHDOG_INFO);
        $this->logger->send();
        return;
      }
    }

    // Hold node info in the manager.
    $this->nodes = node_load_multiple($node_ids);
    $this->logger->addRow("Node IDs to process: " . var_export($node_ids, TRUE), WATCHDOG_INFO);

    $this->cookies = array();
    // Create requestors.
    foreach ($node_ids as $nid) {
      $requestor = new HiroRequestJson($this->logger, $this->nodes[$nid]);
      $this->validateCookie($requestor, $nid);
      // Requestor object is ready.
      $this->requestors[$nid] = $requestor;
    }
  }

  /**
   * Perform process.
   */
  public function process() {
    if (!$this->checkAccess()) {
      // Check that processing is enabled.
      return $this->logger;
    }

    foreach ($this->nodes as $nid => $node) {
      $process_data = array();
      $requestor = $this->requestors[$nid];

      if (!$requestor->isLoggedIn()) {
        $this->logger->addRow("Failed logging in, skipping this media seller.", WATCHDOG_INFO, $nid, "login", $requestor->businessEntityId);
        continue;
      }
      // All process types except publisher.
      $media_seller_message = NULL;
      $process_types = array('media_seller', 'product', 'ad_network');
      foreach ($process_types as $type) {
        $parser = $this->parsers[$type];
        $data = $requestor->performRequest($this->date, $type);
        $this->logger->addRow("Response: " . var_export($data, TRUE), WATCHDOG_INFO, $nid, $type);
        $process_data[$type] = $parser->extractData($data, $nid);

        // Create new messages.
        foreach ($process_data[$type] as $data_row) {
          $message = hiro_message_create($type, $nid, $data_row, $this->logger, $this->date->get("timestamp"));
          if ($type == 'media_seller') {
            $media_seller_message = $message;
          }
        }
      }

      // Perform a request for each of the publishers.
      foreach (hiro_get_publishers_from_media_seller($nid, $this->logger) as $publisher_id) {
        $type = 'publisher';
        if (!$data = $requestor->performRequest($this->date, $type, $publisher_id)) {
          continue;
        }
        $process_data[$type][$publisher_id] = $this->parsers[$type]->extractData($data, $nid, $publisher_id);

        // Create new messages.
        foreach ($process_data[$type][$publisher_id] as $data_row) {
          hiro_message_create($type, $nid, $data_row, $this->logger, $this->date->get("timestamp"));
        }
      }

      $this->alterMediaSellerData($process_data, $media_seller_message);

    }
    $this->logger->send();
    return $this->logger;
  }

  /**
   * Get the process types' names.
   *
   * @return array
   */
  public function getProcessTypes() {
    return array(
      'media_seller',
      'ad_network',
      'product',
      // Publisher is requested per publisher ID.
      'publisher',
    );
  }

  /**
   * Make sure a requestor is logged in and has a cookie string.
   *
   * @param $requestor
   *   Requestor to check.
   * @param int $nid
   *   Node ID to check for.
   */
  private function validateCookie($requestor, $nid = NULL) {
    if ($nid && !$this->checkQueryRange($nid, $this->date->get("timestamp"))) {
      // Date range is irrelevant to the node.
      $this->logger->addRow("Date is out of display range for node $nid, skipping.", WATCHDOG_INFO, $nid, "media_seller", $requestor->businessEntityId);
      unset($this->nodes[$nid]);
      unset($this->requestors[$nid]);
      return;
    }
    // Get a cookie if already exists.
    $cookie_id = serialize($requestor->getCookieCredentials());
    $cookie = empty($this->cookies[$cookie_id]) ? NULL : $this->cookies[$cookie_id];

    // Set the saved cookie to the requestor (or NULL if one doesn't exist).
    $requestor->setCookie($cookie);
    // Login / acquire relevant cookie.
    $this->cookies[$cookie_id] = $requestor->login();
  }

  /**
   * Adds "unmapped" fields' values.
   *
   * @param $process_data
   *  Parsed data from server.
   * @param $media_seller_message
   *   Message to alter.
   */
  private function alterMediaSellerData($process_data, $media_seller_message) {
    if (empty($media_seller_message)) {
      return;
    }

    $mapped_placements = 0;
    $mapped_impressions = 0;
    foreach ($process_data['publisher'] as $publisher_data) {
      // Use the first row.
      $publisher_data = reset($publisher_data);
      // Sum impressions and placements.
      $mapped_placements += $publisher_data['field_count_placement'];
      $mapped_impressions += $publisher_data['field_count_impression'];
    }
    // Plug unmapped impressions and placements into the media seller message.
    $message_wrapper = entity_metadata_wrapper('message', $media_seller_message);
    $message_wrapper->field_count_unmap_placement->set($message_wrapper->field_count_placement->value() - $mapped_placements);
    $message_wrapper->field_count_unmap_impression->set($message_wrapper->field_count_impression->value() - $mapped_impressions);
    $message_wrapper->save();
  }

  /**
   * Check a query range for a node.
   *
   * @param $nid
   * @return bool
   */
  private function checkQueryRange($nid) {
    $date_start = $this->date->get("timestamp");
    $date_end = $this->date->get("last second");
    return hiro_query_range_is_relevant($nid, $date_start, $date_end);
  }

  /**
   * Checks if processing is enabled.
   *
   * @return bool
   *  Whether we're allowed to process.
   */
  protected function checkAccess() {
    // Check that processing is enabled on the site.
    if (!variable_get('hiro_process_data', TRUE)) {
      $this->logger->addRow('Processing data is disabled, exiting.', WATCHDOG_WARNING);
      $this->logger->send();
      return FALSE;
    }
    return TRUE;
  }
}
