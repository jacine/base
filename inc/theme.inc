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

      // This fixes a bug caused by running drupal_html_class($key), which
      // could potentially be something like "menu-345 active". Running
      // drupal_html_class() on that turns it into menu-345-active, which is
      // wrong. We need "menu-123 active". This may break stuff elsewhere, so
      // need to test it out a bit.
      $class_friendly_key = explode(' ', $key);
      if (is_array($class_friendly_key)) {
        $class = array_merge($class_friendly_key, $class);
      }
      else {
        // Use the array key as class name.
        $class[] = drupal_html_class($key);
      }


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
  return l('More', $vars['url'], array(
    'attributes' => array(
      'title' => $title,
      'class' => array('more-link', 'more-' . drupal_html_class($title)),
      )
    )
  );
}

/**
 * Overrides theme_item_list().
 * - Use an early D8 version of this. It includes bug fixes.
 * - Remove the div.item-list wrapper.
 */
function base_item_list($vars) {
  $items = $vars['items'];
  $title = (string) $vars['title'];
  $type = $vars['type'];
  $list_attributes = $vars['attributes'];

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
    $output = $title . $output;
  }

  return $output;
}

/**
 * Overrides base_pager().
 * - Simplify the list item classes.
 * - Remove the arrows from the labels.
 * - Add rel="next/prev" attributes.
 * - Add an empty <a> element to the active link.
 * - Change the "current" class to "active".
 * - Remove first/last classes. theme_item_list() handles this already.
 */
function base_pager($vars) {
  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];
  $quantity = $vars['quantity'];
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = theme('pager_first', array('text' => (isset($tags[0]) ? $tags[0] : t('First')), 'element' => $element, 'parameters' => $parameters));
  $li_previous = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : t('Previous')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('Next')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_last = theme('pager_last', array('text' => (isset($tags[4]) ? $tags[4] : t('Last')), 'element' => $element, 'parameters' => $parameters));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
    //   if ($i > 1) {
    //     $items[] = array(
    //       'class' => array('ellipsis'),
    //       'data' => '…',
    //     );
    //   }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('item'),
            'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('active'),
            'data' => '<a>' . $i . '</a>',
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('item'),
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }
      // if ($i < $pager_max) {
      //   $items[] = array(
      //     'class' => array('ellipsis'),
      //     'data' => '…',
      //   );
      // }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('next'),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'data' => $li_last,
      );
    }
    return '<div class="pagination" role="navigation"><h2 class="element-invisible">' . t('Pages') . '</h2>' . theme('item_list', array(
      'items' => $items,
      'attributes' => array('class' => array('pager')),
    )) . '</div>';
  }
}

/**
 * Overrides theme_pager_next().
 * - Adds rel="next" to the link.
 */
function base_pager_next($vars) {
  $text = $vars['text'];
  $element = $vars['element'];
  $interval = $vars['interval'];
  $parameters = $vars['parameters'];
  global $pager_page_array, $pager_total;
  $output = '';

  // If we are anywhere but the last page
  if ($pager_page_array[$element] < ($pager_total[$element] - 1)) {
    $page_new = pager_load_array($pager_page_array[$element] + $interval, $element, $pager_page_array);
    // If the next page is the last page, mark the link as such.
    if ($page_new[$element] == ($pager_total[$element] - 1)) {
      $output = theme('pager_last', array('text' => $text, 'element' => $element, 'parameters' => $parameters));
    }
    // The next page is not the last page.
    else {
      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters));
    }
  }

  return $output;
}

/**
 * Overrides theme_pager_previous().
 * - Adds rel="prev" to the link.
 */
function base_pager_previous($vars) {
  $text = $vars['text'];
  $element = $vars['element'];
  $interval = $vars['interval'];
  $parameters = $vars['parameters'];
  global $pager_page_array;
  $output = '';

  // If we are anywhere but the first page
  if ($pager_page_array[$element] > 0) {
    $page_new = pager_load_array($pager_page_array[$element] - $interval, $element, $pager_page_array);

    // If the previous page is the first page, mark the link as such.
    if ($page_new[$element] == 0) {
      $output = theme('pager_first', array('text' => $text, 'element' => $element, 'parameters' => $parameters));
    }
    // The previous page is not the first page.
    else {
      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters));
    }
  }

  return $output;
}
