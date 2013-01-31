<?php

/**
 * Implements hook_js_alter().
 */
function base_js_alter(&$js) {
  $theme = drupal_get_path('theme', 'base');

  // JS Files to remove.
  $remove = array(
    'misc/tableheader.js',
  );

  if (!empty($remove)) {
    foreach ($js as $key => $value) {
      if (in_array($key, $remove)) {
        unset($js[$key]);
      }
    }
  }

  // JS files to replace.
  $replace = array(
    'misc/tabledrag.js' => 'tabledrag.js',
    'misc/collapse.js' => 'collapse.js',
    'misc/vertical-tabs.js' => 'vertical-tabs.js',
    'misc/states.js' => 'states.js',
    'modules/toolbar/toolbar.js' => 'toolbar.js',
    'misc/machine-name.js' => 'machine-name.js',
    'modules/user/user.js' => 'password.js',
  );
  if (!empty($replace)) {
    foreach ($js as $key => $value) {
      // Override jQuery (even when jQuery Update is used).
      if (strstr($key, 'jquery.min.js')) {
        $js[$key]['data'] = $theme . '/js/libraries/jquery/jquery.min.js';
        $js[$key]['version'] = '1.9.0';
      }
      // Handle replacements.
      if (array_key_exists($key, $replace)) {
        $js[$key]['data'] = $theme . '/js/' . $replace[$key];
        if ($replace[$key] == 'toolbar.js') {
          $js[$key]['scope'] = 'footer';
        }
      }
    }
  }
}

/**
 * Implements hook_css_alter().
 */
function base_css_alter(&$css) {
  $theme = drupal_get_path('theme', 'base');

  // CSS files to remove.
  $remove = array(
    'misc/ui/jquery.ui.theme.css',
    'modules/aggregator/aggregator.css',
    'modules/block/block.css',
    'modules/book/book.css',
    'modules/comment/comment.css',
    'modules/field/theme/field.css',
    'modules/filter/filter.css',
    'modules/node/node.css',
    'modules/poll/poll.css',
    'modules/user/user.css',
    'modules/search/search.css',
    'modules/shortcut/shortcut.css',
    // 'modules/system/system.admin.css',
    'modules/system/system.menus.css',
    'modules/system/system.messages.css',
    'modules/system/system.theme.css',
    drupal_get_path('module', 'ctools') . '/css/ctools.css',
    drupal_get_path('module', 'views') . '/css/views.css',
  );
  if (!empty($remove)) {
    foreach ($css as $key => $value) {
      if (in_array($key, $remove)) {
        unset($css[$key]);
      }
    }
  }

  // CSS files to replace.
  $replace = array(
    'modules/system/system.base.css' => 'global.css',
    'misc/vertical-tabs.css' => 'vertical-tabs.css',
    'modules/toolbar/toolbar.css' => 'toolbar.css',
  );
  if (!empty($replace)) {
    foreach ($css as $key => $value) {
      if (array_key_exists($key, $replace)) {
        $css[$key]['data'] = $theme . '/css/' . $replace[$key];
      }
    }
  }

  // Make contextual.css load last.
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

/**
 * Implements hook_html_head_alter().
 */
function base_html_head_alter(&$head_elements) {
  global $theme;
  // Get the path to the active theme (not necessarily this theme).
  $path = drupal_get_path('theme', $theme);

  // Simplify the meta charset element.
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );

  // Use Chrome Frame.
  $head_elements['base_chrome_frame'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'X-UA-Compatible',
      'content' => 'IE=edge,chrome=1'
    ),
  );

  // Mobile-friendly Viewport settings.
  $head_elements['base_handheld_friendly'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'HandheldFriendly',
      'content' => 'True',
    ),
  );
  $head_elements['base_mobile_optimized'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'MobileOptimized',
      'content' => '320',
    ),
  );
  $head_elements['base_viewport'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width,initial-scale=1.0',
    ),
  );

  // Activate ClearType for Mobile IE.
  $head_elements['base_cleartype'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'cleartype',
      'content' => 'on',
    ),
  );

  // Apple Touch Icons. See: http://mathiasbynens.be/notes/touch-icons
  // Note: When a subtheme is active, this pulls the icons from the subtheme,
  // not the base theme. Make sure these files exist.

  if (theme_get_setting('touch_icons') == 1) {
    // For third-generation iPad with high-resolution Retina display.
    $head_elements['base_apple_touch_144'] = array(
      '#type' => 'html_tag',
      '#tag' => 'link',
      '#attributes' => array(
        'rel' => 'apple-touch-icon-precomposed',
        'sizes' => '144x144',
        'href' => $path . '/img/apple-touch-icon-144x144-precomposed.png'
      ),
    );
    // iPhone 4 with high-resolution Retina display.
    $head_elements['base_apple_touch_114'] = array(
      '#type' => 'html_tag',
      '#tag' => 'link',
      '#attributes' => array(
        'rel' => 'apple-touch-icon-precomposed',
        'sizes' => '114x114',
        'href' => $path . '/img/apple-touch-icon-114x114-precomposed.png'
      ),
    );
    // First and second generation iPads.
    $head_elements['base_apple_touch_72'] = array(
      '#type' => 'html_tag',
      '#tag' => 'link',
      '#attributes' => array(
        'rel' => 'apple-touch-icon-precomposed',
        'sizes' => '72x72',
        'href' => $path . '/img/apple-touch-icon-72x72-precomposed.png'
      ),
    );
    // Fallback...
    // Non-Retina iPhone, iPod Touch, and Android 2.1+ devices.
    // Also be sure to include apple-touch-icon.png. iOS 1 and Blackberry don't
    // support the precomposed option,
    $head_elements['base_apple_touch'] = array(
      '#type' => 'html_tag',
      '#tag' => 'link',
      '#attributes' => array(
        'rel' => 'apple-touch-icon-precomposed',
        'href' => $path . '/img/apple-touch-icon-precomposed.png'
      ),
    );
  }
}
