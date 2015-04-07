<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Managers page'),
  'description' => t('Display the managers index page.'),
  'category' => t('Ethosia'),
  'hook theme' => 'ethosia_manager_managers_theme',
  'defaults' => array(
    'layout_type' => 'box',
  ),
);

/**
 * Render callback.
 */
function ethosia_manager_managers_content_type_render($subtype, $conf, $args, $context) {
  // Get the page layout.
  $layout = $conf['layout_type'];

  // Get the page number from the arguments.
  $page = ethosia_general_get_page_from_args($args);

  $options = array(
    'restful_resource' => 'managers',
    'page_theme' => 'ethosia_managers',
    'page_element' => page_element_get_element_details('managers', 'box'),
  );

  if (!empty($_GET['category'])) {
    $options['restful_request'] = array('category' => $_GET['category']);
  }

  $data = ethosia_general_get_page_results($options, $layout, $page);

  $block = new stdClass();
  $block->content = $data['content'];
  return $block;
}

/**
 * Edit form.
 *
 * Add layout type select field in order to define the page layout.
 */
function ethosia_manager_managers_content_type_edit_form($form, &$form_state) {
  ethosia_general_set_plugin_layout_type_field($form, $form_state);

  return $form;
}

/**
 * Edit form submit callback.
 */
function ethosia_manager_managers_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['layout_type'] = $form_state['values']['layout_type'];
}

/**
 * Delegated hook_theme().
 */
function ethosia_manager_managers_theme(&$theme, $plugin) {
  $theme['ethosia_managers'] = array(
    'template' => 'ethosia_managers',
    'path' => $plugin['path'],
    'variables' => array(),
  );
}
