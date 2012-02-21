<?php

/**
 * Implements hook_preprocess_html().
 * - Use Drupal 8 way of processing attributes for <html> and <body>.
 */
function base_preprocess_html(&$vars) {
  // Create a variable for the path to the theme.
  $vars['path'] = drupal_get_path('theme', 'base');
  
  // Initializes attributes which are specific to the html and body elements.
  $vars['html_attributes_array'] = array();
  $vars['body_attributes_array'] = array();

  // HTML element attributes.
  $vars['html_attributes_array']['lang'] = $GLOBALS['language']->language;
  $vars['html_attributes_array']['dir'] = $GLOBALS['language']->direction ? 'rtl' : 'ltr';
  
  if (module_exists('rdf')) {
    // Adds RDF namespace prefix bindings in the form of an RDFa 1.1 prefix
    // attribute inside the html element.
    $prefixes = array();
    foreach (rdf_get_namespaces() as $prefix => $uri) {
      $vars['html_attributes_array']['prefix'][] = $prefix . ': ' . $uri . "\n";
    }
  }

  // Add the admin stylesheet.
  if ($vars['is_admin']) {
    drupal_add_css($vars['path'] . '/css/admin.css', array('group' => CSS_THEME, 'media' => 'screen'));
  }

  // Add body classes indicating tabs.
  if (menu_primary_local_tasks() && menu_secondary_local_tasks()) {
    $vars['classes_array'][] = 'with-tabs';
  }
  elseif (menu_primary_local_tasks()) {
    $vars['classes_array'][] = 'with-tabs tabs-primary';
  }
  elseif (menu_secondary_local_tasks()) {
    $vars['classes_array'][] = 'with-tabs tabs-secondary';
  }
  else {
    $vars['classes_array'][] = 'no-tabs';
  }

  // The default body classes are way too verbose for what we need here.
  // This should really be optional, but since it's not, need to rebuild in
  // in order to remove them.
  $suggestion_classes = array();
  if ($suggestions = theme_get_suggestions(arg(), 'page', '-')) {
     foreach ($suggestions as $suggestion) {
       if ($suggestion != 'page-front') {
         $suggestion_classes[] = drupal_html_class($suggestion);
       }
     }
   }
   $vars['classes_array'] = array_diff($vars['classes_array'], $suggestion_classes);
}

/**
 * Implements template_preprocess_page().
 */
function base_preprocess_page(&$vars) {
  // Prevent the title from being printed in both the node and page template
  // when a full node is being viewed. In order to have properly sectioned HTML5
  // markup for nodes, we must ensure the title is printed inside the node
  // template, as opposed to the page.tpl.php template.
  if (!empty($vars['node']) && $vars['page'] && !arg(2)) {
    $vars['title'] = '';
  }
}

/**
 * Implements template_preprocess_block().
 */
