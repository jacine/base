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
  $form['base']['node_title_location'] = array(
    '#type' => 'select',
    '#title' => t('Where should the node title print?'),
    '#options' => array(
      'node' => t('node.tpl.php'),
      'page' => t('page.tpl.php'),
    ),
    '#default_value' => theme_get_setting('node_title_location'),
  );

  $form['base']['mobile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['base']['mobile']['mobile_friendly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mobile Friendly?'),
    '#default_value' => theme_get_setting('mobile_friendly'),
    '#description' => t('Prints mobile <code>&lt;meta&gt;</code> tags.')
  );
  $form['base']['mobile']['touch_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Touch Icons?'),
    '#default_value' => theme_get_setting('touch_icons'),
    '#description' => '<p>' . t('Prints <code>&lt;meta&gt;</code> tags for pointing toward the following images, which you must provide in the subtheme:') . '</p><ul>
        <li><code>img/apple-touch-icon-144x144-precomposed.png</code></li>
        <li><code>img/apple-touch-icon-114x114-precomposed.png</code></li>
        <li><code>img/apple-touch-icon-72x72-precomposed.png</code></li>
        <li><code>img/apple-touch-icon-57x57-precomposed.png</code></li>
      </ul>',
  );

  return $form;
}
