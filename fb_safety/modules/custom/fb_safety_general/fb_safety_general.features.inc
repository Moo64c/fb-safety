<?php
/**
 * @file
 * fb_safety_general.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fb_safety_general_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function fb_safety_general_node_info() {
  $items = array(
    'child' => array(
      'name' => t('Child'),
      'base' => 'node_content',
      'description' => t('Holds child\'s information.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
