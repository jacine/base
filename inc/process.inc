<?php

/**
 * Implements template_process().
 */
function base_process(&$vars) {
  // Use one variable for attributes!
  if ($vars['classes']) {
    $vars['attributes'] = ' class="' . $vars['classes'] . '"' . $vars['attributes'];
  }
}

/**
 * Implements hook_process_html().
 */
function base_process_html(&$vars) {
  // Flatten out html_attributes and body_attributes, which are defined
  // in base_preprocess_html().
  $vars['html_attributes'] = drupal_attributes($vars['html_attributes_array']);
  if ($vars['classes']) {
    $vars['body_attributes'] = $vars['attributes'];
  }
}

/**
 * Implements hook_process_page().
 */
function base_process_page(&$vars) {
  if (isset($vars['suppress_node_title'])) {
    // Prevent the title from being printed in both the node and page template
    // when a full node is being viewed. In order to have properly sectioned HTML5
    // markup for nodes, we must ensure the title is printed inside the node
    // template, as opposed to the page.tpl.php template.
    if (!empty($vars['node']) && $vars['page'] && !arg(2)) {
      $vars['title'] = '';
    }
  }
}

/**
 * Implements template_process_maintenance_page().
 * @see also base_preprocess_maintenance_page() in preprocess.inc.
 */
function base_process_maintenance_page(&$vars) {
  // Remove all scripts from the page.
  if (theme_get_setting('maintenance_js') === 0) {
    $vars['scripts'] = '';
  }
}

/**
 * Implements hook_process_html_tag().
 */
function base_process_html_tag(&$vars) {
  $element = &$vars['element'];

  // Remove type attribute and CDATA comments from script/link/style tags.
  if (in_array($element['#tag'], array('script', 'link', 'style'))) {
    unset($element['#attributes']['type']);
    unset($element['#value_prefix']);
    unset($element['#value_suffix']);
  }
}
