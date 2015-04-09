<?php

/**
 * @file
 * template.php
 */

/**
 * Preprocess page.
 */
function bootstrap_subtheme_preprocess_page(&$variables) {
  global $user;
  $login = "";

  if (empty($user->uid)) {
    $form = drupal_get_form('user_login_block');
    unset($form['links']);
    $login = drupal_render($form);
  }
  else {
    $login = l('logout', 'user/logout');
  }

  $variables['login'] = $login;
}

/**
 * Node preprocess.
 */
function bootstrap_subtheme_preprocess_node(&$variables) {
  $node = $variables['node'];
  $view_mode = $variables['view_mode'];
  $variables['theme_hook_suggestions'][] = "node__{$node->type}__{$view_mode}";

  $preprocess_function = "bootstrap_subtheme_preprocess_node__{$node->type}__{$view_mode}";
  if (function_exists($preprocess_function)) {
    $preprocess_function($variables);
  }
}

/**
 * Preprocess teaser display for event nodes.
 */
function bootstrap_subtheme_preprocess_node__event__teaser(&$variables) {
  $wrapper = entity_metadata_wrapper('node', $variables['node']);
  $tid = $wrapper->field_severity->getIdentifier();

  $classes = array(
    1 => 'yellow',
    2 => 'orange',
    3 => 'red',
    4 => 'red2',
    5 => 'blue3',
  );

  $variables['title'] = $wrapper->label();
  $variables['body'] = $wrapper->body->value();
  if (is_array($variables['body'])) {
    $variables['body'] = $variables['body']['safe_value'];
  }

  $time = $wrapper->created->value();
  $variables['class'] = empty($classes[$tid]) ? "yellow" : $classes[$tid];
  $variables['icon'] = _bootstrap_subtheme_event_icon($tid, $variables['class']);
  $variables['date'] = date("d-m-Y", $time);
}

/**
 * Return an image depending on the term used.
 * @param $tid
 * @return formatted image HTML.
 */
function _bootstrap_subtheme_event_icon($tid, $class) {
  $image_path = drupal_get_path('theme', 'bootstrap_subtheme') . "/images/" . $class .".png";
  return "<img class=\"event-icon\" src=\"$image_path\"/>";
}
