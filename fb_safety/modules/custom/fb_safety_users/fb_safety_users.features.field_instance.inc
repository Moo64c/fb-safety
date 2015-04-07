<?php
/**
 * @file
 * fb_safety_users.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fb_safety_users_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'user-user-field_children'
  $field_instances['user-user-field_children'] = array(
    'bundle' => 'user',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'user',
    'field_name' => 'field_children',
    'label' => 'Children',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 7,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Children');

  return $field_instances;
}
