<?php
/**
 * Created by PhpStorm.
 * User: amir
 * Date: 07/04/15
 * Time: 11:04 PM
 */

class fb_raw_data_pusher {
  private $table;

  public function __construct($table) {
    db_query("SET NAMES utf8mb4");
    $this->table = $table;
  }

  public function push_data($data) {
    foreach ($data as $row) {
      // FIXME
      $time = time();
      $default_values = array(
        'event_id'      => -1,
        'user_id_to'    => -1,
        'user_id_from'  => -1,
        'event_type'    => 'STATUS_EVENT_TYPE',
        'created_time'  => $time,
        'updated_time'  => $time,
        'likeAmount'    => 0,
        'commentAmount' => 0,
        'data'  => "",
      );

      $query = db_insert($this->table);
      $row += $default_values;
      $query->fields($row);
      $query->execute();
    }
  }
}
