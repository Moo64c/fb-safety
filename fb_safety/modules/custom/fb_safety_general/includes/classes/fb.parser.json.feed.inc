<?php
/**
 * Created by PhpStorm.
 * User: amir
 * Date: 23/02/15
 * Time: 4:50 PM
 */

class FbParserJsonFeed extends FbParserJsonBase {

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

    foreach($data['data'] as $row) {
      $row_data = array();
      foreach($this->map as $key => $values) {
        // Use the header value to assign a field name.
        $filtered_data = $this->filter($values, !empty($row[$key]) ? $row[$key] : "");
        if (!empty($values["add_row"])) {
          foreach($filtered_data as $row) {
            $return_data[] = $row;
          }
        }
        else {
          $row_data[$values['field']] = $filtered_data;
        }
      }
      $return_data[] = $row_data;
    }

    return $return_data;
  }
  protected function map() {
    return array(
      'id' => array(
        'field' => 'event_id',
        "filter" => "text",
      ),
      'message' => array(
        'field' => 'data',
        "filter" => "text",
      ),
      'from' => array(
        'field' => 'user_id_from',
        "filter" => "from_id",
      ),
      'created_time' => array(
        'field' => 'created_time',
        "filter" => "time",
      ),
      'updated_time' => array(
        'field' => 'updated_time',
        "filter" => "time",
      ),
      'type' => array(
        'field' => 'event_type',
        'filter' => 'type_selector',
      ),
      'likes' => array(
        'field' => 'likeAmount',
        'filter' => 'data_amount',
      ),
      'comments' => array(
        'field' => 'commentAmount',
        'filter' => 'feed_comments',
        'add_row' => TRUE,
      ),
    );
  }
}