function base_preprocess_block(&$vars) {
  // Add the block ID as a class.
  $vars['classes_array'][] = $vars['block_html_id'];

  $vars['title_attributes_array']['class'][] = 'block-title';
  
  // Hide the block titles in the header and navigation regions by default.
  if ($vars['block']->region == 'header' || $vars['block']->region == 'navigation') {
    $vars['title_attributes_array']['class'][] = 'invisible';
  }    

  // Add default "content" class to the block template. Removing this in a sub-theme
  // for example, will prevent a div from printing, unless there are RDF attributes.
  // @see block.tpl.php provided by this theme.
  $vars['content_attributes_array']['class'][] = 'content';
  
  // Add ARIA roles to core blocks.
  $roles = array(
    'complementary' => array(
      'aggregator',
      'help',
      'locale',
      'poll',
      'profile',
      'node' => array('syndicate'),
      'system' => array('powered-by', 'help'),
      'user' => array('new', 'online'),
    ),
    'navigation' => array(
      'blog',
      'book',
      'comment',
      'forum',
      'menu',
      'menu_block',
      'node' => array('recent'),
      'shortcut',
      'statistics',
      'system' => array_keys(menu_list_system_menus()),
    ),
    'search' => array(
      'search',
    ),
    'form' => array(
      'user' => array('login'),
    ),
  );

  // Loop through the roles defined above and apply them to the necessary blocks.
  // @todo Figure out why this isn't working properly.
  foreach ($roles as $role => $module) {
    if (!is_array($role) && in_array($vars['block']->module, $module)) {
      $vars['attributes_array']['role'] = "$role";
    }
    elseif (is_array($role)) {
      foreach ($role as $module => $delta) {
        if ($vars['block']->module == $module && in_array($vars['block']->delta, $delta)) {
          $vars['attributes_array']['role'] = "$role";
        }
      }
    }

  // Provide some template suggestions based on ARIA roles.
  switch (isset($vars['attributes_array']['role'])) {
    case 'navigation':
      $vars['theme_hook_suggestions'][] = 'block__nav';
      break;
    case 'form':
    case 'search':
      $vars['theme_hook_suggestions'][] = 'block__plain';
      break;
    }
  }

  // Find the main navigation and change the template based on region placement.
  if ($vars['block']->region == 'navigation' || $vars['block']->region == 'header') {
    if ($vars['block']->module == 'menu_block' || $vars['block']->module == 'menu' || $vars['block']->module == 'system' && in_array($vars['block']->delta, array_keys(menu_list_system_menus()))) {
      if ($vars['block']->region == 'navigation') {
        // The wrappers and hidden title are dealt with in page.tpl.php.
        $vars['theme_hook_suggestions'][] = 'block__content_only';
      }
      else {
        // The block is placed in the header, so wrap it in <nav> and add .main-menu class.
        $vars['classes_array'][] = 'main-menu';
        $vars['theme_hook_suggestions'][] = 'block__nav';
      }
    }
  }

}

/**
 * Implements template_preprocess_node().
 */
function base_preprocess_node(&$vars) {
  $node = $vars['node'];
  
  // Add article ARIA role.
  $vars['attributes_array']['role'] = 'article';

  // Remove the default classes. This is all IE6 stuff.
  $classes_to_remove = array(drupal_html_class('node-' . $node->type), 'node-promoted', 'node-sticky', 'node-unpublished', 'node-teaser', 'node-preview');
  $vars['classes_array'] = array_diff($vars['classes_array'], $classes_to_remove);
  
  // Recreate some of them without the 'node-' prefix.
  // Gather node classes.
  $vars['classes_array'][] = drupal_html_class($node->type);
  if (isset($vars['preview'])) {
    $vars['classes_array'][] = 'preview';
  }
  if ($vars['sticky']) {
    $vars['classes_array'][] = 'sticky';
  }
  if ($vars['teaser']) {
    $vars['classes_array'][] = 'teaser';
  }
  if (!$vars['status']) {
    $vars['classes_array'][] = 'unpublished';
  }

  // Create a timestamp suitable for the datetime attribute.
  $vars['timestamp'] = date_iso8601($node->created);
}


/**
 * Implements template_preprocess_field().
 */
function base_preprocess_field(&$vars) {
  // Default field wrapper.
  $vars['tag'] = 'div';
  
  // Field label wrappers.
  switch ($vars['element']['#field_name']) {
    case 'field_tags':
      $vars['heading'] = 'span';
      break;
    default:
      // If the view mode is "full" use <h2> for the field labels. Otherwise,
      // assume a teaser or more compact view mode is being displayed, and use <h3>.
      $vars['heading'] = ($vars['element']['#view_mode'] == 'full') ? 'h2' : 'h3';
      break;
  }

  // Add a less verbose field name class: .field-NAME
  $vars['classes_array'][] = drupal_html_class($vars['element']['#field_name']);

  // Use .field-body instead of .body
  if ($vars['element']['#field_name'] == 'body') {
    $vars['classes_array'][] = 'field-body';
  }

  // Remove some classes from the wrapper <div>.
  $classes_to_remove = array(
    'field',
    'field-label-' . drupal_html_class($vars['element']['#label_display']),
    'field-type-' . drupal_html_class($vars['element']['#field_type']),
    'field-name-' . drupal_html_class($vars['element']['#field_name']),
    'body',
    'clearfix',
  );
  $vars['classes_array'] = array_diff($vars['classes_array'], $classes_to_remove);
}