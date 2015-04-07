<?php
/**
 * @file
 * Class file to parse Hiro JSON requests.
 */

interface FbParserInterface {

  /**
   * Attempt to parse.
   *
   * @param String $data_string Encoded / non parse data string.
   * @param int $nid Node ID
   * @return array Array of the parse information that fit the field schema.
   * Array of the parse information that fit the field schema.
   */
  public function extractData($data_string, $nid = NULL);

  /**
   * Whether if the parsing has had an error.
   * @return boolean
   */
  public function hasError();
}
