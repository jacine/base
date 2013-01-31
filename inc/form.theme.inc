<?php

/**
 * @file
 * Contains form-related theme function overrides.
 *
 * - theme_button()
 * - theme_checkbox()
 * - theme_checkboxes()
 * - theme_container()
 * - theme_date()
 * - theme_fieldset()
 * - theme_file()
 * - theme_file_managed_file()
 * - theme_form()
 * - theme_form_element()
 * - theme_image_button()
 * - theme_password()
 * - theme_radio()
 * - theme_radios()
 * - theme_select()
 * - theme_textarea()
 * - theme_textfield()
 * - theme_text_format_wrapper()
 */

/**
 * Overrides theme_button().
 * - Also used for theme_submit().
 * - Remove .form-button-disabled class.
 * - Remove .form-button-type class.
 */
function base_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  return '<input' . drupal_attributes($element['#attributes']) . '>';
}

/**
 * Overrides theme_image_button().
 * - Remove .form-button-disabled class.
 * - Remove .form-button-type class.
 */
function base_image_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'image';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['src'] = file_create_url($element['#src']);
  if (!empty($element['#title'])) {
    $element['#attributes']['alt'] = $element['#title'];
    $element['#attributes']['title'] = $element['#title'];
  }

  return '<input' . drupal_attributes($element['#attributes']) . '>';
}

/**
 * Overrides theme_checkbox().
 * - Remove .form-checkbox class.
 */
function base_checkbox($variables) {
  $element = $variables['element'];
  $t = get_t();
  $element['#attributes']['type'] = 'checkbox';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  // Required for .required, .error classes.
  _form_set_class($element);

  return '<input' . drupal_attributes($element['#attributes']) . ' >';
}

/**
 * Overrides theme_checkboxes().
 * - Add .form-container class.
 */
function base_checkboxes($variables) {
  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }

  $attributes['class'][] = 'form-container';
  $attributes['class'][] = 'form-checkboxes';
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = array_merge($attributes['class'], $element['#attributes']['class']);
  }
  if (isset($element['#attributes']['title'])) {
    $attributes['title'] = $element['#attributes']['title'];
  }
  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
}

/**
 * Override theme_container().
 * - Get rid of the form-wrapper class.
 * - Identify #states containers as components.
 */
function base_container($vars) {
  $element = $vars['element'];

  // Special handling for form elements.
  if (isset($element['#array_parents'])) {
    // Assign an html ID.
    if (!isset($element['#attributes']['id'])) {
      $element['#attributes']['id'] = $element['#id'];
    }
    // Add the 'form-wrapper' class.
    // $element['#attributes']['class'][] = 'form-wrapper';
    $element['#attributes']['class'][] = 'form-container';

    // If there are #states attached to this element, add some markup to
    // tell us this.
    if (!empty($element['#states'])) {
      $element['#attributes']['data-component'][] = 'states';
    }
  }

  return '<div' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</div>';
}

/**
 * Overrides theme_date().
 * - Add .form-container and .form-selects classes.
 */
function base_date($variables) {
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  // Make this consistent with multiple radios and checkboxes.
  $attributes['class'][] = 'form-container';
  $attributes['class'][] = 'form-selects';

  // Debating whether or not to keep this.
  $attributes['class'][] = 'container-inline';

  return '<div' . drupal_attributes($attributes) . '>' . drupal_render_children($element) . '</div>';
}

/**
 * Override theme_fieldset().
 * - Get rid of div.fieldset-wrapper.
 * - Get rid of legend span shit. Can add it back if needed.
 * - Change .fieldset-description to straight .description.
 * - Remove .form-wrapper class.
 */
