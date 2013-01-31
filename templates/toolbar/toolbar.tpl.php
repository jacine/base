<?php

/**
 * @file
 * Default template for admin toolbar.
 *
 * Available variables:
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default value has the following:
 *   - toolbar: The current template type, i.e., "theming hook".
 * - $toolbar['toolbar_user']: User account / logout links.
 * - $toolbar['toolbar_menu']: Top level management menu links.
 * - $toolbar['toolbar_drawer']: A place for extended toolbar content.
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_toolbar()
 *
 * @ingroup themeable
 */

// There's no reason the home link should be in a <ul> by itself!?
$toolbar['toolbar_user']['#links'] = array_merge($toolbar['toolbar_home']['#links'], $toolbar['toolbar_user']['#links']);

?>
<a id="toolbarToggle" class="toolbar-toggle" href="#toolbar"><span class="visually-hidden"><?php print t('Toggle toolbar'); ?></span></a>
<div id="toolbar" class="toolbar" style="display: none">
  <div class="toolbar-user">
    <?php print render($toolbar['toolbar_user']); ?>
  </div>
  <div class="toolbar-menu">
    <?php print render($toolbar['toolbar_menu']); ?>
    <?php if ($toolbar['toolbar_drawer']):?>
      <?php print render($toolbar['toolbar_toggle']); ?>
    <?php endif; ?>
  </div>
  <div class="toolbar-drawer">
    <?php print render($toolbar['toolbar_drawer']); ?>
  </div>
</div>
