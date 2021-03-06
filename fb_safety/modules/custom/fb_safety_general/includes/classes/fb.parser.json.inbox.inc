<?php
/**
 * Created by PhpStorm.
 * User: amir
 * Date: 23/02/15
 * Time: 4:50 PM
 */

class FbParserJsonInbox extends FbParserJsonBase {

  /**
   * Create a new parser.
   */
  public function __construct() {
    parent::__construct();
    $this->map = $this->map();
    $this->type = "inbox";
  }

  /**
   * @param String $data_string
   * @param null $nid
   * @return array
   */
  public function extractData($data_string, $nid = NULL) {
    $data = parent::extractData($data_string, $nid);
    $return_data = array();

    foreach($data['data'] as $rows) {
      if (empty($rows['comments'])) {
        continue;
      }
      $rows = $rows['comments']['data'];
      $this->extractComments($rows, $return_data);
    }
    return $return_data;
  }

  public function extractComments($rows, &$return_data) {
    // Convert the data to an array.
    foreach($rows as $row) {
      $row_data = array();
      foreach($this->map as $key => $values) {
        // Use the header value to assign a field name.
        $row_data[$values['field']] =  $this->filter($values, !empty($row[$key]) ? $row[$key] : "");
      }
      $return_data[] = $row_data;
    }
  }

  protected function map() {
    return array(
      'id' => array(
        'field' => 'event_id',
        "filter" => "text",
      ),
      'from' => array(
        'field' => 'user_id_from',
        "filter" => "from_id",
      ),
      'message' => array(
        'field' => 'data',
        "filter" => "text",
      ),
      'created_time' => array(
        'field' => 'created_time',
        "filter" => "time",
      ),
      'event_type' => array(
        'field' => 'event_type',
        'filter' => 'force_value',
        'value' => 'PRIVATE_MESSAGE_EVENT_TYPE',
      ),
    );
  }
}
