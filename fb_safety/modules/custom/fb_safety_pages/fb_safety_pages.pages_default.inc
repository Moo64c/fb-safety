<?php
/**
 * @file
 * fb_safety_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function fb_safety_pages_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'home';
  $page->task = 'page';
  $page->admin_title = 'Home';
  $page->admin_description = '';
  $page->path = ' ';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_home_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'home';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Home',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(
      0 => array(
        'identifier' => 'User',
        'keyword' => 'user',
        'name' => 'user',
        'type' => 'current',
        'uid' => '',
        'id' => 1,
      ),
    ),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'threecol_25_50_25_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'top' => NULL,
      'left' => NULL,
      'middle' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '77415c0f-28ca-4f85-ab70-fc7d1be7cdc6';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-9b751d0f-6278-453e-ad83-647745d3f539';
    $pane->panel = 'bottom';
    $pane->type = 'graphs';
    $pane->subtype = 'graphs';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '9b751d0f-6278-453e-ad83-647745d3f539';
    $display->content['new-9b751d0f-6278-453e-ad83-647745d3f539'] = $pane;
    $display->panels['bottom'][0] = 'new-9b751d0f-6278-453e-ad83-647745d3f539';
    $pane = new stdClass();
    $pane->pid = 'new-2671ff36-436c-4930-8d88-854db89b57f6';
    $pane->panel = 'bottom';
    $pane->type = 'views_panes';
    $pane->subtype = 'events-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'uid' => '%user:uid',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '2671ff36-436c-4930-8d88-854db89b57f6';
    $display->content['new-2671ff36-436c-4930-8d88-854db89b57f6'] = $pane;
    $display->panels['bottom'][1] = 'new-2671ff36-436c-4930-8d88-854db89b57f6';
    $pane = new stdClass();
    $pane->pid = 'new-cb57e9f7-cdc3-429f-88f5-0a8798a644e6';
    $pane->panel = 'bottom';
    $pane->type = 'graphs';
    $pane->subtype = 'graphs';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'cb57e9f7-cdc3-429f-88f5-0a8798a644e6';
    $display->content['new-cb57e9f7-cdc3-429f-88f5-0a8798a644e6'] = $pane;
    $display->panels['bottom'][2] = 'new-cb57e9f7-cdc3-429f-88f5-0a8798a644e6';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['home'] = $page;

  return $pages;

}
