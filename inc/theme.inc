<?php

/**
 * Overrides theme_breadcrumb().
 * - Add <nav> wrapper.
 * - Add heading.
 */
function base_breadcrumb($vars) {
  if (!empty($vars['breadcrumb']) && count($vars['breadcrumb']) > 1) {
    $heading = t('Breadcrumb Navigation');
    return '<nav class="breadcrumbs"><h2 class="visuallyhidden">' . $heading . "</h2>\n" . implode(' / ', $vars['breadcrumb']) . '</nav>';
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

/**
 * Overrides theme_more_link().
 * - Removes the wrapping DIV and adds to class to the link itself.
 * - Made the text and title attribute configurable.
 * @todo Make Views use this.
 */
function base_more_link($vars) {
  $title = !empty($vars['title']) ? t($vars['title']) : t('More');
  return l($title, $vars['url'], array(
    'attributes' => array(
      'title' => !empty($vars['title']) ? $vars['title'] : t('More'),
      'class' => 'more-link ' . drupal_html_class($vars['title']),
      )
    )
  );
}

/**
 * Overrides theme_form().
 * - Removes the wrapper DIV.
 */
function base_form($vars) {
  $element = $vars['element'];
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  element_set_attributes($element, array('method', 'id'));
  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }
  return '<form' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</form>';
}

/**
 * Overrides theme_item_list()
 *
 */
function base_item_list($variables) {
  $items = $variables['items'];
  $title = (string) $variables['title'];
  $type = $variables['type'];
  $list_attributes = $variables['attributes'];

  $output = '';
  if ($items) {
    $output .= '<' . $type . drupal_attributes($list_attributes) . '>';

    $num_items = count($items);
    $i = 0;
    foreach ($items as $key => $item) {
      $i++;
      $attributes = array();

      if (is_array($item)) {
        $value = '';
        if (isset($item['data'])) {
          $value .= $item['data'];
        }
        $attributes = array_diff_key($item, array('data' => 0, 'children' => 0));

        // Append nested child list, if any.
        if (isset($item['children'])) {
          // HTML attributes for the outer list are defined in the 'attributes'
          // theme variable, but not inherited by children. For nested lists,
          // all non-numeric keys in 'children' are used as list attributes.
          $child_list_attributes = array();
          foreach ($item['children'] as $child_key => $child_item) {
            if (is_string($child_key)) {
              $child_list_attributes[$child_key] = $child_item;
              unset($item['children'][$child_key]);
            }
          }
          $value .= theme('item_list', array(
            'items' => $item['children'],
            'type' => $type,
            'attributes' => $child_list_attributes,
          ));
        }
      }
      else {
        $value = $item;
      }

      $attributes['class'][] = ($i % 2 ? 'odd' : 'even');
      if ($i == 1) {
        $attributes['class'][] = 'first';
      }
      if ($i == $num_items) {
        $attributes['class'][] = 'last';
      }

      $output .= '<li' . drupal_attributes($attributes) . '>' . $value . '</li>';
    }
    $output .= "</$type>";
  }

  // Only output the list container and title, if there are any list items.
  // Check to see whether the block title exists before adding a header.
  // Empty headers are not semantic and present accessibility challenges.
  if ($output !== '') {
    if ($title !== '') {
      $title = '<h3>' . $title . '</h3>';
    }
    $output = '<div class="item-list">' . $title . $output . '</div>';
  }

  return $output;
}