function base_fieldset($vars) {
  $element = $vars['element'];
  element_set_attributes($element, array('id'));

  // Required for .required, .error classes.
  _form_set_class($element);

  if ($element['#collapsible'] == TRUE) {
    // Use data attributes to communicate component and state.
    $element['#attributes']['data-component'] = 'collapse';
    $element['#attributes']['data-state'] = $element['#collapsed'] == 1 ? 'closed' : 'open';

    // Remove the classes.
    $element['#attributes']['class'] = array_diff($element['#attributes']['class'], array('collapsible', 'collapsed', 'expanded'));
    // Don't print an empty class attribute.
    if (empty($element['#attributes']['class'])) {
      unset($element['#attributes']['class']);
    }
  }

  $legend = '';
  if (!empty($element['#title'])) {
    $legend = '<legend class="title">' . $element['#title'] . '</legend>';
  }

  $output = '';
  if (!empty($element['#description'])) {
    $output = '<div class="description">' . $element['#description'] . '</div>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  if ($element['#collapsible'] == TRUE) {
    $output = '<div class="content">' . $output . '</div>';
  }

  return '<fieldset' . drupal_attributes($element['#attributes']) . '>' . $legend . $output .  "</fieldset>\n";
}

/**
 * Overrides theme_file().
 * - Stop adding the size attribute unless it's different than the default.
 * - Stop adding the .form-file class.
 */
function base_file($vars) {
  $element = $vars['element'];
  $element['#attributes']['type'] = 'file';

  // Stop fucking with the size of elements just for the hell of it.
  $attrs = array('id', 'name', 'size');
  if ($element['#size'] == 60) {
    $attrs = array_diff($attrs, array('size'));
  }
  element_set_attributes($element, $attrs);

  // Required for .required, .error classes.
  _form_set_class($element);

  return '<input' . drupal_attributes($element['#attributes']) . '>';
}

/**
 * Overrides theme_file_managed_file().
 * - Make consistent with other form components.
 */
function base_file_managed_file($vars) {
  $element = $vars['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'form-managed-file';

  // This wrapper is required to apply JS behaviors and CSS styling.
  $output = '';
  $output .= '<div' . drupal_attributes($attributes) . '>';
  $output .= drupal_render_children($element);
  $output .= '</div>';
  return $output;
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
 * Overrides theme_form_element().
 */
function base_form_element($vars) {
  $element = &$vars['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.


  // $attributes['class'] = array('form-item');
  // if (!empty($element['#type'])) {
  //   $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  // }
  // if (!empty($element['#name'])) {
  //   $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
  // }
  //
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }

  // Define what we are actually dealing with.
  $container = array('item', 'markup');
  $controls = array('checkboxes', 'radios', 'date');
  $control = array('button', 'checkbox', 'machine_name', 'emailfield', 'fieldset', 'file', 'form', 'image_button','managed_file', 'radio', 'select', 'submit', 'textarea', 'textfield', 'value', 'weight');
  $component = array('password', 'password_confirm', 'tableselect', 'text_format', 'vertical_tabs');

  // What doesn't come through here:
  // - actions, container, fieldset
  // - text_format - Ugh...
  // - machine_name is done with JavaScript but treated like a regular component.
  //
  if (!empty($element['#type'])) {
    switch ($element['#type']) {
      case in_array($element['#type'], $control):
        $attributes['class'][] = 'form-control';
        break;
      case in_array($element['#type'], $controls):
        $attributes['class'][] = 'form-controls';
        break;
      case in_array($element['#type'], $container):
        $attributes['class'][] = 'form-container';
        break;
      case in_array($element['#type'], $component):
        $attributes['class'][] = 'form-component';
        // These usally require more complex code.
        $attributes['data-component'] = strtr($element['#type'], '_', '-');
        break;
    }
  }

  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<div class="prepend">' . $element['#field_prefix'] . '</div> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <div class="append">' . $element['#field_suffix'] . '</div>' : '';

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $vars);
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= ' ' . theme('form_element_label', $vars) . "\n";
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

/**
 * Overrides theme_password().
 * - Stop adding the size attribute unless it's different than the default.
 */
function base_password($vars) {
  $element = $vars['element'];
  $element['#attributes']['type'] = 'password';

  // Stop fucking with the size of elements just for the hell of it.
  $attrs = array('id', 'name', 'size', 'maxlength');
  if ($element['#size'] == 60) {
    $attrs = array_diff($attrs, array('size'));
  }
  element_set_attributes($element, $attrs);

  // Required for .required, .error classes.
  _form_set_class($element);

  return '<input' . drupal_attributes($element['#attributes']) . '>';
}

/**
 * Overrides theme_radio().
 * - Remove .form-radio class.
 */
function base_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio'));

  return '<input' . drupal_attributes($element['#attributes']) . '>';
}

/**
 * Overrides theme_radios().
 * - Add .form-container class to the wrapper.
 */
function base_radios($variables) {
  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  $attributes['class'] = 'form-container form-radios';
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] .= ' ' . implode(' ', $element['#attributes']['class']);
  }
  if (isset($element['#attributes']['title'])) {
    $attributes['title'] = $element['#attributes']['title'];
  }
  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
}

