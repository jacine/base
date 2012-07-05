<?php

/**
 * @file
 * Displays the search form block.
 *
 * Available variables:
 * - $search_form: The complete search form ready for print.
 * - $search: Associative array of search elements. Can be used to print each
 *   form element separately.
 *
 * Default elements within $search:
 * - $search['search_block_form']: Text input area wrapped in a div.
 * - $search['actions']: Rendered form buttons.
 * - $search['hidden']: Hidden form elements. Used to validate forms when
 *   submitted.
 *
 * Modules can add to the search form, so it is recommended to check for their
 * existence before printing. The default keys will always exist. To check for
 * a module-provided field, use code like this:
 * @code
 *   <?php if (isset($search['extra_field'])): ?>
 *     <div class="extra-field">
 *       <?php print $search['extra_field']; ?>
 *     </div>
 *   <?php endif; ?>
 * @endcode
 *
 * @see template_preprocess_search_block_form()
 */

// This is a really asinine implementation. The work done in
// template_preprocess_search_block_form() to give "helper" variables to themers
// prevent this form from being themed like other forms in Drupal. Sigh.
// The hidden variables which are required for the form to actually work are
// already rendered into the $search['hidden'] variable, so including
// drupal_render_children() doesn't work.  Way to be consistent Drupal...
// Note: The only desired changes here were to remove the wrappers.
//   - The .container-inline <div>.
//   - The .form-actions <div>.
//   - The .form-item <div>

unset($form['search_block_form']['#theme_wrappers']);

?>
<?php print render($form['search_block_form']); ?>
<?php print render($form['actions']['submit']); ?>
<?php print $search['hidden']; ?>
