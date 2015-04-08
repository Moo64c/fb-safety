<?php
/**
 * @file
 * Class file to parse Hiro JSON requests.
 */

abstract class FbParserJsonBase implements FbParserInterface {
  private $hasError;
  protected $map;
  protected $type;

  public function __construct() {
    $this->hasError = FALSE;
    $this->map = $this->map();
    $this->type = "none";
  }

  /**
   * Parse the data.
   * @param String $data_string Encoded / non parse data string.
   * @param int $nid "Child" Node ID
   * @return array Array of the parse information that fit the field schema.
   * Array of the parse information that fit the field schema.
   */
  public function extractData($data_string, $nid = NULL) {
    $return_data = array();
    // Reset error status.
    $this->hasError = FALSE;
    // Decodes to arrays instead of classes as in json_decode().
    if (!$data = json_decode($data_string)) {
      $this->hasError = TRUE;
      return $return_data;
    }

    // Check for errors and problems in the data.
    if ($this->checkError($data, $nid)) {
      return $return_data;
    }
    $data = hiro_object_to_array($data);
    foreach($data['data'] as $rows) {
      if (empty($rows['comments'])) {
        continue;
      }
      $rows = $rows['comments']['data'];

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

    return $return_data;
  }

  /**
   * Maps the parsed data to field names.
   */
  protected function map() {
    return array();
  }

  /**
   * Send data to be filtered after parsing.
   * @param $filter_type
   * @param $data
   * @return mixed
   */
  protected function filter($filter_type, $data) {
    $filter = $filter_type['filter'];
    $function_name  = "fb_safety_general_data_filter_$filter";
    if ($filter == "force_value") {
      return $filter_type['value'];
    }
    return function_exists($function_name) ? check_plain($function_name($data)) : check_plain($data);
  }

  /**
   * Whether if the parsing has had an error.
   * @return boolean
   */
  public function hasError() {
    return $this->hasError;
  }

  /**
   * Check an HTTP response for possible errors before parsing.
   * @param $data Object
   *   Http response object.
   * @param $nid
   *   Node ID.
   * @return bool
   *   Whether the response is OK to parse.
   */
  public function checkError($data, $nid) {
    //TODO
    return FALSE;
  }
}

function hiro_object_to_array($object) {
  if (is_object($object)) {
    $object = get_object_vars($object);
    return hiro_object_to_array($object);
  }
  if (is_array($object)) {
    foreach ($object as $object_name => $object_variable) {
      $object[$object_name] = hiro_object_to_array($object_variable);
    }
  }
  return $object;
}
