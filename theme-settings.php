<?php

/**
 * Implements hook_form_FORM_ID_alter().
 */
function base_form_system_theme_settings_alter(&$form, $form_state) {

  $form['base'] = array(
    '#type' => 'fieldset',
    '#title' => t('Base settings'),
    '#weight' => -1,
  );

  // General settings.
  $form['base']['node_title_location'] = array(
    '#attributes' => array('class' => array('container-inline')),
    '#type' => 'radios',
    '#title' => t('Where should the node title print?'),
    '#options' => array(
      'node' => t('node.tpl.php'),
      'page' => t('page.tpl.php'),
    ),
    '#default_value' => theme_get_setting('node_title_location'),
  );

  // jQuery Override
  $form['base']['jquery'] = array(
    '#type' => 'fieldset',
    '#title' => t('jQuery'),
  );
  $form['base']['jquery']['use_theme_jquery'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use theme's version of jQuery."),
    '#default_value' => theme_get_setting('use_theme_jquery'),
    '#description' => t('Note: This will override any version of jQuery present, whether controlled by Drupal core, <a href="@jquery-update">jQuery Update</a>, or <a href="@speedy">Speedy</a> module(s), or otherwise, and may conflict with contributed modules that have a end user interface. Not using it may cause features of the theme not to work properly as well.', array('@jquery-update' => 'http://drupal.org/project/query_update', '@speedy' => 'http://drupal.org/project/speedy')),
  );

  // Mobile settings.
  $form['base']['mobile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile'),
  );
  $form['base']['mobile']['mobile_friendly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mobile Friendly?'),
    '#default_value' => theme_get_setting('mobile_friendly'),
    '#description' => t('Prints mobile <code>&lt;meta&gt;</code> tags.'),
  );
  $form['base']['mobile']['touch_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Touch Icons?'),
    '#default_value' => theme_get_setting('touch_icons'),
    '#description' => '<p>' . t('Prints <code>&lt;meta&gt;</code> tags for pointing toward the following images, which you must provide in the subtheme.') . '</p>
      <details>
        <summary>Touch icon file names</summary>
        <ul>
          <li><code>img/apple-touch-icon-144x144-precomposed.png</code></li>
          <li><code>img/apple-touch-icon-114x114-precomposed.png</code></li>
          <li><code>img/apple-touch-icon-72x72-precomposed.png</code></li>
          <li><code>img/apple-touch-icon-57x57-precomposed.png</code></li>
        </ul>
      </details>',
  );

  // Maintentance settings.
  $form['base']['maintenance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Maintenance Pages'),
  );
  $form['base']['maintenance']['maintenance_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load JavaScript files on maintenance page'),
    '#default_value' => theme_get_setting('maintenance_js'),
    '#description' => t("Most maintenance pages are very lightweight and don't require JS, so why load it?"),
  );
  $form['base']['maintenance']['maintenance_html'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow maintenance message to contain HTML'),
    '#default_value' => theme_get_setting('maintenance_html'),
    '#description' => t('By default, the maintenance message is plain text. Checking this option will run it through the <a href="@full-html-filter">Full HTML</a> text formatter.', array('@full-html-filter' => url('admin/config/content/formats/full_html'))),
  );

  return $form;
}