/**
 * Overrides theme_select().
 * - Removes the .form-select class.
 */
function base_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));

  // Required for .required, .error classes.
  _form_set_class($element);

  return '<select' . drupal_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select>';
}

/**
 * Overrides theme_textarea().
 * - Stop adding the cols attribute unless it's different than the default.
 * - Stop adding the .form-textarea class.
 * - Remove the .form-textarea-wrapper class and wrapper. It's not even
 *   extendable via preprocess.
 * - Remove resizable textarea functionality.
 */
function base_textarea($vars) {
  $element = $vars['element'];

  // Stop fucking with the size of elements just for the hell of it.
  $attrs = array('id', 'name', 'cols', 'rows');
  if ($element['#cols'] == 60) {
    $attrs = array_diff($attrs, array('cols'));
  }
  element_set_attributes($element, $attrs);

  // Required for .required, .error classes.
  _form_set_class($element);

  // $wrapper_attributes = array(
  //   'class' => array('form-textarea-wrapper'),
  // );

  // Add resizable behavior.
  // if (!empty($element['#resizable'])) {
  //   drupal_add_library('system', 'drupal.textarea');
  //   $wrapper_attributes['class'][] = 'resizable';
  // }

  // $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output = '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  // $output .= '</div>';
  return $output;
}

/**
 * Overrides theme_textfield().
 * - Stop adding the size attribute unless it's different than the default.
 * - @todo Make autocomplete consistent with other widgets.
 */
function base_textfield($vars) {
  $element = $vars['element'];
  $element['#attributes']['type'] = 'text';

  // Stop fucking with the size of elements just for the hell of it.
  $attrs = array('id', 'name', 'value', 'size', 'maxlength');
  if ($element['#size'] == 60) {
    $attrs = array_diff($attrs, array('size'));
  }
  element_set_attributes($element, $attrs);

  // Required for .required, .error classes.
  _form_set_class($element);

  // @todo Get rid of this shit.
  // This whole block should not fucking be hacked in here like this.
  $extra = '';
  if ($element['#autocomplete_path'] && drupal_valid_path($element['#autocomplete_path'])) {
    drupal_add_library('system', 'drupal.autocomplete');
    $element['#attributes']['class'][] = 'form-autocomplete';

    $attributes = array();
    $attributes['type'] = 'hidden';
    $attributes['id'] = $element['#attributes']['id'] . '-autocomplete';
    $attributes['value'] = url($element['#autocomplete_path'], array('absolute' => TRUE));
    $attributes['disabled'] = 'disabled';
    $attributes['class'][] = 'autocomplete';
    $extra = '<input' . drupal_attributes($attributes) . '>';
  }

  // 3rd time, 3rd different method of processing attributes for the same
  // fucking input.
  $output = '<input' . drupal_attributes($element['#attributes']) . '>';

  return $output . $extra;
}

/**
 * Overrides theme_text_format_wrapper().
 * Of course this function is crap, but since no effort is made to classify
 * form output, this is the kind of bullshit we end up with.
 * - @todo Make this use theme_container() or theme_widget() instead.
 */
function base_text_format_wrapper($vars) {
  $element = $vars['element'];
  $output = '<div class="form-component" data-component="text-format">';
  $output .= $element['#children'];
  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . '</div>';
  }
  $output .= "</div>\n";

  return $output;
}
