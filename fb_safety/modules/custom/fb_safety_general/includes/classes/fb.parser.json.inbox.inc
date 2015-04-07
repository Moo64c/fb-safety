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

  protected function map() {
    return array(
      'id' => array(
        'field' => 'id_id',
        "filter" => "text",
      ),
      'from' => array(
        'field' => 'from_id',
        "filter" => "from_id",
      ),
      'message' => array(
        'field' => 'content',
        "filter" => "text",
      ),
      'created_time' => array(
        'field' => 'created_time',
        "filter" => "number",
      ),
    );
  }
}
