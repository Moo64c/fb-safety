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
    // Reset error status.
    $this->hasError = FALSE;
    // Decodes to arrays instead of classes as in json_decode().
    if (!$data = json_decode($data_string)) {
      $this->hasError = TRUE;
      return;
    }

    // Check for errors and problems in the data.
    if ($this->checkError($data, $nid)) {
      return;
    }
    $data = hiro_object_to_array($data);
    return $data;
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
    $return_value = function_exists($function_name) ? $function_name($data) : $data;
    return is_array($return_value) ? $return_value : check_plain($return_value);

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
