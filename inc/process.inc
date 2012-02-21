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
    $vars['body_attributes'] = ' class="' . $vars['classes'] . '"' . $vars['attributes'];
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
