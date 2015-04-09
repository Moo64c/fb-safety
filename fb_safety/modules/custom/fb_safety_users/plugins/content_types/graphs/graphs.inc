<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Events graph'),
  'description' => t('fdsafdsa'),
  'category' => t('Socia'),
  'hook theme' => 'fb_safety_users_graphs_content_type_theme',
);

/**
 * Render callback.
 */
function fb_safety_users_graphs_content_type_render($subtype, $conf, $args, $context) {
  global $user;
  $block = new stdClass();

  if(empty($user->uid)) {
    // No user.
    return;
  }
  $wrapper = entity_metadata_wrapper('user', $user);

  $events = array();
  $child_wrapper = $wrapper->field_children;
  if (empty($child_wrapper)) {
    return;
  }
  $child_events = array();
  $child_name = $child_wrapper->label();

  if (empty($child_wrapper->field_events)) {
    return;
  }

  foreach ($child_wrapper->field_events as $event_wrapper) {
    $event_id = $event_wrapper->getIdentifier();
    $date = $event_wrapper->created->value();
    $date = date('d-m-Y', $date);
    $child_events[$date][$event_id] = $event_wrapper->value();
  }
  $events[$child_name] = $child_events;

  $java_struct = new stdClass();
  $java_struct->data = new stdClass();
  $java_struct->data->json = new stdClass();
  $java_struct->bindto = "#chart";

  foreach ($events as $child_name => $child_data) {
    $row_data = array();
    foreach ($child_data as $date => $events) {
      $row_data[] = count($events);
    }
    $java_struct->data->json->{$child_name} = $row_data;
  }

  $json = json_encode($java_struct);
  $javascript_to_add = "jQuery(document).ready(function () { var chart = c3.generate($json);});";
  drupal_add_js($javascript_to_add,array('type' => 'inline'));

  $block->content = theme('users_events_graph', array());
  return $block;
}

/**
 * Delegated hook_theme().
 */
function fb_safety_users_graphs_content_type_theme(&$theme, $plugin) {
  $theme['users_events_graph'] = array(
    'template' => 'graphs',
    'path' => $plugin['path'],
    'variables' => array(),
  );
}
