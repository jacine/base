<?php

/**
 * Overrides theme_breadcrumb().
 * - Add <nav> wrapper.
 * - Add heading.
 */
function base_breadcrumb($vars) {
  if (!empty($vars['breadcrumb']) && count($vars['breadcrumb']) > 1) {
    $heading = t('Breadcrumb Navigation');
    return '<nav class="breadcrumbs"><h2 class="visuallyhidden">' . $heading . '</h2>\n' . implode(' / ', $vars['breadcrumb']) . '</nav>';
  }
}

/**
 * Overrides theme_mark().
 * - Use <mark> element.
 */
function base_mark($vars) {
  global $user;
  $type = $vars['type'];

  if ($user->uid) {
    if ($type == MARK_NEW) {
      return ' <mark class="new">' . t('new') . '</mark>';
    }
    elseif ($type == MARK_UPDATED) {
      return ' <mark class="updated">' . t('updated') . '</mark>';
    }
  }
}

/**
 * Overrides theme_menu_local_tasks().
 */
function base_menu_local_tasks(&$vars) {
  $output = '';

  if (!empty($vars['primary'])) {
    $vars['primary']['#prefix'] = '<h2 class="visuallyhidden">' . t('Primary tabs') . '</h2>';
    $vars['primary']['#prefix'] .= '<ul class="tabs primary">';
    $vars['primary']['#suffix'] = '</ul>';
    $output .= render($vars['primary']);
  }
  if (!empty($vars['secondary'])) {
    $vars['secondary']['#prefix'] = '<h2 class="visuallyhidden">' . t('Secondary tabs') . '</h2>';
    $vars['secondary']['#prefix'] = '<ul class="tabs secondary">';
    $vars['secondary']['#suffix'] = '</ul>';
    $output .= render($vars['secondary']);
  }

  if ($output) {
    return '<nav class="tabs" role="navigation">' . $output . '</nav>';
  }

}

/**
 * Overrides theme_tablesort_indicator().
 */
function base_tablesort_indicator($vars) {
  if ($vars['style'] == 'asc') {
    return ' <span class="sort ascending">' . t('sort ascending') . '</span>';
  }
  else {
    return ' <span class="sort descending">' . t('sort descending') . '</span>';
  }
}

/**
 * Overrides theme_links().
 * This version is from the Edge module. The patch was RTBC for Drupal 7, but it
 * was "too late." In addition to being more efficient it fixes a couple bugs.
 */
function base_links($vars) {
  global $language_url;

  $links = $vars['links'];
  $attributes = $vars['attributes'];
  $heading = $vars['heading'];
  $output = '';

  if (!empty($links)) {
    // Prepend the heading to the list, if any.
    if (!empty($heading)) {
      // Convert a string heading into an array, using a H2 tag by default.
      if (is_string($heading)) {
        $heading = array('text' => $heading);
      }
      // Merge in default array properties into $heading.
      $heading += array(
        'level' => 'h2',
        'attributes' => array(),
      );
      // @todo Remove backwards compatibility for $heading['class'].
      if (isset($heading['class'])) {
        $heading['attributes']['class'] = $heading['class'];
      }

      $output .= '<' . $heading['level'] . drupal_attributes($heading['attributes']) . '>';
      $output .= check_plain($heading['text']);
      $output .= '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 0;
    foreach ($links as $key => $link) {
      $i++;

      $class = array();
      // Use the array key as class name.
      $class[] = drupal_html_class($key);
      // Add odd/even, first, and last classes.
      $class[] = ($i % 2 ? 'odd' : 'even');
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }

      // Handle links.
      if (isset($link['href'])) {
        $is_current_path = ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()));
        $is_current_language = (empty($link['language']) || $link['language']->langcode == $language_url->langcode);
        if ($is_current_path && $is_current_language) {
          $class[] = 'active';
        }
        // Pass in $link as $options, they share the same keys.
        $item = l($link['title'], $link['href'], $link);
      }
      // Handle title-only text items.
      else {
        // Merge in default array properties into $link.
        $link += array(
          'html' => FALSE,
          'attributes' => array(),
        );
        $item = '<span' . drupal_attributes($link['attributes']) . '>';
        $item .= ($link['html'] ? $link['title'] : check_plain($link['title']));
        $item .= '</span>';
      }

      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';
      $output .= $item;
      $output .= '</li>';
    }

    $output .= '</ul>';
  }

  return $output;
}
