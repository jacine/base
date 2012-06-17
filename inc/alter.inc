<?php

/**
 * Implements hook_html_head_alter().
 */
function base_html_head_alter(&$head_elements) {
  // Simplify the meta charset element.
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}

/**
 * Implements hook_css_alter().
 */
function base_css_alter(&$css) {
  $theme = drupal_get_path('theme', 'base');

  // Remove some stylesheets.
  unset(
    $css['modules/user/user.css'],
    $css['misc/ui/jquery.ui.theme.css'],
    $css['modules/comment/comment.css'],
    $css['modules/field/theme/field.css'],
    $css['modules/filter/filter.css'],
    $css['modules/node/node.css'],
    $css['modules/search/search.css'],
    $css['modules/system/system.menus.css'],
    $css['modules/system/system.messages.css'],
    $css['modules/system/system.theme.css'],
    $css[drupal_get_path('module', 'ctools') . '/css/ctools.css'],
    $css[drupal_get_path('module', 'views') . '/css/views.css'],
    $css[drupal_get_path('module', 'context') . '/plugins/context_reaction_block.css']
  );

  // Use the theme's reset instead of system.base.css.
  if (isset($css['modules/system/system.base.css'])) {
    $css['modules/system/system.base.css']['data'] = $theme . '/css/global.css';
    $css['modules/system/system.base.css']['group'] = CSS_SYSTEM;
  }

  // Set the default media type to screen.
  foreach ($css as $path => $value) {
    if ($css[$path]['media'] == 'all') {
      $css[$path]['media'] = 'screen';
    }
  }

  // Load the Contextual CSS last, whether overridden or not.
  if (isset($css['modules/contextual/contextual.css'])) {
    $css['modules/contextual/contextual.css']['group'] = CSS_THEME;
    $css['modules/contextual/contextual.css']['weight'] = 100;
  }
}

/**
 * Implements hook_page_alter().
 */
function base_page_alter(&$page) {
  // Remove all the region wrappers, except for page_top and page_bottom.
  foreach (element_children($page) as $key => $region) {
    if (!empty($page[$region]['#theme_wrappers']) && !in_array($region, array('page_top', 'page_bottom'))) {
      $page[$region]['#theme_wrappers'] = array_diff($page[$region]['#theme_wrappers'], array('region'));
    }
  }
  // Remove the wrapper from the main content block.
  if (!empty($page['content']['system_main'])) {
    $page['content']['system_main']['#theme_wrappers'] = array_diff($page['content']['system_main']['#theme_wrappers'], array('block'));
  }

}
